/*
  ==============================================================================

    dctree_ModSequence.cpp
    Created: 13 Apr 2016 1:02:06pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_ModSequence.h"

namespace DCTree
{
	ModSequence::ModSequence(std::vector<Node*> children, int modTicks) : Composite(children), _modTicks(modTicks), _currentTick(0)
	{
	}

	Result ModSequence::OnStart()
	{
		_currentTick = 0;
		return Result::Success;
	}

	Result ModSequence::OnTick(double tickTime)
	{
		if (_currentTick < _children.size())
		{
			auto result = _children[_currentTick]->Tick(tickTime);

			switch (result)
			{
			case Result::Success:
				break;
			case Result::Failure:
				return Result::Failure;
			case Result::Running:
				return Result::Running;
			}
		}

		_currentTick = (_currentTick + 1) % _modTicks;

		return Result::Running;
	}
}
