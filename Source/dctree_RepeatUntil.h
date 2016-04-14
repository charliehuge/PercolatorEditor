/*
  ==============================================================================

    dctree_RepeatUntil.h
    Created: 13 Apr 2016 1:03:06pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_REPEATUNTIL_H_INCLUDED
#define DCTREE_REPEATUNTIL_H_INCLUDED

#include "dctree_Decorator.h"

namespace DCTree
{
	class RepeatUntil : public Decorator
	{
	public:
		RepeatUntil(Node *child, Result finishResult);

	protected:
		Result OnTick(double tickTime) override;

		Result _finishResult;
	};

	template<>
	inline SerializableNode GetDefaultNode<RepeatUntil>()
	{
		SerializableNode sNode(ConcreteNodeType::RepeatUntil);

		sNode.MaxChildren = 1;

		SerializableNodeParam p;
		p.Name = "result";
		p.Type = NodeParamType::Result;
		p.HasRange = true;
		p.IntMin = 0;
		p.IntMax = 2;
		p.IntValue = 0;
		sNode.Params.push_back(p);

		return sNode;
	}

}

#endif  // DCTREE_REPEATUNTIL_H_INCLUDED
