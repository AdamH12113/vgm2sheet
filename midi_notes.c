#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include "midi_types.h"
#include "midi_strings.h"

uint16_t BE_Read16(uint8_t *value);
uint32_t BE_Read32(uint8_t *value);
struct var_len VarLen_Read(uint8_t *value);
void MIDI_State_Machine(uint8_t *data, size_t totalSize);
size_t Process_Chunk(uint8_t *data);
void Process_Track(uint8_t *data);
void Process_MIDI_Event(uint8_t status, uint8_t *data);


//MIDI state variables. So far, this is just the timing parameters.
static uint32_t ppqn = 30, tempo = 500000;
uint32_t g_time = 0;


int main(int argc, char *argv[])
{
	FILE *inFile;
	uint8_t *midiData;
	size_t dataSize;
	int nextByte, c;
	
	//Check for valid command line arguments
	if (argc != 3)
	{
		fprintf(stderr, "Usage:\n\tmidi_notes <input filename> <PPQN>\n\n");
		return EXIT_FAILURE;
	}
	
	//Save the PPQN value
	ppqn = strtol(argv[2], NULL, 10);

	//Open the input file
	inFile = fopen(argv[1], "rb");
	if (inFile == NULL)
	{
		fprintf(stderr, "Error opening file: %s\n\n", strerror(errno));
		return EXIT_FAILURE;
	}
	
	//Read the input file into memory. We could parse it one byte at a time,
	//but I prefer to read all of the data into memory. This makes it easier
	//to tokenize later. But first, we need to allocate memory, and that means
	//we need to know how big the file is. Unfortunately, there's no standard
	//way to do this. We'll have to settle for POSIX compliance instead.
	fseek(inFile, 0, SEEK_END);
	dataSize = (size_t)ftell(inFile);
	rewind(inFile);
	
	midiData = malloc(dataSize);
	if (midiData == NULL)
	{
		fprintf(stderr, "Error allocating memory: %s\n\n", strerror(errno));
		return EXIT_FAILURE;
	}
	fread(midiData, sizeof(uint8_t), dataSize, inFile);
	if (errno != 0)
	{
		fprintf(stderr, "Error reading from file: %s\n\n", strerror(errno));
		return EXIT_FAILURE;
	}
	fclose(inFile);
	
	//Invoke the MIDI state machine to do the real work
	MIDI_State_Machine(midiData, dataSize);
	
	//It's a good habit to manually free the memory
	free(midiData);
}


//MIDI state machine. This is the interface function for interpreting the MIDI
//file and producing converted data.
void MIDI_State_Machine(uint8_t *data, size_t totalSize)
{
	size_t usedSize = 0;
	
	while (usedSize < totalSize)
	{
		usedSize += Process_Chunk(data + usedSize);
	}
}


//Helper functions for reading big-endian values from the byte stream
uint16_t BE_Read16(uint8_t *value)
{
	return (uint16_t)value[0] << 8 |
	       (uint16_t)value[1] << 0;
}

uint32_t BE_Read32(uint8_t *value)
{
	return (uint32_t)value[0] << 24 |
	       (uint32_t)value[1] << 16 |
		   (uint32_t)value[2] << 8 |
		   (uint32_t)value[3] << 0;
}

//The MIDI standard allows for variable-length numbers. These are given as a
//sequence of up to four bytes. The lower 7 bits are numerical data, and the
//most-significant bit is a flag which indicates that more bytes are needed.
//The maximum possible number of data bits is 28.
struct var_len VarLen_Read(uint8_t *value)
{
	struct var_len v = {0, 0};
	int c;
	
	for (c = 0; c < 4; c++)
	{
		v.value <<= 7;
		v.value |= (value[c] & 0x7F);
		v.size++;
		
		if ((value[c] & 0x80) == 0x00)
			break;
	}
	
	return v;
}
	

//Process a chunk. First, get the type and length, then either process the
//header (if it's a header chunk) or call a helper function to process events
//(if it's a track chunk). Finally, return the number of bytes processed.
size_t Process_Chunk(uint8_t *dataStart)
{
	struct midi_chunk chunk;
	struct midi_header header;
	uint16_t temp;
	
	//Read the chunk type and length. MIDI bytes are in big-endian order, so we
	//can't just do 32-bit reads even if we wanted to be lazy.
	chunk.type = BE_Read32(dataStart);
	chunk.length = BE_Read32(dataStart + sizeof(uint32_t));
	chunk.data = dataStart + 2*sizeof(uint32_t);

	//Output the chunk type and length. If this is a header chunk, process it
	//now. If it's a track chunk, call a helper function.
	if (chunk.type == MIDI_HEADER_CHUNK)
	{
		header.format = BE_Read16(chunk.data);
		header.tracks = BE_Read16(chunk.data + sizeof(uint16_t));
		temp = BE_Read16(chunk.data + 2*sizeof(uint16_t));
		header.division = temp & 0x7FFF;
		header.divType = temp >> 15;
		//Ignore any extra data as per the MIDI spec
		
		//Save the division
		if (header.divType != 0)
		{
			fprintf(stderr, "Error: SMTPE timing is not supported\n\n");
			exit(EXIT_FAILURE);
		}
		
		printf("\nHeader chunk: length = %" PRIu32 ", format = %" PRIu16
		       ", tracks = %" PRIu16 ", division = %" PRIu16 ", div type = %"
			   PRIu16 "\n", chunk.length, header.format, header.tracks,
			   header.division, header.divType);
	} else if (chunk.type == MIDI_TRACK_CHUNK)
	{
		printf("\nTrack chunk: length = %" PRIu32 "\n", chunk.length);
		Process_Track(chunk.data);
	} else
	{
		fprintf(stderr, "\nUnknown chunk type: %08" PRIx32 "\n", chunk.type);
		exit(EXIT_FAILURE);
	}

	return chunk.length + 2*sizeof(uint32_t);
}


