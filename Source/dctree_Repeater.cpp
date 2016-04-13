/*
  ==============================================================================

    dctree_Repeater.cpp
    Created: 8 Apr 2016 7:40:44pm
    Author:  Charlie Huguenard

  ==============================================================================
*/

#include "dctree_Repeater.h"

namespace DCTree
{
	Repeater::Repeater(Node* child) : Decorator(child)
	{
	}

	Result Repeater::OnTick(double tickTime)
	{
		_child->Tick(tickTime);
		return Result::Running;
	}
}
