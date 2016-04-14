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

		sNode.MaxChildren = 1;

		return sNode;
	}

	template<>
	inline Node *CreateRuntimeNode<Repeater>(const std::vector<SerializableNodeParam> &/*params*/, const std::vector<Node *> &children)
	{
		if (children.size() > 0) 
			return new Repeater(children[0]); 
		else 
			return nullptr;
	}
}

#endif  // DCTREE_REPEATER_H_INCLUDED
