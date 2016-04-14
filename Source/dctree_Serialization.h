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
#include "dctree_SerializableNode.h"
#include "dctree_Node.h"

namespace DCTree
{	
	Node *CreateRuntimeTree(const std::string &jsonString);
}

#endif  // DCTREE_SERIALIZATION_H_INCLUDED
