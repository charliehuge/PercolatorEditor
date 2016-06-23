/*
  ==============================================================================

    Plucky.cpp
    Created: 15 Apr 2016 2:31:15pm
    Author:  charl_000

  ==============================================================================
*/

#include "Plucky.h"
#include "MusicMathUtil.h"
#include <ctime>
#include <random>

namespace DCSynths
{
	PluckedStringVoice::PluckedStringVoice(): _pos(0), _bufferSize(0)
	{
		for (int i = 0; i < PLUCKY_NOISE_SIZE; ++i)
		{
			_noise[i] = static_cast<double>(rand()) * (1.0 / RAND_MAX) - 0.5;
		}
	}

	void PluckedStringVoice::Excite(double frequency, double sampleRate, double decay)
	{
		_bufferSize = static_cast<size_t>(fmin(sampleRate / frequency, PLUCKY_BUFFER_SIZE));
		_pos = 0;

		for (int i = 0; i < _bufferSize; ++i)
		{
			_buffer[i] = _noise[i % PLUCKY_NOISE_SIZE];
		}
	}

	float PluckedStringVoice::GetSample(double decay)
	{
		if (_bufferSize == 0)
		{
			return 0.0f;
		}

		const int nextPos = (_pos + 1) % _bufferSize;
		_buffer[nextPos] = decay * 0.5 * (_buffer[_pos] + _buffer[nextPos]);
		const auto samp = static_cast<float>(_buffer[_pos]);
		_pos = nextPos;
		return samp;
	}

	Plucky::Plucky(int sampleRate): Instrument(sampleRate), _decay(0.99), _nextVoice(0)
	{
		time_t s;
		time(&s);
		srand(static_cast<unsigned int>(s));
	}

	Plucky::~Plucky()
	{
	}

	void Plucky::PlayNote(const NoteInfo& noteInfo)
	{
		_voices[_nextVoice].Excite(DCMusicMathUtil::MidiNoteToFrequency(noteInfo.NoteNumber), _sampleRate, _decay);
		_nextVoice = (_nextVoice + 1) % PLUCKY_VOICES;
	}

	void Plucky::ReleaseNote(const NoteInfo& noteInfo)
	{
	}

	void Plucky::SetParameter(int idx, double value)
	{
	}

	void Plucky::ProcessBuffer(float* buffer, int numSamples)
	{
		for (int sIdx = 0; sIdx < numSamples; ++sIdx)
		{
			for (int vIdx = 0; vIdx < PLUCKY_VOICES; ++vIdx)
			{
				buffer[sIdx] += _voices[vIdx].GetSample(_decay);
			}
		}
	}
}
