/*
  ==============================================================================

    dctree_Sequence.h
    Created: 13 Apr 2016 1:03:46pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_SEQUENCE_H_INCLUDED
#define DCTREE_SEQUENCE_H_INCLUDED

#include "dctree_Composite.h"

namespace DCTree
{
	class Sequence : public Composite
	{
	public:
		explicit Sequence(std::vector<Node *> children);

	protected:
		Result OnStart() override;
		Result OnTick(double tickTime) override;

		int _currentChildIdx;
	};

	template<>
	inline SerializableNode GetDefaultNode<Sequence>()
	{
		SerializableNode sNode(ConcreteNodeType::Sequence);

		sNode.MaxChildren = DCT_COMPOSITE_MAXCHILDREN;

		return sNode;
	}

	template<>
	inline Node *CreateRuntimeNode<Sequence>(const std::vector<SerializableNodeParam> &/*params*/, const std::vector<Node *> &children)
	{
		return new Sequence(children);
	}
}

#endif  // DCTREE_SEQUENCE_H_INCLUDED
