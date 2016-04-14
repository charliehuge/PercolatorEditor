/*
  ==============================================================================

    dctree_ModSequence.h
    Created: 13 Apr 2016 1:02:06pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_MODSEQUENCE_H_INCLUDED
#define DCTREE_MODSEQUENCE_H_INCLUDED

#include  "dctree_Composite.h"

namespace DCTree
{
	class ModSequence : public Composite
	{
	public:
		ModSequence(std::vector<Node *> children, int modTicks);

	protected:
		Result OnStart() override;
		Result OnTick(double tickTime) override;

		int _modTicks;
		int _currentTick;
	};

	template<>
	inline SerializableNode GetDefaultNode<ModSequence>()
	{
		SerializableNode sNode(ConcreteNodeType::ModSequence);

		sNode.MaxChildren = DCT_COMPOSITE_MAXCHILDREN;

		SerializableNodeParam p;
		p.Name = "ticks";
		p.Type = NodeParamType::Int;
		p.HasRange = true;
		p.IntMin = 1;
		p.IntMax = 16;
		p.IntValue = 1;
		sNode.Params.push_back(p);

		return sNode;
	}

}

#endif  // DCTREE_MODSEQUENCE_H_INCLUDED
