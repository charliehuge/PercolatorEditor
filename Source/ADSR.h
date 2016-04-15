#pragma once

namespace DCSynths
{
	enum class EnvelopeState
	{
		Idle = 0,
		Attack,
		Decay,
		Sustain,
		Release
	};

	class ADSR
	{
	public:
		ADSR();

		~ADSR();

		void Start();

		void Release();

		void SetEnvelope(int attackSamples, int decaySamples, double sustainLevel, int releaseSamples);

		double GetLevel();

	private:
		EnvelopeState _state;
		double _output;
		double _attackInc;
		double _decayInc;
		double _sustainLevel;
		double _releaseInc;
	};
}

