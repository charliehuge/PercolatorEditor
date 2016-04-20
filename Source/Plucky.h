/*
  ==============================================================================

    Plucky.h
    Created: 15 Apr 2016 2:31:15pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef PLUCKY_H_INCLUDED
#define PLUCKY_H_INCLUDED

#include "Instrument.h"

#define PLUCKY_VOICES 4
#define PLUCKY_NOISE_SIZE 2048
#define PLUCKY_BUFFER_SIZE 2048

namespace DCSynths
{
	class PluckedStringVoice
	{
	public:
		PluckedStringVoice();

		void Excite(double frequency, double sampleRate, double decay);

		float GetSample(double decay);

	private:
		double _noise[PLUCKY_NOISE_SIZE];
		double _buffer[PLUCKY_BUFFER_SIZE];
		size_t _pos;
		size_t _bufferSize;
	};

	class Plucky : public Instrument
	{
	public:
		explicit Plucky(int sampleRate);

		~Plucky();

		void PlayNote(const NoteInfo& noteInfo) override;
		void ReleaseNote(const NoteInfo& noteInfo) override;
		void SetParameter(int idx, double value) override;
		void ProcessBuffer(float* buffer, int numSamples) override;

	private:
		PluckedStringVoice _voices[PLUCKY_VOICES];
		double _decay;
		int _nextVoice;
	};
}

#endif  // PLUCKY_H_INCLUDED
