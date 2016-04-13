/*
  ==============================================================================

    dctree_Serialization.cpp
    Created: 11 Apr 2016 4:28:55pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_Serialization.h"
#include "json.hpp"

using json = nlohmann::json;

#define DCT_JSON_NODETYPE "node_type"
#define DCT_JSON_PARAMS "params"
#define DCT_JSON_CHILDREN "children"
#define DCT_JSON_EDITORPOS "editor_pos"

namespace DCTree
{
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

	std::string GetNodeDisplayName(ConcreteNodeType nodeType)
	{
		auto idx = static_cast<int>(nodeType);

		if (idx < 0 || idx >= static_cast<int>(ConcreteNodeType::COUNT))
		{
			return "ERROR";
		}

		return NodeDisplayNames[static_cast<int>(nodeType)];
	}

	std::string GetNodeTypeName(ConcreteNodeType nodeType)
	{
		auto idx = static_cast<int>(nodeType);

		if (idx < 0 || idx >= static_cast<int>(ConcreteNodeType::COUNT))
		{
			return "ERROR";
		}

		return NodeTypeNames[static_cast<int>(nodeType)];
	}

	ConcreteNodeType GetNodeType(std::string name)
	{
		for (int i = 0; i < static_cast<int>(ConcreteNodeType::COUNT); ++i)
		{
			if (name.compare(NodeTypeNames[i]) == 0)
			{
				return static_cast<ConcreteNodeType>(i);
			}
		}

		return ConcreteNodeType::INVALID;
	}

	/*
	Create a list of SerializableNode from a JSON string
	*/
	SerializableNode CreateSerializableNode(const json &jsonObject);

	std::vector<SerializableNode> CreateSerializableNodes(const std::string& jsonString)
	{
		auto jsonObj = json::parse(jsonString);
		std::vector<SerializableNode> sNodes;

		for (size_t i = 0; i < jsonObj.size(); ++i)
		{
			sNodes.push_back(CreateSerializableNode(jsonObj[i]));
		}

		return sNodes;
	}

	SerializableNode CreateSerializableNode(const json &jsonObject)
	{
		SerializableNode sNode;
		sNode.NodeType = GetNodeType(jsonObject[DCT_JSON_NODETYPE].get<std::string>());
		sNode.ChildIndexes = jsonObject[DCT_JSON_CHILDREN].get<std::vector<int>>();
		sNode.x = jsonObject[DCT_JSON_EDITORPOS][0].get<int>();
		sNode.y = jsonObject[DCT_JSON_EDITORPOS][1].get<int>();

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

			sNode.Params.push_back(p);
		}

		return sNode;
	}

	/*
	Create a JSON string from a list of SerializableNode
	*/
	json CreateJsonObject(const SerializableNode &sNode);

	std::string CreateJson(const std::vector<SerializableNode>& sNodes, bool prettyPrint)
	{
		json jsonObj;

		for (size_t i = 0; i < sNodes.size(); ++i)
		{
			jsonObj.push_back(CreateJsonObject(sNodes[i]));
		}

		return prettyPrint ? jsonObj.dump(4) : jsonObj.dump();
	}

	json CreateJsonObject(const SerializableNode &sNode)
	{
		json jsonObj;
		jsonObj[DCT_JSON_NODETYPE] = GetNodeTypeName(sNode.NodeType);
		
		// workaround for type conversion not working with std::vector
		jsonObj[DCT_JSON_CHILDREN] = json::array();
		for (size_t i = 0; i < sNode.ChildIndexes.size(); ++i)
		{
			jsonObj[DCT_JSON_CHILDREN] += sNode.ChildIndexes[i];
		}

		jsonObj[DCT_JSON_EDITORPOS] = { sNode.x, sNode.y };

		// and again with the workaround
		jsonObj[DCT_JSON_PARAMS] = json::object();
		for (size_t i = 0; i < sNode.Params.size(); ++i)
		{
			auto param = sNode.Params[i];

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

	/*
	Create a runtime tree from JSON
	*/
	Node* CreateRuntimeTree(int index, const std::vector<SerializableNode>& sNodes);
	Node* CreateRuntimeNode(const SerializableNode& sNode, std::vector<Node*> children);

	Node* CreateRuntimeTree(const std::string& jsonString)
	{
		auto sNodes = CreateSerializableNodes(jsonString);

		// find the root and make the tree from that
		for (size_t i = 0; i < sNodes.size(); ++i)
		{
			if (sNodes[i].NodeType == ConcreteNodeType::ROOT)
			{
				if (sNodes[i].ChildIndexes.size() < 1) return nullptr;
				auto rootChildIdx = sNodes[i].ChildIndexes[0];
				if (rootChildIdx < 0 || rootChildIdx >= sNodes.size()) return nullptr;
				return CreateRuntimeTree(rootChildIdx, sNodes);
			}
		}

		return nullptr;
	}

	Node* CreateRuntimeTree(int index, const std::vector<SerializableNode>& sNodes)
	{
		auto sNode = sNodes[index];
		std::vector<Node *> children;

		for (size_t i = 0; i < sNode.ChildIndexes.size(); ++i)
		{
			children.push_back(CreateRuntimeTree(sNode.ChildIndexes[i], sNodes));
		}

		return CreateRuntimeNode(sNode, children);
	}

	Node* CreateRuntimeNode(const SerializableNode& sNode, std::vector<Node*> children)
	{
		switch (sNode.NodeType)
		{
		case ConcreteNodeType::Repeater:
		{
			if (children.size() > 0)
				return new Repeater(children[0]);

			return nullptr;
		}
		case ConcreteNodeType::FiniteRepeater:
		{
			if (children.size() > 0)
			{
				auto numRepeats = 1;

				for (size_t i = 0; i < sNode.Params.size(); ++i)
				{
					if (sNode.Params[i].Name.compare("NumRepeats") == 0)
					{
						numRepeats = sNode.Params[i].IntValue;
					}
				}

				return new FiniteRepeater(children[0], numRepeats);
			}

			return nullptr;
		}
		case ConcreteNodeType::Sequence: break;
		case ConcreteNodeType::Selector: break;
		case ConcreteNodeType::PlayNote: break;
		case ConcreteNodeType::StopNote: break;
		}

		return nullptr;
	}

}
