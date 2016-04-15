/*
  ==============================================================================

    dctree_DCTree.cpp
    Created: 13 Apr 2016 5:46:33pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_DCTree.h"

// include concrete node types for the default serializable template functions
#include "dctree_Charger.h"
#include "dctree_FiniteRepeater.h"
#include "dctree_Inverter.h"
#include "dctree_ModSequence.h"
#include "dctree_PlayNote.h"
#include "dctree_Repeater.h"
#include "dctree_RepeatUntil.h"
#include "dctree_Selector.h"
#include "dctree_Sequence.h"
#include "dctree_StopNote.h"
#include "dctree_Succeeder.h"


namespace DCTree
{
	/*
	Runtime tree creation
	*/
	Node* CreateRuntimeTree(int index, const std::vector<SerializableNode>& sNodes, DCSynths::Instrument *instrument);
	Node* CreateRuntimeNodeWrapper(const SerializableNode& sNode, const std::vector<Node*> &children, DCSynths::Instrument *instrument);

	Node* CreateRuntimeTree(const std::string& jsonString, DCSynths::Instrument *instrument)
	{
		auto sNodes = DeserializeTree(jsonString);

		// find the root and make the tree from that
		for (size_t i = 0; i < sNodes.size(); ++i)
		{
			if (sNodes[i].NodeType == ConcreteNodeType::ROOT)
			{
				if (sNodes[i].ChildIndexes.size() < 1) return nullptr;
				auto rootChildIdx = sNodes[i].ChildIndexes[0];
				if (rootChildIdx < 0 || rootChildIdx >= sNodes.size()) return nullptr;
				return CreateRuntimeTree(rootChildIdx, sNodes, instrument);
			}
		}

		return nullptr;
	}

	Node* CreateRuntimeTree(int index, const std::vector<SerializableNode>& sNodes, DCSynths::Instrument *instrument)
	{
		auto sNode = sNodes[index];
		std::vector<Node *> children;

		for (size_t i = 0; i < sNode.ChildIndexes.size(); ++i)
		{
			children.push_back(CreateRuntimeTree(sNode.ChildIndexes[i], sNodes, instrument));
		}

		return CreateRuntimeNodeWrapper(sNode, children, instrument);
	}

	Node* CreateRuntimeNodeWrapper(const SerializableNode& sNode, const std::vector<Node*> &children, DCSynths::Instrument *instrument)
	{
		switch (sNode.NodeType)
		{
		case ConcreteNodeType::Charger: 
			return CreateRuntimeNode<Charger>(sNode.Params, children);
		case ConcreteNodeType::FiniteRepeater:
			return CreateRuntimeNode<FiniteRepeater>(sNode.Params, children);
		case ConcreteNodeType::Inverter:
			return CreateRuntimeNode<Inverter>(sNode.Params, children);
		case ConcreteNodeType::ModSequence:
			return CreateRuntimeNode<ModSequence>(sNode.Params, children);
		case ConcreteNodeType::PlayNote:
			return CreateRuntimeNode<PlayNote>(sNode.Params, children, instrument);
		case ConcreteNodeType::Repeater:
			return CreateRuntimeNode<Repeater>(sNode.Params, children);
		case ConcreteNodeType::RepeatUntil:
			return CreateRuntimeNode<RepeatUntil>(sNode.Params, children);
		case ConcreteNodeType::Selector:
			return CreateRuntimeNode<Selector>(sNode.Params, children);
		case ConcreteNodeType::Sequence:
			return CreateRuntimeNode<Sequence>(sNode.Params, children);
		case ConcreteNodeType::StopNote:
			return CreateRuntimeNode<StopNote>(sNode.Params, children, instrument);
		case ConcreteNodeType::Succeeder:
			return CreateRuntimeNode<Succeeder>(sNode.Params, children);
		case ConcreteNodeType::COUNT:
		case ConcreteNodeType::INVALID:
		case ConcreteNodeType::ROOT:
			break;
		}

		return nullptr;
	}

	std::string SerializeTree(const std::vector<SerializableNode>& sNodes, bool prettyPrint)
	{
		json jsonObj;

		for (size_t i = 0; i < sNodes.size(); ++i)
		{
			jsonObj.push_back(sNodes[i].ToJson());
		}

		return prettyPrint ? jsonObj.dump(4) : jsonObj.dump();
	}

	std::vector<SerializableNode> DeserializeTree(const std::string& jsonString)
	{
		auto jsonObj = json::parse(jsonString);
		std::vector<SerializableNode> sNodes;

		for (size_t i = 0; i < jsonObj.size(); ++i)
		{
			auto sNode = CreateDefaultSerializableNode(SerializableNode::GetNodeTypeFromName(jsonObj[i][DCT_JSON_NODETYPE].get<std::string>()));
			sNode.FromJson(jsonObj[i]);
			sNodes.push_back(sNode);
		}

		return sNodes;
	}

	SerializableNode CreateDefaultSerializableNode(ConcreteNodeType nodeType)
	{
		switch (nodeType)
		{
		case ConcreteNodeType::Charger:
			return GetDefaultNode<Charger>();
		case ConcreteNodeType::FiniteRepeater:
			return GetDefaultNode<FiniteRepeater>();
		case ConcreteNodeType::Inverter:
			return GetDefaultNode<Inverter>();
		case ConcreteNodeType::ModSequence:
			return GetDefaultNode<ModSequence>();
		case ConcreteNodeType::PlayNote:
			return GetDefaultNode<PlayNote>();
		case ConcreteNodeType::Repeater:
			return GetDefaultNode<Repeater>();
		case ConcreteNodeType::RepeatUntil:
			return GetDefaultNode<RepeatUntil>();
		case ConcreteNodeType::Selector:
			return GetDefaultNode<Selector>();
		case ConcreteNodeType::Sequence:
			return GetDefaultNode<Sequence>();
		case ConcreteNodeType::StopNote:
			return GetDefaultNode<StopNote>();
		case ConcreteNodeType::Succeeder:
			return GetDefaultNode<Succeeder>();
		case ConcreteNodeType::ROOT:
		{
			SerializableNode sNode(ConcreteNodeType::ROOT);
			sNode.MaxChildren = 1;
			return sNode;
		}
		case ConcreteNodeType::COUNT:
		case ConcreteNodeType::INVALID:
		default:
			return SerializableNode(ConcreteNodeType::INVALID);
		}
	}
}
