/*
  ==============================================================================

    dctree_Selector.cpp
    Created: 13 Apr 2016 1:03:54pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_Selector.h"

namespace DCTree
{
	Selector::Selector(std::vector<Node*> children): Composite(children), _selectedChildIdx(-1)
	{
	}

	Result Selector::OnStart()
	{
		_selectedChildIdx = -1;
		return Result::Success;
	}

	Result Selector::OnTick(double tickTime)
	{
		if (_selectedChildIdx < 0)
		{
			for (size_t i = 0; i < _children.size(); ++i)
			{
				auto result = _children[i]->Tick(tickTime);

				if (result == Result::Failure)
				{
					continue;
				}

				_selectedChildIdx = i;
				return result;
			}

			return Result::Failure;
		}
		
		return _children[_selectedChildIdx]->Tick(tickTime);
	}
}
