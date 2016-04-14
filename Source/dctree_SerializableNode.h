/*
  ==============================================================================

    dctree_SerializableNode.h
    Created: 13 Apr 2016 2:28:20pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_SERIALIZABLENODE_H_INCLUDED
#define DCTREE_SERIALIZABLENODE_H_INCLUDED

#include <vector>
#include <map>
#include "json.hpp"

using json = nlohmann::json;

namespace DCTree
{
	enum class ConcreteNodeType
	{
		ROOT = 0,
		Charger,
		FiniteRepeater,
		Inverter,
		ModSequence,
		PlayNote,
		Repeater,
		RepeatUntil,
		Selector,
		Sequence,
		StopNote,
		Succeeder,
		COUNT,
		INVALID
	};

	enum class NodeParamType { Int, Double, String, Result };

	struct SerializableNodeParam
	{
		std::string Name;
		NodeParamType Type;
		bool HasRange;
		int IntValue;
		int IntMin;
		int IntMax;
		double DoubleValue;
		double DoubleMin;
		double DoubleMax;
		std::string StringValue;
	};

	class SerializableNode
	{
	public:
		explicit SerializableNode(ConcreteNodeType nodeType);

		ConcreteNodeType NodeType;
		std::vector<int> ChildIndexes;
		int MaxChildren;
		std::vector<SerializableNodeParam> Params;
		int x, y;

		static std::string Serialize(const std::vector<SerializableNode> &sNodes, bool prettyPrint = true);
		static std::vector<SerializableNode> Deserialize(const std::string &jsonString);

		std::string GetDisplayName() const;
		std::string GetTypeName() const;

	private:
		const static std::map<ConcreteNodeType, std::string> _displayNames;
		const static std::map<ConcreteNodeType, std::string> _typeNames;

		json ToJson() const;
		void FromJson(const json &jsonString);
	};

}

#endif  // DCTREE_SERIALIZABLENODE_H_INCLUDED
