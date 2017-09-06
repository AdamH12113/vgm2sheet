//String descriptions for various MIDI features. These are helpful for producing
//readable debug output. They're set up as arrays where the array index is the
//corresponding data byte in the event.

//MIDI controls for controller change messages. This includes the channel mode
//messages. The array index is the second data byte in the message.
const char *midi_controls[] =
{
	"Bank Select",
	"Modulation Wheel or Lever",
	"Breath Controller",
	"Undefined",
	"Foot Controller",
	"Portamento Time",
	"Data Entry MSB",
	"Channel Volume",
	"Balance",
	"Undefined",
	"Pan",
	"Expression Controller",
	"Effect Control 1",
	"Effect Control 2",
	"Undefined",
	"Undefined",
	"GP Controller 1",
	"GP Controller 2",
	"GP Controller 3",
	"GP Controller 4",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"LSB for Bank Select",
	"LSB for Modulation",
	"LSB for Breath",
	"LSB for Undefined",
	"LSB for Foot",
	"LSB for Portamento",
	"LSB for Data Entry",
	"LSB for Channel Volume",
	"LSB for Balance",
	"LSB for Undefined",
	"LSB for Pan",
	"LSB for Expression",
	"LSB for Effect control 1",
	"LSB for Effect control 2",
	"LSB for Undefined",
	"LSB for Undefined",
	"LSB for GP Controller 1",
	"LSB for GP Controller 2",
	"LSB for GP Controller 3",
	"LSB for GP Controller 4",
	"LSB for Undefined",
	"LSB for Undefined",
	"LSB for Undefined",
	"LSB for Undefined",
	"LSB for Undefined",
	"LSB for Undefined",
	"LSB for Undefined",
	"LSB for Undefined",
	"LSB for Undefined",
	"LSB for Undefined",
	"LSB for Undefined",
	"LSB for Undefined",
	"Damper Pedal",
	"Portamento On/Off",
	"Sostenuto On/Off",
	"Soft Pedal On/Off",
	"Legato Footswitch",
	"Hold 2",
	"SC 1 (Sound Variation)",
	"SC 2 (Timbre)",
	"SC 3 (Release Time)",
	"SC 4 (Attack Time)",
	"SC 5 (Brightness)",
	"SC 6 (Decay Time)",
	"SC 7 (Vibrato Rate)",
	"SC 8 (Vibrato Depth)",
	"SC 9 (Vibrato Delay)",
	"SC 10 (undefined)",
	"GP Controller 5",
	"GP Controller 6",
	"GP Controller 7",
	"GP Controller 8",
	"Portamento Control",
	"Undefined",
	"Undefined",
	"Undefined",
	"HR Velocity Prefix",
	"Undefined",
	"Undefined",
	"Effects 1 Depth",
	"Effects 2 Depth",
	"Effects 3 Depth",
	"Effects 4 Depth",
	"Effects 5 Depth",
	"Data ++",
	"Data --",
	"LSB of NRPN",
	"LSB of NRPN",
	"LSB of RPN",
	"MSB of RPN",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Mode: All Sound Off",
	"Mode: Reset All",
	"Mode: Local Control",
	"Mode: All Notes Off",
	"Mode: Omni Off",
	"Mode: Omni On",
	"Mode: Mono On",
	"Mode: Poly On"
};

//MIDI voice messages. The array index is the upper four bits of the status byte
//minus 0x8.
const char *midi_voice_messages[] =
{
	"Note Off",
	"Note On",
	"Polyphonic Key Pressure",
	"Controller Change",
	"Program Change",
	"Channel Key Pressure",
	"Pitch Bend"
};

//Note names, mainly for the note on and off events. The array index is the
//key number (first data byte) mod 12.
const char *noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

