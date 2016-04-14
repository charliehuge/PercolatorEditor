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
	StopNote::StopNote(int note) : _note(note)
	{
	}

	Result StopNote::OnTick(double tickTime)
	{
		return Result::Success;
	}
}
