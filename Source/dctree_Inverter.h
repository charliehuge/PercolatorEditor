/*
  ==============================================================================

    dctree_Inverter.h
    Created: 13 Apr 2016 1:01:56pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_INVERTER_H_INCLUDED
#define DCTREE_INVERTER_H_INCLUDED

#include "dctree_Decorator.h"

namespace DCTree
{
	class Inverter : public Decorator
	{
	public:
		Inverter(Node *child);

	protected:
		Result OnTick(double tickTime) override;
	};

	template<>
	inline SerializableNode GetDefaultNode<Inverter>()
	{
		SerializableNode sNode(ConcreteNodeType::Inverter);

		sNode.MaxChildren = 1;

		return sNode;
	}
}

#endif  // DCTREE_INVERTER_H_INCLUDED