//Instrument names for program change events. The array index is the program
//number (first data byte).
const char *midi_instruments[] =
{
	"Acoustic Grand Piano",
	"Bright Acoustic Piano",
	"Electric Grand Piano",
	"Honky-Tonk Piano",
	"Electric Piano 1",
	"Electric Piano 2",
	"Harpsichord",
	"Clavi",
	"Celesta",
	"Glockenspiel",
	"Music Box",
	"Vibraphone",
	"Marimba",
	"Xylophone",
	"Tubular Bells",
	"Dulcimer",
	"Drawbar Organ",
	"Percussive Organ",
	"Rock Organ",
	"Church Organ",
	"Reed Organ",
	"Accordion",
	"Harmonica",
	"Tango Accordion",
	"Acoustic Guitar (nylon)",
	"Acoustic Guitar (steel)",
	"Electric Guitar (jazz)",
	"Electric Guitar (clean)",
	"Electric Guitar (muted)",
	"Overdriven Guitar",
	"Distortion Guitar",
	"Guitar Harmonics",
	"Acoustic Bass",
	"Electric Bass (finger)",
	"Electric Bass (pick)",
	"Fretless Bass",
	"Slap Bass 1",
	"Slap Bass 2",
	"Synth Bass 1",
	"Synth Bass 2",
	"Violin",
	"Viola",
	"Cello",
	"Contrabass",
	"Tremolo Strings",
	"Pizzicato Strings",
	"Orchestral Harp",
	"Timpani",
	"String Ensemble 1",
	"String Ensemble 2",
	"SynthStrings 1",
	"SynthStrings 2",
	"Choir Aahs",
	"Voice Oohs",
	"Synth Voice",
	"Orchestra Hit",
	"Trumpet",
	"Trombone",
	"Tuba",
	"Muted Trumpet",
	"French Horn",
	"Brass Section",
	"SynthBrass 1",
	"SynthBrass 2",
	"Soprano Sax",
	"Alto Sax",
	"Tenor Sax",
	"Baritone Sax",
	"Oboe",
	"English Horn",
	"Bassoon",
	"Clarinet",
	"Piccolo",
	"Flute",
	"Recorder",
	"Pan Flute",
	"Blown Bottle",
	"Skakuhachi",
	"Whistle",
	"Ocarina",
	"Lead 1 (square)",
	"Lead 2 (sawtooth)",
	"Lead 3 (calliope)",
	"Lead 4 (chiff)",
	"Lead 5 (charang)",
	"Lead 6 (voice)",
	"Lead 7 (fifths)",
	"Lead 8 (bass+lead)",
	"Pad 1 (new age)",
	"Pad 2 (warm)",
	"Pad 3 (polysynth)",
	"Pad 4 (choir)",
	"Pad 5 (bowed)",
	"Pad 6 (metallic)",
	"Pad 7 (halo)",
	"Pad 8 (sweep)",
	"FX 1 (rain)",
	"FX 2 (soundtrack)",
	"FX 3 (crystal)",
	"FX 4 (atmosphere)",
	"FX 5 (brightness)",
	"FX 6 (goblins)",
	"FX 7 (echoes)",
	"FX 8 (sci-fi)",
	"Sitar",
	"Banjo",
	"Shamisen",
	"Koto",
	"Kalimba",
	"Bagpipe",
	"Fiddle",
	"Shanai",
	"Tinkle Bell",
	"Agogo",
	"Steel Drums",
	"Woodblock",
	"Taiko Drum",
	"Melodic Tom",
	"Synth Drum",
	"Reverse Cymbal",
	"Guitar Fret Noise",
	"Breath Noise",
	"Seashore",
	"Bird Tweet",
	"Telephone Ring",
	"Helicopter",
	"Applause",
	"Gunshot"
};

//Percussion (drum) names. These are the key values for channel 10. The array
//index is the key value (first data byte) minus 35.
const char *midi_drums[] =
{
	"Acoustic Bass Drum",
	"Bass Drum 1",
	"Side Stick",
	"Acoustic Snare",
	"Hand Clap",
	"Electric Snare",
	"Low Floor Tom",
	"Closed Hi-Hat",
	"High Floor Tom",
	"Pedal Hi-Hat",
	"Low Tom",
	"Open Hi-Hat",
	"Low-Mid Tom",
	"Hi-Mid Tom",
	"Crash Cymbal 1",
	"High Tom",
	"Ride Cymbal 1",
	"Chinese Cymbal",
	"Ride Bell",
	"Tambourine",
	"Splash Cymbal",
	"Cowbell",
	"Crash Cymbal 2",
	"Vibraslap",
	"Ride Cymbal 2",
	"Hi Bongo",
	"Low Bongo",
	"Mute Hi Conga",
	"Open Hi Conga",
	"Low Conga",
	"High Timbale",
	"Low Timbale",
	"High Agogo",
	"Low Agogo",
	"Cabasa",
	"Maracas",
	"Short Whistle",
	"Long Whistle",
	"Short Guiro",
	"Long Guiro",
	"Claves",
	"Hi Wood Block",
	"Low Wood Block",
	"Mute Cuica",
	"Open Cuica",
	"Mute Triangle",
	"Open Triangle"
};

	
	
	
	