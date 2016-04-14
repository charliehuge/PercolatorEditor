/*
  ==============================================================================

    dctree_Selector.h
    Created: 13 Apr 2016 1:03:54pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_SELECTOR_H_INCLUDED
#define DCTREE_SELECTOR_H_INCLUDED

#include "dctree_Composite.h"

namespace DCTree
{
	class Selector : public Composite
	{
	public:
		explicit Selector(std::vector<Node *> children);

	protected:
		Result OnStart() override;
		Result OnTick(double tickTime) override;

		int _selectedChildIdx;
	};

	template<>
	inline SerializableNode GetDefaultNode<Selector>()
	{
		SerializableNode sNode(ConcreteNodeType::Selector);

		sNode.MaxChildren = DCT_COMPOSITE_MAXCHILDREN;

		return sNode;
	}

}

#endif  // DCTREE_SELECTOR_H_INCLUDED
