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
}

#endif  // DCTREE_SUCCEEDER_H_INCLUDED
