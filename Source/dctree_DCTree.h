/*
  ==============================================================================

    dctree_DCTree.h
    Created: 13 Apr 2016 5:46:33pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_DCTREE_H_INCLUDED
#define DCTREE_DCTREE_H_INCLUDED

#include <string>
#include "dctree_Node.h"
#include "Instrument.h"

namespace DCTree
{
	Node *CreateRuntimeTree(const std::string &jsonString, DCSynths::Instrument *instrument);

	std::string SerializeTree(const std::vector<SerializableNode> &sNodes, bool prettyPrint = false);

	std::vector<SerializableNode> DeserializeTree(const std::string &jsonString);

	SerializableNode CreateDefaultSerializableNode(ConcreteNodeType nodeType);
}

#endif  // DCTREE_DCTREE_H_INCLUDED
