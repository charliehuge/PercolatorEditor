#include "ADSR.h"
#include <math.h>

namespace DCSynths
{
	ADSR::ADSR(): _state(EnvelopeState::Idle), _output(0), _attackInc(-1), _decayInc(-1), _sustainLevel(0), _releaseInc(-1)
	{
	}

	ADSR::~ADSR()
	{
	}

	void ADSR::Start()
	{
		_state = EnvelopeState::Attack;
	}

	void ADSR::Release()
	{
		if (_state == EnvelopeState::Idle)
		{
			return;
		}

		_state = EnvelopeState::Release;
	}

	void ADSR::SetEnvelope(int attackSamples, int decaySamples, double sustainLevel, int releaseSamples)
	{
		_sustainLevel = fmax(0.0, fmin(sustainLevel, 1.0));
		_attackInc = attackSamples > 0 ? 1.0 / attackSamples : -1;
		_decayInc = decaySamples > 0 ? (1.0 - _sustainLevel) / decaySamples : -1;
		_releaseInc = releaseSamples > 0 ? _sustainLevel / releaseSamples : -1;
	}

	double ADSR::GetLevel()
	{
		switch (_state)
		{
		case EnvelopeState::Idle:
			break;
		case EnvelopeState::Attack:
			if (_attackInc < 0)
			{
				_output = 1.0;
				_state = EnvelopeState::Decay;
				break;
			}

			_output += _attackInc;

			if (_output > 1.0)
			{
				_output = 1.0;
				_state = EnvelopeState::Decay;
			}

			break;
		case EnvelopeState::Decay:
			if (_decayInc < 0)
			{
				_output = _sustainLevel;
				_state = EnvelopeState::Sustain;
				break;
			}

			_output -= _decayInc;

			if (_output < _sustainLevel)
			{
				_output = _sustainLevel;
				_state = EnvelopeState::Sustain;
			}
			break;
		case EnvelopeState::Sustain:
			break;
		case EnvelopeState::Release:
			if (_releaseInc < 0)
			{
				_output = 0.0;
				_state = EnvelopeState::Idle;
				break;
			}

			_output -= _releaseInc;

			if (_output < 0.0)
			{
				_output = 0.0;
				_state = EnvelopeState::Idle;
			}
			break;
		default:
			break;
		}

		return _output;
	}
}
