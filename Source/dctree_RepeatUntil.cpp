/*
  ==============================================================================

    dctree_RepeatUntil.cpp
    Created: 13 Apr 2016 1:03:06pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_RepeatUntil.h"

namespace DCTree
{
	RepeatUntil::RepeatUntil(Node* child, Result finishResult): Decorator(child), _finishResult(finishResult)
	{
	}

	Result RepeatUntil::OnTick(double tickTime)
	{
		auto result = _child->Tick(tickTime);

		if (result == _finishResult)
		{
			return Result::Success;
		}

		return Result::Running;
	}
}
