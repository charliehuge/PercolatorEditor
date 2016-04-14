/*
  ==============================================================================

    dctree_Charger.h
    Created: 13 Apr 2016 1:01:40pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_CHARGER_H_INCLUDED
#define DCTREE_CHARGER_H_INCLUDED

#include "dctree_Decorator.h"

namespace DCTree
{
	class Charger : public Decorator
	{
	public:
		Charger(Node *child, int numCharges);

	protected:
		Result OnStart() override;
		Result OnTick(double tickTime) override;

		int _numCharges;
		int _chargeCount;
	};

	template<>
	inline SerializableNode GetDefaultNode<Charger>()
	{
		SerializableNode sNode(ConcreteNodeType::Charger);

		sNode.MaxChildren = 1;

		SerializableNodeParam p;
		p.Name = "charges";
		p.Type = NodeParamType::Int;
		p.HasRange = true;
		p.IntMin = 1;
		p.IntMax = 32;
		p.IntValue = 2;
		sNode.Params.push_back(p);

		return sNode;
	}

	template<>
	inline Node *CreateRuntimeNode<Charger>(const std::vector<SerializableNodeParam> &params, const std::vector<Node *> &children)
	{
		if (params.size() > 0 && children.size() > 0)
		{
			return new Charger(children[0], params[0].IntValue);
		}

		return nullptr;
	}
}

#endif  // DCTREE_CHARGER_H_INCLUDED
