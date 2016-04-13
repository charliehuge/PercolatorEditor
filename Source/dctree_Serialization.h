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
#include <vector>
#include "json.hpp"
#include "dctree_Node.h"

using json = nlohmann::json;

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

	enum class NodeParamType { Int, Double, String };

	struct SerializableNodeParam
	{
		std::string Name;
		int IntValue;
		double DoubleValue;
		std::string StringValue;
		NodeParamType Type;
	};

	struct SerializableNode
	{
		ConcreteNodeType NodeType;
		std::vector<int> ChildIndexes;
		std::vector<SerializableNodeParam> Params;
		int x, y;
	};

	std::string GetNodeDisplayName(ConcreteNodeType nodeType);

	std::string GetNodeTypeName(ConcreteNodeType nodeType);

	ConcreteNodeType GetNodeTypeFromName(std::string name);

	Node *CreateRuntimeTree(json jsonObject);

	SerializableNode CreateSerializableNodeFromJson(json jsonObject);

	Node *CreateRuntimeTree(int index, std::vector<SerializableNode> sNodes);

	Node *CreateRuntimeNode(SerializableNode sNode, std::vector<Node *> children);

	json CreateJsonForTree(std::vector<SerializableNode> sNodes);

	json CreateJsonFromSerializableNode(SerializableNode sNode);
}

#endif  // DCTREE_SERIALIZATION_H_INCLUDED
