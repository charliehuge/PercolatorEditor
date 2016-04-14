/*
  ==============================================================================

    dctree_Sequence.cpp
    Created: 13 Apr 2016 1:03:46pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_Sequence.h"

namespace DCTree
{
	Sequence::Sequence(std::vector<Node*> children): Composite(children), _currentChildIdx(0)
	{
	}

	Result Sequence::OnStart()
	{
		_currentChildIdx = 0;
		return Result::Success;
	}

	Result Sequence::OnTick(double tickTime)
	{
		auto result = _children[_currentChildIdx]->Tick(tickTime);

		if (result != Result::Success)
		{
			return result;
		}

		if (++_currentChildIdx < _children.size())
		{
			return Result::Running;
		}

		return Result::Success;
	}
}
