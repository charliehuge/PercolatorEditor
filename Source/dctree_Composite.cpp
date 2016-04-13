/*
  ==============================================================================

    dctree_Composite.cpp
    Created: 8 Apr 2016 7:39:46pm
    Author:  Charlie Huguenard

  ==============================================================================
*/

#include "dctree_Composite.h"

namespace DCTree
{
	Composite::Composite(std::vector<Node*> children) : _children(children)
	{
	}

	Composite::~Composite()
	{
		while (!_children.empty())
		{
			delete _children.back();
			_children.pop_back();
		}
	}
}
