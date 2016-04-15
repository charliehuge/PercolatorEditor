/*
  ==============================================================================

    dctree_StopNote.cpp
    Created: 13 Apr 2016 1:02:52pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_StopNote.h"

namespace DCTree
{
	StopNote::StopNote(int note, DCSynths::Instrument *instrument) : _note(note), _instrument(instrument)
	{
	}

	Result StopNote::OnTick(double tickTime)
	{
		_instrument->ReleaseNote(_note, 0);
		return Result::Success;
	}
}
