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
	Node* CreateRuntimeTree(int index, const std::vector<SerializableNode>& sNodes);
	Node* CreateRuntimeNode(const SerializableNode& sNode, std::vector<Node*> children);

	Node* CreateRuntimeTree(const std::string& jsonString)
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
		case ConcreteNodeType::PlayNote:
		{
			auto note = 0;

			for (size_t i = 0; i < sNode.Params.size(); ++i)
			{
				if (sNode.Params[i].Name.compare("Note") == 0)
				{
					note = sNode.Params[i].IntValue;
				}
			}

			return new PlayNote(note);
		}

		case ConcreteNodeType::StopNote: break;
		case ConcreteNodeType::Charger: break;
		case ConcreteNodeType::Inverter: break;
		case ConcreteNodeType::ModSequence:
		{
			auto modTicks = 0;

			for (size_t i = 0; i < sNode.Params.size(); ++i)
			{
				if (sNode.Params[i].Name.compare("ModTicks") == 0)
				{

				}
			}
		}
		case ConcreteNodeType::RepeatUntil: break;
		case ConcreteNodeType::Succeeder: break;
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
			SerializableNode sNode(ConcreteNodeType::INVALID);
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
		case ConcreteNodeType::COUNT:
		case ConcreteNodeType::INVALID:
		default:
			return SerializableNode(ConcreteNodeType::INVALID);
		}
	}
}
