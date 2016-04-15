/*
  ==============================================================================

    MusicMathUtil.h
    Created: 14 Apr 2016 5:30:45pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef MUSICMATHUTIL_H_INCLUDED
#define MUSICMATHUTIL_H_INCLUDED

namespace DCMusicMathUtil
{
	enum class Note { A = 0, Bb, B, C, Db, D, Eb, E, F, Gb, G, Ab };

	enum class ScaleMode { Ionian = 0, Dorian = 2, Phrygian = 4, Lydian = 5, Mixolydian = 7, Aeolian = 9, Locrian = 11 };

	const double C0 = 8.1757989156;

	double MidiNoteToFrequency(int midiNote);

	double ScaleIntervalToFrequency(int interval, int octave, Note rootNote, ScaleMode scaleMode);

	int ScaleIntervalToMidiNote(int interval, int octave, Note rootNote, ScaleMode scaleMode);

	double SemitonesToPitchMultiplier(double semitones);

	double PitchMultiplierToSemitones(double pitchMultiplier);
}

#endif  // MUSICMATHUTIL_H_INCLUDED
