#include "LPFFourPole.h"
#include <math.h>

namespace DCSynths
{
	LPFFourPole::LPFFourPole(double sampleDuration)
	{
		_sampleDuration = sampleDuration;
		_cutoff = 110;
		_resonance = 0;
		Init();
	}

	LPFFourPole::~LPFFourPole()
	{
	}

	void LPFFourPole::Init()
	{
		// initialize values
		_y1 = _y2 = _y3 = _y4 = _oldx = _oldy1 = _oldy2 = _oldy3 = 0;
		Recalculate();
	};

	void LPFFourPole::Recalculate()
	{
		auto f = (_cutoff + _cutoff) * _sampleDuration; //[0 - 1]
		_p = f*(1.8 - 0.8*f);
		_k = _p + _p - 1.0;

		auto t = (1.0 - _p)*1.386249;
		auto t2 = 12.0 + t*t;
		_r = _resonance*(t2 + 6.0*t) / (t2 - 6.0*t);
	};

	double LPFFourPole::GetSample(double input)
	{
		// process input
		_x = input - _r * _y4;

		//Four cascaded onepole filters (bilinear transform)
		_y1 = _x * _p + _oldx * _p - _k * _y1;
		_y2 = _y1 * _p + _oldy1 * _p - _k * _y2;
		_y3 = _y2 * _p + _oldy2 * _p - _k * _y3;
		_y4 = _y3 * _p + _oldy3 * _p - _k * _y4;

		//Clipper band limited sigmoid
		_y4 -= (_y4 * _y4 * _y4) / 6.0;

		_oldx = _x;
		_oldy1 = _y1;
		_oldy2 = _y2;
		_oldy3 = _y3;
		return _y4;
	}

	void LPFFourPole::SetCutoff(double cutoff)
	{
		cutoff = fmax(0.0, fmin(cutoff, 1.0));
		_cutoff = cutoff * cutoff * cutoff * (CUTOFF_MAX - CUTOFF_MIN) + CUTOFF_MIN;
		Recalculate();
	}

	void LPFFourPole::SetResonance(double resonance)
	{
		resonance = fmax(0.0, fmin(resonance, 1.0));
		_resonance = resonance * (RES_MAX - RES_MIN) + RES_MIN;
		Recalculate();
	}
}
