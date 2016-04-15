/*
  ==============================================================================

    Instrument.h
    Created: 14 Apr 2016 4:09:40pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef INSTRUMENT_H_INCLUDED
#define INSTRUMENT_H_INCLUDED
#include <string>
#include <vector>

namespace DCSynths
{
	enum class NoteState { On, Off, Deactivated };

	struct NoteInfo
	{
		NoteState State;
		int NoteNumber;
		double Velocity;
		int DelaySamples;

		NoteInfo(NoteState state, int noteNumber, double velocity, int delaySamples): 
			State(state), NoteNumber(noteNumber), Velocity(velocity), DelaySamples(delaySamples)
		{}

		static NoteInfo Default() { return NoteInfo(NoteState::Deactivated, 0, 0, 0); }
	};

	struct ParamInfo
	{
		std::string Name;
		double Value;
		double Min;
		double Max;

		ParamInfo(std::string name, double value, double min, double max) : Name(name), Value(value), Min(min), Max(max) {}
	};

	class Instrument
	{
	public:
		virtual ~Instrument();

		void PlayNote(int midiNote, double velocity = 1, double delaySeconds = 0);

		void PlayNote(int midiNote, double velocity = 1, int delaySamples = 0);

		virtual void PlayNote(const NoteInfo &noteInfo) = 0;

		void ReleaseNote(int midiNote, double delaySeconds = 0);

		void ReleaseNote(int midiNote, int delaySamples = 0);

		virtual void ReleaseNote(const NoteInfo &noteInfo) = 0;

		virtual void SetParameter(int idx, double value) = 0;

		int GetParameterCount() const;

		std::vector<ParamInfo> GetParameters() const;

		virtual void ProcessBuffer(float *buffer, int numSamples) = 0;

	protected:
		explicit Instrument(int sampleRate);

		int _sampleRate;
		double _sampleDuration;
		std::vector<ParamInfo> _params;
	};
}

#endif  // INSTRUMENT_H_INCLUDED
