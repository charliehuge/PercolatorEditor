/*
  ==============================================================================

    dctree_PlayNote.h
    Created: 13 Apr 2016 1:02:40pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_PLAYNOTE_H_INCLUDED
#define DCTREE_PLAYNOTE_H_INCLUDED

#include "dctree_Node.h"

namespace DCTree
{
	class PlayNote : public Node
	{
	public:
		explicit PlayNote(int note);

	protected:
		Result OnTick(double tickTime) override;

		int _note;
	};

	template<>
	inline SerializableNode GetDefaultNode<PlayNote>()
	{
		SerializableNode sNode(ConcreteNodeType::PlayNote);

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

}

#endif  // DCTREE_PLAYNOTE_H_INCLUDED
