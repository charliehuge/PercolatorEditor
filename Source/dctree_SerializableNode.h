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
		Charger = 0,
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
		INVALID,
		ROOT,
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
		SerializableNode();
		explicit SerializableNode(ConcreteNodeType nodeType);

		ConcreteNodeType NodeType;
		std::vector<int> ChildIndexes;
		int MaxChildren;
		std::vector<SerializableNodeParam> Params;
		int x, y;

		json ToJson() const;
		void FromJson(const json &jsonString);
		std::string GetDisplayName() const;
		std::string GetTypeName() const;

		static std::string GetDisplayName(ConcreteNodeType nodeType);

	private:
		const static std::map<ConcreteNodeType, std::string> _displayNames;
		const static std::map<ConcreteNodeType, std::string> _typeNames;
	};

}

#endif  // DCTREE_SERIALIZABLENODE_H_INCLUDED
