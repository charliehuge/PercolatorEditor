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
#include "dctree_Include.h"

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

	ConcreteNodeType GetNodeType(std::string name);

	std::vector<SerializableNode> CreateSerializableNodes(const std::string &jsonString);

	std::string CreateJson(const std::vector<SerializableNode> &sNodes, bool prettyPrint = false);
	
	Node *CreateRuntimeTree(const std::string &jsonString);
}

#endif  // DCTREE_SERIALIZATION_H_INCLUDED
