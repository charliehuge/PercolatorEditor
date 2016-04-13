/*
  ==============================================================================

    dctree_Decorator.h
    Created: 8 Apr 2016 7:40:02pm
    Author:  Charlie Huguenard

  ==============================================================================
*/

#ifndef DCTREE_DECORATOR_H_INCLUDED
#define DCTREE_DECORATOR_H_INCLUDED

#include "dctree_Node.h"

namespace DCTree
{
	class Decorator : public Node
	{
	public:
		explicit Decorator(Node *child);

		~Decorator();
		
	protected:
		Node *_child;
	};
}

#endif  // DCTREE_DECORATOR_H_INCLUDED
