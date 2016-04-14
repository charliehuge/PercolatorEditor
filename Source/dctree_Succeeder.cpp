/*
  ==============================================================================

    dctree_Succeeder.cpp
    Created: 13 Apr 2016 1:03:32pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_Succeeder.h"

namespace DCTree
{
	Succeeder::Succeeder(Node* child): Decorator(child)
	{
	}

	Result Succeeder::OnTick(double tickTime)
	{
		auto result = _child->Tick(tickTime);

		switch (result)
		{
		case Result::Success:
		case Result::Failure:
			return Result::Success;
		default: 
			return result;
		}
	}
}
