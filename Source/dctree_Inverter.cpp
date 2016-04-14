/*
  ==============================================================================

    dctree_Inverter.cpp
    Created: 13 Apr 2016 1:01:56pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_Inverter.h"

namespace DCTree
{
	Inverter::Inverter(Node* child) : Decorator(child)
	{
	}

	Result Inverter::OnTick(double tickTime)
	{
		auto result = _child->Tick(tickTime);

		switch (result)
		{
		case Result::Success:
			return Result::Failure;
		case Result::Failure:
			return Result::Success;
		default:
			return result;
		}
	}
}