//Process a track. This involves reading a series of events, which may be MIDI
//events, SysEx events, or meta events. The events are all different lengths, so
//we have to keep track of the data position here. The track will conclude with
//an End of Track meta event, so we don't need to know the overall length.
void Process_Track(uint8_t *data)
{
	struct var_len v;
	uint32_t eventLen;
	uint8_t status, metaType;
	size_t pos = 0;
	
	//Main event loop
	while (1)
	{
		//Get the delta time and print the current time
		v = VarLen_Read(data + pos);
		g_time += v.value;
		pos += v.size;

		//Figure out what kind of event this is
		status = data[pos++];
		if ((status & 0xF0) < 0xF0)
		{
			//MIDI event -- process
			Process_MIDI_Event(status, data + pos);
			if ((status & 0xF0) == 0xC0 || (status & 0xF0) == 0xD0)
				pos++;
			else
				pos += 2;
		} else if (status == 0xF0 || status == 0xF7)
		{
			//SysEx event -- ignore
			v = VarLen_Read(data + pos);
			eventLen = v.value;
			pos += v.size;
			pos += eventLen;
		} else if (status == 0xFF)
		{
			//Meta event -- process if it sets the tempo or ends the track
			metaType = data[pos++];
			v = VarLen_Read(data + pos);
			eventLen = v.value;
			pos += v.size;
			
			//If this is a Set Tempo event, update the tempo
			if (metaType == MIDI_META_SET_TEMPO)
			{
				tempo = (uint32_t)data[pos+0] << 16 |
				        (uint32_t)data[pos+1] << 8  |
						(uint32_t)data[pos+2];
				printf("New tempo: %" PRIu32 "\n", tempo);
			}
			
			pos += eventLen;

			if (metaType == MIDI_META_END_OF_TRACK)
				break;
		} else
		{
			fprintf(stderr, "Unknown event type %02" PRIx8 "\n", status);
			exit(EXIT_FAILURE);
		}
	}
}

struct NoteConverter {float duration; const char *name;};

struct NoteConverter noteLengths[] =
{
	{0.015625, "64"},
	{0.03125,  "32"},
	{0.0625,   "16"},
	{0.125,     "8"},
	{0.25,      "4"},
	{0.5,       "2"},
	{1.0,       "1"},
	{2.0,  "\breve"}
};

const size_t numNotes = sizeof(noteLengths)/sizeof(struct NoteConverter);
		
//Process a MIDI channel voice or mode message. These all have fixed lengths,
//with one or two data bytes after the status byte.
void Process_MIDI_Event(uint8_t status, uint8_t *data)
{
	static uint32_t noteStarts[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	const char *note;
	uint32_t dTime;
	uint8_t msgType, msgIndex, channel, octave;
	float duration;

	//Parse the status byte
	msgType = status & 0xF0;
	msgIndex = (msgType >> 4) - 0x8;
	channel = status & 0x0F;
	dTime = g_time - noteStarts[channel];
	
	duration = (float)dTime / (float)(4 * ppqn);
	
	//Parse the first data byte as a key (note) just in case
	note = noteNames[data[0] % 12];
	octave = data[0] / 12;

	switch (msgType)
	{
		case MIDI_EVENT_NOTE_ON:
			if (g_time > noteStarts[channel])
			{
				printf("ch %2" PRIu8 "  ", channel);
				printf("Rest: %" PRIu32  "      \t%1.4f\n", dTime, duration);
			}
			noteStarts[channel] = g_time;
			break;
		case MIDI_EVENT_NOTE_OFF:
			printf("ch %2" PRIu8 "  ", channel);
			printf("Note: %-4" PRIu32 "  %s%" PRIu8 "\t%1.4f   ", dTime, note, octave, duration);
			noteStarts[channel] = g_time;
			
			if (duration > 0.9*2.0 && duration < 1.1*2.0)
			{
				printf("2/1");
			} else if (duration > 0.9*1.0 && duration < 1.1*1.0)
			{
				printf("1/1");
			} else if (duration > 0.9*0.5 && duration < 1.1*0.5)
			{
				printf("1/2");
			} else if (duration > 0.9*0.25 && duration < 1.1*0.25)
			{
				printf("1/4");
			} else if (duration > 0.9*0.125 && duration < 1.1*0.125)
			{
				printf("1/8");
			} else if (duration > 0.9*0.0625 && duration < 1.1*0.0625)
			{
				printf("1/16");
			} else if (duration > 0.9*0.375 && duration < 1.1*0.375)
			{
				printf("1/4 .");
			} else if (duration > 0.9*0.625 && duration < 1.1*0.625)
			{
				printf("1/2 + 1/8");
			} else if (duration > 0.9*0.75 && duration < 1.1*0.75)
			{
				printf("1/2 .");
			} else if (duration > 0.9*1.75 && duration < 1.1*1.75)
			{
				printf("1/1 + 1/2 .");
			} else if (duration > 0.9*0.1875 && duration < 1.1*0.1875)
			{
				printf("1/8 .");
			} else if (duration > 0.9*1.125 && duration < 1.1*1.125)
			{
				printf("1/1 + 1/8");
			} else if (duration > 0.9*0.875 && duration < 1.1*0.875)
			{
				printf("1/2 + 1/4 .");
			} else
			{
				printf("UNKNOWN");
			}
			printf("\n");
			break;
		default:
			//Ignore all other events
			break;
	}
}
			
			
			
