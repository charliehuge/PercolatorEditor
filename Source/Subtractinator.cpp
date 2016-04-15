#include "Subtractinator.h"
#include <math.h>
#include "MusicMathUtil.h"

namespace DCSynths
{
	enum ParamId
	{
		p_Detune = 0,
		p_Slide,
		p_Cutoff,
		p_Resonance,
		p_EnvAmt,
		p_Attack,
		p_Decay,
		p_Sustain,
		p_Release,
		COUNT
	};

	Subtractinator::Subtractinator(int sampleRate) : 
		Instrument(sampleRate), 
		_osc1(_sampleDuration), 
		_osc2(_sampleDuration), 
		_filter(_sampleDuration), 
		_currentNote(NoteInfo::Default()), 
		_nextNote(NoteInfo::Default())
	{
		_params = {
			ParamInfo("detune", 0, 0, 1),
			ParamInfo("slide", 0, 0, 0.5),
			ParamInfo("cutoff", 0, 0, 1),
			ParamInfo("resonance", 0, 0, 1),
			ParamInfo("env amt", 0, 0, 1),
			ParamInfo("attack", 0, 0, 0.25),
			ParamInfo("decay", 0, 0, 0.25),
			ParamInfo("sustain", 0, 0, 1),
			ParamInfo("release", 0, 0, 0.25)
		};
	}

	Subtractinator::~Subtractinator()
	{
	}

	void Subtractinator::PlayNote(const NoteInfo& noteInfo)
	{
		// if we have a current note, queue this one up
		// otherwise, just set this one as the current note
		if (_currentNote.State == NoteState::Deactivated)
		{
			_currentNote = noteInfo;
			SetFrequency(DCMusicMathUtil::MidiNoteToFrequency(noteInfo.NoteNumber));
		}
		else
		{
			_nextNote = noteInfo;
		}
	}

	void Subtractinator::ReleaseNote(const NoteInfo& noteInfo)
	{
		// always set release to the next note
		// that way, if there's no current note playing, we do nothing
		_nextNote = noteInfo;
	}

	void Subtractinator::SetParameter(int idx, double value)
	{
		if (idx >= 0 && idx < _params.size())
		{
			_params[idx].Value = fmax(_params[idx].Min, fmin(value, _params[idx].Max));

			switch (idx)
			{
			case p_Detune:
				break;
			case p_Slide:
				break;
			case p_Cutoff:
				break;
			case p_Resonance:
				_filter.SetResonance(value);
				break;
			case p_EnvAmt:
				break;
			case p_Attack:
				break;
			case p_Decay:
				break;
			case p_Sustain:
				break;
			case p_Release:
				break;
			}
		}
	}

	void Subtractinator::ProcessBuffer(float *buffer, int numSamples)
	{
		// don't bother if we're idling
		if (_currentNote.State == NoteState::Deactivated)
		{
			return;
		}

		auto sIdx = 0;

		// if we're waiting to play, advance to the sample we're waiting for
		// if that's beyond this buffer, return early
		if (_currentNote.State == NoteState::On && _currentNote.DelaySamples >= 0)
		{
			sIdx = _currentNote.DelaySamples;
			_currentNote.DelaySamples -= numSamples;

			if (sIdx >= numSamples)
			{
				return;
			}

			StartEnvelope();
		}

		double sample, envAmt;

		for (; sIdx < numSamples; ++sIdx)
		{
			// if there's a next note, see if we should switch to that
			if (_nextNote.State != NoteState::Deactivated && --_nextNote.DelaySamples < 0)
			{
				switch (_nextNote.State)
				{
				case NoteState::On: 
					SetFrequency(DCMusicMathUtil::MidiNoteToFrequency(_nextNote.NoteNumber));
					StartEnvelope();
					break;
				case NoteState::Off: 
					_env.Release();
					break;
				case NoteState::Deactivated: break;
				}

				_currentNote = _nextNote;
				_nextNote.State = NoteState::Deactivated;
			}

			envAmt = _env.GetLevel();

			if (envAmt > 0.0)
			{
				// avoid phase cancellation when we're not detuned
				if (_params[p_Detune].Value > 0.0)
				{
					sample = _osc1.GetSample() + _osc2.GetSample();
				}
				else
				{
					sample = _osc1.GetSample();
				}

				_filter.SetCutoff(_params[p_Cutoff].Value + _params[p_EnvAmt].Value * envAmt);
				sample = _filter.GetSample(sample);
			}
			else
			{
				// if we have no more notes queued up, return early
				if (_nextNote.State == NoteState::Deactivated)
				{
					_filter.Init();
					_currentNote.State = NoteState::Deactivated;
					return;
				}

				sample = 0.0;
			}

			buffer[sIdx] += static_cast<float>(sample);
		}
	}

	void Subtractinator::SetFrequency(double frequency)
	{
		auto detune = _params[p_Detune].Value;
		auto slideSamps = static_cast<int>(_params[p_Slide].Value * _sampleRate);

		if (detune > 0.0)
		{
			_osc1.SetFrequency(frequency * pow(2.0, detune / 12.0), slideSamps);
			_osc2.SetFrequency(frequency * pow(2.0, -detune / 12.0), slideSamps);
		}
		else
		{
			_osc1.SetFrequency(frequency, slideSamps);
			_osc2.SetFrequency(frequency, slideSamps);
		}
	}

	void Subtractinator::StartEnvelope()
	{
		_env.SetEnvelope(
			static_cast<int>(_params[p_Attack].Value * _sampleRate), 
			static_cast<int>(_params[p_Decay].Value * _sampleRate), 
			_params[p_Sustain].Value, 
			static_cast<int>(_params[p_Release].Value * _sampleRate));
		_env.Start();
	}
}
