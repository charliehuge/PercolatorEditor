/*
  ==============================================================================

    dctree_Charger.cpp
    Created: 13 Apr 2016 1:01:40pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_Charger.h"

namespace DCTree
{
	Charger::Charger(Node* child, int numCharges) : Decorator(child), _numCharges(numCharges), _chargeCount(0)
	{
	}

	Result Charger::OnStart()
	{
		_chargeCount = 0;
		return Result::Success;
	}

	Result Charger::OnTick(double tickTime)
	{
		if (++_chargeCount >= _numCharges)
		{
			return _child->Tick(tickTime);
		}

		return Result::Running;
	}
}
