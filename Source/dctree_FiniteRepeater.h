/*
  ==============================================================================

    dctree_FiniteRepeater.h
    Created: 8 Apr 2016 7:40:56pm
    Author:  Charlie Huguenard

  ==============================================================================
*/

#ifndef DCTREE_FINITEREPEATER_H_INCLUDED
#define DCTREE_FINITEREPEATER_H_INCLUDED

#include "dctree_Decorator.h"

namespace DCTree
{
	class FiniteRepeater : public Decorator
	{
	public:
		explicit FiniteRepeater(Node *child, int numRepeats);

	protected:
		Result OnStart() override;
		Result OnTick(double tickTime) override;

		int _numRepeats;
		int _repeatCount;
	};

	template<>
	inline SerializableNode GetDefaultNode<FiniteRepeater>()
	{
		SerializableNode sNode(ConcreteNodeType::FiniteRepeater);

		sNode.MaxChildren = 1;

		SerializableNodeParam p;
		p.Name = "repeats";
		p.Type = NodeParamType::Int;
		p.HasRange = true;
		p.IntMin = 1;
		p.IntMax = 32;
		p.IntValue = 2;
		sNode.Params.push_back(p);

		return sNode;
	}

	template<>
	inline Node *CreateRuntimeNode<FiniteRepeater>(const std::vector<SerializableNodeParam> &params, const std::vector<Node *> &children)
	{
		if (params.size() > 0 && children.size() > 0)
		{
			return new FiniteRepeater(children[0], params[0].IntValue);
		}

		return nullptr;
	}
}

#endif  // DCTREE_FINITEREPEATER_H_INCLUDED
