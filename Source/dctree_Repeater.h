/*
  ==============================================================================

    dctree_Repeater.h
    Created: 8 Apr 2016 7:40:44pm
    Author:  Charlie Huguenard

  ==============================================================================
*/

#ifndef DCTREE_REPEATER_H_INCLUDED
#define DCTREE_REPEATER_H_INCLUDED

#include "dctree_Decorator.h"

namespace DCTree
{
	class Repeater : public Decorator
	{
	public:
		explicit Repeater(Node *child);

	protected:
		Result OnTick(double tickTime) override;
	};

	template<>
	inline SerializableNode GetDefaultNode<Repeater>()
	{
		SerializableNode sNode(ConcreteNodeType::Repeater);

		sNode.MaxChildren = 0;

		return sNode;
	}
}

#endif  // DCTREE_REPEATER_H_INCLUDED
