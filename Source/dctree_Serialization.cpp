/*
  ==============================================================================

    dctree_Serialization.cpp
    Created: 11 Apr 2016 4:28:55pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_Serialization.h"
#include "dctree_Include.h"

namespace DCTree
{
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

	ConcreteNodeType GetNodeTypeFromName(std::string name)
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

	Node* CreateRuntimeTree(json jsonObject)
	{
		std::vector<SerializableNode> sNodes;

		for (size_t i = 0; i < jsonObject.size(); ++i)
		{
			sNodes.push_back(CreateSerializableNodeFromJson(jsonObject[i]));
		}

		// find the root and make the tree from that
		for (size_t i = 0; i < sNodes.size(); ++i)
		{
			if (sNodes[i].NodeType == ConcreteNodeType::ROOT)
			{
				if (sNodes[i].ChildIndexes.size() < 1)
				{
					return nullptr;
				}

				auto rootChildIdx = sNodes[i].ChildIndexes[0];

				if (rootChildIdx < 0 || rootChildIdx >= sNodes.size())
				{
					return nullptr;
				}

				return CreateRuntimeTree(rootChildIdx, sNodes);
			}
		}

		return nullptr;
	}

	SerializableNode CreateSerializableNodeFromJson(json jsonObject)
	{
		SerializableNode sNode;
		sNode.NodeType = GetNodeTypeFromName(jsonObject[DCT_JSON_NODETYPE].get<std::string>());
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

	Node* CreateRuntimeTree(int index, std::vector<SerializableNode> sNodes)
	{
		auto sNode = sNodes[index];
		std::vector<Node *> children;
		
		for (size_t i = 0; i < sNode.ChildIndexes.size(); ++i)
		{
			children.push_back(CreateRuntimeTree(sNode.ChildIndexes[i], sNodes));
		}

		return CreateRuntimeNode(sNode, children);
	}

	Node* CreateRuntimeNode(SerializableNode sNode, std::vector<Node*> children)
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
		default: break;
		}

		return nullptr;
	}
}
