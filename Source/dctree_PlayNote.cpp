/*
  ==============================================================================

    dctree_PlayNote.cpp
    Created: 13 Apr 2016 1:02:40pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_PlayNote.h"
#include "JuceHeader.h"

namespace DCTree
{
	PlayNote::PlayNote(int note) : _note(note)
	{
	}

	Result PlayNote::OnTick(double tickTime)
	{
		DBG("PlayNote: " << _note << ", " << tickTime);
		return Result::Success;
	}
}
