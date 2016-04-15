#pragma once

namespace DCSynths
{
	class SawOsc
	{
	public:
		explicit SawOsc(double sampleDuration);

		~SawOsc();

		void Reset();

		void SetFrequency(double frequency, int slideSamples);

		double GetSample();

	private:
		double _sampleDuration;
		double _phase;
		double _phaseInc;
		double _phaseIncSlideInc;
		int _phaseIncSlideSamples;
	};
}
