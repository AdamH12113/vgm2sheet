//MIDI standard file data structures

#include <stdint.h>

//Variable-length values can be from 1-4 bytes long, with 1 bit per byte used as
//a flag. It's helpful to be able to return both the numerical value and its
//byte length in the data stream.
struct var_len
{
	uint32_t value;
	size_t size;
};

//MIDI files consist of a series of chunks. Each chunk begins with fixed-size
//type and length fields along with a variable-length data segment.
struct midi_chunk
{
	uint32_t type;
	uint32_t length;
	uint8_t *data;
};

//Chunk types. These are ASCII for "Mthd" and "Mtrk".
#define MIDI_HEADER_CHUNK 0x4D546864
#define MIDI_TRACK_CHUNK  0x4D54726B

//MIDI header data
struct midi_header
{
	uint16_t format;    //Track format
	uint16_t tracks;    //Number of track chunks in the file
	uint16_t division;  //Default unit of time
	uint16_t divType;   //0: ticks/quarter, 1: fps and ticks/frame
};

//Track formats. Format 0 files have only one track. Format 1 files have
//multiple tracks which are played simultaneously. Format 2 files have multiple
//tracks which are played sequentially.
#define MIDI_FORMAT_ONETRACK      0
#define MIDI_FORMAT_SIMULTANEOUS  1
#define MIDI_FORMAT_SEQUENTIAL    2

//MIDI event. These could be channel voice messages or channel mode messages,
//and are always 2-3 bytes long.
struct midi_event
{
	uint8_t status; //Command and channel -- no idea why it's called "status"
	uint8_t data1;  //First data byte
	uint8_t data2;  //Second data byte -- not all events have one
};

//MIDI event types. These include channel voice messages, System Exclusive
//events, and meta events.
#define MIDI_EVENT_NOTE_OFF          0x80
#define MIDI_EVENT_NOTE_ON           0x90
#define MIDI_EVENT_POLY_KEY_PRESSURE 0xA0
#define MIDI_EVENT_CONTROLLER_CHANGE 0xB0
#define MIDI_EVENT_PROGRAM_CHANGE    0xC0
#define MIDI_EVENT_CHAN_KEY_PRESSURE 0xD0
#define MIDI_EVENT_PITCH_BEND        0xE0
#define MIDI_EVENT_SYSEX             0xF0
#define MIDI_EVENT_SYSEX_ESCAPE      0xF7
#define MIDI_EVENT_META              0xFF

//MIDI channel mode messages. The controller change command value is reused for
//the channel mode messages, so they'll be defined by their data byte.
#define MIDI_MODE_ALL_SOUND_OFF      0x78
#define MIDI_MODE_LOCAL_CONTROL      0x7A
#define MIDI_MODE_ALL_NOTES_OFF      0x7B
#define MIDI_MODE_OMNI_OFF           0x7C
#define MIDI_MODE_OMNI_ON            0x7D
#define MIDI_MODE_MONO_ON            0x7E
#define MIDI_MODE_POLY_ON            0x7F

//Meta events. These are only used to provide information. They don't affect the
//audio output. As with the channel mode messages, these are defined by their
//data byte.
#define MIDI_META_SEQUENCE           0x00
#define MIDI_META_TEXT               0x01
#define MIDI_META_COPYRIGHT          0x02
#define MIDI_META_TRACK_NAME         0x03
#define MIDI_META_INSTRUMENT_NAME    0x04
#define MIDI_META_LYRIC              0x05
#define MIDI_META_MARKER             0x06
#define MIDI_META_CUE_POINT          0x07
#define MIDI_META_CHANNEL_PREFIX     0x20
#define MIDI_META_END_OF_TRACK       0x2F
#define MIDI_META_SET_TEMPO          0x51
#define MIDI_META_SMTPE_OFFSET       0x54
#define MIDI_META_TIME_SIGNATURE     0x58
#define MIDI_META_KEY_SIGNATURE      0x59
#define MIDI_META_SEQUENCER_SPECIFIC 0x7F




