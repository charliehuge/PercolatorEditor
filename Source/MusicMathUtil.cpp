/*
  ==============================================================================

    MusicMathUtil.cpp
    Created: 14 Apr 2016 5:30:45pm
    Author:  charl_000

  ==============================================================================
*/

#include "MusicMathUtil.h"
#include <math.h>
#include <algorithm>

namespace DCMusicMathUtil
{
	double MidiNoteToFrequency(int midiNote)
	{
		return C0 * SemitonesToPitchMultiplier(midiNote);
	}

	double ScaleIntervalToFrequency(int interval, int octave, Note rootNote, ScaleMode scaleMode)
	{
		return MidiNoteToFrequency(ScaleIntervalToMidiNote(interval, octave, rootNote, scaleMode));
	}

	int ScaleIntervalToMidiNote(int interval, int octave, Note rootNote, ScaleMode scaleMode)
	{
		// clamp to [1, 7], subtract 1 to shift to zero-indexed
		interval = std::max(1, std::min(interval, 7)) - 1;
		// don't want octaves less than zero
		octave = std::max(0, octave);
		// get the shifted ionian root by subtracting the root note by the scale mode offset
		auto smInt = static_cast<int>(scaleMode);
		auto ionianRoot = static_cast<int>(rootNote) - smInt;

		int ionianInterval = (interval + smInt) % 7;
		int extraOctaves = (interval + smInt) / 7;
		int semitones;

		if (ionianInterval == 0)
		{
			semitones = 0;
		}
		else if (ionianInterval == 1)
		{
			semitones = 2;
		}
		else if (ionianInterval == 2)
		{
			semitones = 4;
		}
		else if (ionianInterval == 3)
		{
			semitones = 5;
		}
		else if (ionianInterval == 4)
		{
			semitones = 7;
		}
		else if (ionianInterval == 5)
		{
			semitones = 9;
		}
		else// if (ionianInterval == 6)
		{
			semitones = 11;
		}

		return (octave + extraOctaves) * 12 + semitones + ionianRoot;
	}

	double SemitonesToPitchMultiplier(double semitones)
	{
		return pow(2.0, semitones / 12.0);
	}

	double PitchMultiplierToSemitones(double pitchMultiplier)
	{
		return log2(pitchMultiplier) * 12.0;
	}
}
