#pragma once
#include "SawOsc.h"
#include "ADSR.h"
#include "LPFFourPole.h"
#include "Instrument.h"

namespace DCSynths
{
	class Subtractinator : public Instrument
	{
	public:
		explicit Subtractinator(int sampleRate);

		~Subtractinator();

		void PlayNote(const NoteInfo &noteInfo) override;
		
		void ReleaseNote(const NoteInfo &noteInfo) override;
		
		void SetParameter(int idx, double value) override;
		
		void ProcessBuffer(float *buffer, int numSamples) override;

	private:
		void SetFrequency(double frequency);
		void StartEnvelope();

		SawOsc _osc1;
		SawOsc _osc2;
		ADSR _env;
		LPFFourPole _filter;
		NoteInfo _currentNote;
		NoteInfo _nextNote;
	};
}
