/*
  ==============================================================================

    dctree_Succeeder.h
    Created: 13 Apr 2016 1:03:32pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_SUCCEEDER_H_INCLUDED
#define DCTREE_SUCCEEDER_H_INCLUDED

#include "dctree_Decorator.h"

namespace DCTree
{
	class Succeeder : public Decorator
	{
	public:
		explicit Succeeder(Node *child);

	protected:
		Result OnTick(double tickTime) override;
	};

	template<>
	inline SerializableNode GetDefaultNode<Succeeder>()
	{
		SerializableNode sNode(ConcreteNodeType::Succeeder);
		sNode.MaxChildren = 1;
		return sNode;
	}

	template<>
	inline Node *CreateRuntimeNode<Succeeder>(const std::vector<SerializableNodeParam> &/*params*/, const std::vector<Node *> &children)
	{
		if (children.size() > 0) 
			return new Succeeder(children[0]); 
		else 
			return nullptr;
	}
}

#endif  // DCTREE_SUCCEEDER_H_INCLUDED
