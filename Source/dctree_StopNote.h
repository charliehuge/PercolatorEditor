/*
  ==============================================================================

    dctree_StopNote.h
    Created: 13 Apr 2016 1:02:52pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_STOPNOTE_H_INCLUDED
#define DCTREE_STOPNOTE_H_INCLUDED

#include  "dctree_Node.h"

namespace DCTree
{
	class StopNote : public Node
	{
	public:
		explicit StopNote(int note, DCSynths::Instrument *instrument);

	protected:
		Result OnTick(double tickTime) override;

		int _note;
		DCSynths::Instrument *_instrument;
	};

	template<>
	inline SerializableNode GetDefaultNode<StopNote>()
	{
		SerializableNode sNode(ConcreteNodeType::StopNote);
		
		sNode.MaxChildren = 0;
		
		SerializableNodeParam p;
		p.Name = "note";
		p.Type = NodeParamType::Int;
		p.HasRange = true;
		p.IntMin = 0;
		p.IntMax = 127;
		p.IntValue = 60;
		sNode.Params.push_back(p);

		return sNode;
	}

	template<>
	inline Node *CreateRuntimeNode<StopNote>(const std::vector<SerializableNodeParam> &params, const std::vector<Node *> &/*children*/, DCSynths::Instrument *instrument)
	{
		if (params.size() > 0) 
			return new StopNote(params[0].IntValue, instrument); 
		else 
			return nullptr;
	}
}

#endif  // DCTREE_STOPNOTE_H_INCLUDED
