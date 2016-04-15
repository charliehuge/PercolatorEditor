/*
  ==============================================================================

    dctree_SerializableNode.cpp
    Created: 13 Apr 2016 2:28:20pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_SerializableNode.h"

namespace DCTree
{
	const std::map<ConcreteNodeType, std::string> SerializableNode::_displayNames =
	{
		{ ConcreteNodeType::ROOT, "ROOT" },
		{ ConcreteNodeType::Charger, "Charge" },
		{ ConcreteNodeType::FiniteRepeater, "Repeat X" },
		{ ConcreteNodeType::Inverter, "Invert" },
		{ ConcreteNodeType::ModSequence, "Loop" },
		{ ConcreteNodeType::PlayNote, "Play Note" },
		{ ConcreteNodeType::Repeater, "Repeat Inf." },
		{ ConcreteNodeType::RepeatUntil, "Repeat Until" },
		{ ConcreteNodeType::Selector, "Select" },
		{ ConcreteNodeType::Sequence, "Sequence" },
		{ ConcreteNodeType::StopNote, "Stop Note" },
		{ ConcreteNodeType::Succeeder, "Succeed" },
		{ ConcreteNodeType::COUNT, "<COUNT>" },
		{ ConcreteNodeType::INVALID, "<INVALID>" }
	};

	const std::map<ConcreteNodeType, std::string> SerializableNode::_typeNames =
	{
		{ ConcreteNodeType::ROOT, "root" },
		{ ConcreteNodeType::Charger, "charger" },
		{ ConcreteNodeType::FiniteRepeater, "finiterepeater" },
		{ ConcreteNodeType::Inverter, "inverter" },
		{ ConcreteNodeType::ModSequence, "modsequence" },
		{ ConcreteNodeType::PlayNote, "playnote" },
		{ ConcreteNodeType::Repeater, "repeater" },
		{ ConcreteNodeType::RepeatUntil, "repeatuntil" },
		{ ConcreteNodeType::Selector, "selector" },
		{ ConcreteNodeType::Sequence, "sequence" },
		{ ConcreteNodeType::StopNote, "stopnote" },
		{ ConcreteNodeType::Succeeder, "succeeder" },
		{ ConcreteNodeType::COUNT, "<COUNT>" },
		{ ConcreteNodeType::INVALID, "<INVALID>" }
	};

	SerializableNode::SerializableNode(): NodeType(ConcreteNodeType::INVALID), MaxChildren(0), x(0), y(0)
	{
	}

	SerializableNode::SerializableNode(ConcreteNodeType nodeType) : NodeType(nodeType), MaxChildren(0), x(0), y(0)
	{
	}

	json SerializableNode::ToJson() const
	{
		json jsonObj;
		jsonObj[DCT_JSON_NODETYPE] = GetTypeName();

		// workaround for type conversion not working with std::vector
		jsonObj[DCT_JSON_CHILDREN] = json::array();
		for (size_t i = 0; i < ChildIndexes.size(); ++i)
		{
			jsonObj[DCT_JSON_CHILDREN] += ChildIndexes[i];
		}

		jsonObj[DCT_JSON_EDITORPOS] = { x, y };

		// and again with the workaround
		jsonObj[DCT_JSON_PARAMS] = json::object();
		for (size_t i = 0; i < Params.size(); ++i)
		{
			auto param = Params[i];

			switch (param.Type)
			{
			case NodeParamType::Int:
				jsonObj[DCT_JSON_PARAMS][param.Name] = param.IntValue;
				break;
			case NodeParamType::Double:
				jsonObj[DCT_JSON_PARAMS][param.Name] = param.DoubleValue;
				break;
			case NodeParamType::String:
				jsonObj[DCT_JSON_PARAMS][param.Name] = param.StringValue;
				break;
			default: break;
			}
		}

		return jsonObj;
	}

	void SerializableNode::FromJson(const json &jsonObject)
	{	
		// find the type
		{
			auto tName = jsonObject[DCT_JSON_NODETYPE].get<std::string>();
			for (auto it = _typeNames.begin(); it != _typeNames.end(); ++it)
			{
				if (it->second.compare(tName) == 0)
				{
					NodeType = it->first;
					break;
				}
			}
		}

		ChildIndexes = jsonObject[DCT_JSON_CHILDREN].get<std::vector<int>>();
		x = jsonObject[DCT_JSON_EDITORPOS][0].get<int>();
		y = jsonObject[DCT_JSON_EDITORPOS][1].get<int>();

		// get the params
		{
			auto paramsJs = jsonObject[DCT_JSON_PARAMS];

			for (auto it = paramsJs.begin(); it != paramsJs.end(); ++it)
			{
				SerializableNodeParam p;
				p.Name = it.key();

				if (it.value().is_number_integer())
				{
					p.Type = NodeParamType::Int;
					p.IntValue = it.value().get<int>();
				}
				else if (it.value().is_number_float())
				{
					p.Type = NodeParamType::Double;
					p.DoubleValue = it.value().get<double>();
				}
				else if (it.value().is_string())
				{
					p.Type = NodeParamType::String;
					p.StringValue = it.value().get<std::string>();
				}

				for (size_t pIdx = 0; pIdx < Params.size(); ++pIdx)
				{
					if (Params[pIdx].Name.compare(p.Name) == 0)
					{
						Params[pIdx].IntValue = p.IntValue;
						Params[pIdx].DoubleValue = p.DoubleValue;
						Params[pIdx].StringValue = p.StringValue;
						return;
					}
				}

				Params.push_back(p);
			}
		}
	}

	std::string SerializableNode::GetDisplayName() const
	{
		return _displayNames.at(NodeType);
	}

	std::string SerializableNode::GetTypeName() const
	{
		return _typeNames.at(NodeType);
	}

	std::string SerializableNode::GetDisplayName(ConcreteNodeType nodeType)
	{
		return _displayNames.at(nodeType);
	}

	ConcreteNodeType SerializableNode::GetNodeTypeFromName(const std::string& typeName)
	{
		// find the type
		{
			for (auto it = _typeNames.begin(); it != _typeNames.end(); ++it)
			{
				if (it->second.compare(typeName) == 0)
				{
					return it->first;
				}
			}
		}

		return ConcreteNodeType::INVALID;
	}
}
