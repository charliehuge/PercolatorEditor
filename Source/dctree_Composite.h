/*
  ==============================================================================

    dctree_Composite.h
    Created: 8 Apr 2016 7:39:46pm
    Author:  Charlie Huguenard

  ==============================================================================
*/

#ifndef DCTREE_COMPOSITE_H_INCLUDED
#define DCTREE_COMPOSITE_H_INCLUDED

#include "dctree_Node.h"
#include <vector>

#define DCT_COMPOSITE_MAXCHILDREN 8

namespace DCTree
{
	class Composite : public Node
	{
	public:
		Composite(std::vector<Node *> children);

		~Composite();

	protected:
		std::vector<Node *> _children;
	};
}

#endif  // DCTREE_COMPOSITE_H_INCLUDED
