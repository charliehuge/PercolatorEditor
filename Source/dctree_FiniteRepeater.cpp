/*
  ==============================================================================

    dctree_FiniteRepeater.cpp
    Created: 8 Apr 2016 7:40:56pm
    Author:  Charlie Huguenard

  ==============================================================================
*/

#include "dctree_FiniteRepeater.h"

namespace DCTree
{
	FiniteRepeater::FiniteRepeater(Node* child, int numRepeats) : Decorator(child), _numRepeats(numRepeats), _repeatCount(0)
	{
	}

	Result FiniteRepeater::OnStart()
	{
		_repeatCount = 0;
		return Result::Success;
	}

	Result FiniteRepeater::OnTick(double tickTime)
	{
		auto result = _child->Tick(tickTime);

		if (result != Result::Running && ++_repeatCount >= _numRepeats)
		{
			return result;
		}
		
		return Result::Running;
	}
}
