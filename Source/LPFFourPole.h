#pragma once

namespace DCSynths
{
	class LPFFourPole
	{
	public:
		explicit LPFFourPole(double sampleDuration);

		~LPFFourPole();

		void Init();

		void Recalculate();

		double GetSample(double input);

		void SetCutoff(double cutoff);

		void SetResonance(double resonance);
	private:
		const double CUTOFF_MIN = 20.0;
		const double CUTOFF_MAX = 10000.0;
		const double RES_MIN = 0.0;
		const double RES_MAX = 0.9;

		double _cutoff;
		double _resonance;
		double _sampleDuration;
		double _y1, _y2, _y3, _y4;
		double _oldx;
		double _oldy1, _oldy2, _oldy3;
		double _x;
		double _r;
		double _p;
		double _k;
	};
}
