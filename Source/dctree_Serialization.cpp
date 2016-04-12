/*
  ==============================================================================

    dctree_Serialization.cpp
    Created: 11 Apr 2016 4:28:55pm
    Author:  charl_000

  ==============================================================================
*/

#include "dctree_Serialization.h"

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
}
