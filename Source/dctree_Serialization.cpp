/*
  ==============================================================================

    dctree_Serialization.cpp
    Created: 11 Apr 2016 4:28:55pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_Serialization.h"
#include "json.hpp"
#include "dctree_Include.h"

using json = nlohmann::json;


namespace DCTree
{





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

}
