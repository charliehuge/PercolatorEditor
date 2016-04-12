/*
  ==============================================================================

    dctree_Serialization.h
    Created: 11 Apr 2016 4:28:55pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_SERIALIZATION_H_INCLUDED
#define DCTREE_SERIALIZATION_H_INCLUDED

#include <string>
#include "json.hpp"

#define DCT_JSON_NODETYPE "node_type"
#define DCT_JSON_PARAMS "params"
#define DCT_JSON_CHILDREN "children"
#define DCT_JSON_EDITORPOS "editor_pos"

namespace DCTree
{
	enum class ConcreteNodeType
	{
		ROOT = 0,
		Repeater,
		FiniteRepeater,
		Sequence,
		Selector,
		PlayNote,
		StopNote,
		COUNT,
		INVALID
	};

	const std::string NodeTypeNames[] = {
		"root",
		"repeater",
		"finiterepeater",
		"sequence",
		"selector",
		"playnote",
		"stopnote",
		"<undefined>"
	};

	const std::string NodeDisplayNames[] = {
		"Root",
		"Repeat",
		"Repeat X",
		"Sequence",
		"Select",
		"Play Note",
		"Stop Note",
		"<undefined>"
	};

	std::string GetNodeDisplayName(ConcreteNodeType nodeType);

	std::string GetNodeTypeName(ConcreteNodeType nodeType);

	ConcreteNodeType GetNodeTypeFromName(std::string name);
}

#endif  // DCTREE_SERIALIZATION_H_INCLUDED
