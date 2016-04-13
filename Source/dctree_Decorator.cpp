/*
  ==============================================================================

    dctree_Decorator.cpp
    Created: 8 Apr 2016 7:40:02pm
    Author:  Charlie Huguenard

  ==============================================================================
*/

#include "dctree_Decorator.h"

namespace DCTree
{
	Decorator::Decorator(Node *child) : _child(child)
	{
	}

	Decorator::~Decorator()
	{
		if (_child)
		{
			delete _child;
		}
	}
}
