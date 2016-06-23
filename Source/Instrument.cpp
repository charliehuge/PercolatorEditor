/*
  ==============================================================================

    Instrument.cpp
    Created: 14 Apr 2016 4:09:40pm
    Author:  charl_000

  ==============================================================================
*/

#include "Instrument.h"
#include <math.h>

namespace DCSynths
{
	Instrument::Instrument(int sampleRate) : _sampleRate(sampleRate), _sampleDuration(1.0/sampleRate)
	{
	}

	Instrument::~Instrument()
	{
	}

	void Instrument::PlayNote(int midiNote, double velocity, double delaySeconds)
	{
		PlayNote(NoteInfo(NoteState::On, midiNote, fmax(0.0, fmin(velocity, 1.0)), static_cast<int>(delaySeconds * _sampleRate)));
	}

	void Instrument::PlayNote(int midiNote, double velocity, int delaySamples)
	{
		PlayNote(NoteInfo(NoteState::On, midiNote, fmax(0.0, fmin(velocity, 1.0)), delaySamples));
	}

	void Instrument::ReleaseNote(int midiNote, double delaySeconds)
	{
		ReleaseNote(NoteInfo(NoteState::Off, midiNote, 0, static_cast<int>(delaySeconds * _sampleRate)));
	}

	void Instrument::ReleaseNote(int midiNote, int delaySamples)
	{
		ReleaseNote(NoteInfo(NoteState::Off, midiNote, 0, delaySamples));
	}

	int Instrument::GetParameterCount() const
	{
		return _params.size();
	}

	std::vector<ParamInfo> Instrument::GetParameters() const
	{
		return _params;
	}

}
