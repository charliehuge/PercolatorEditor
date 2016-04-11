/*
  ==============================================================================

    dctree_EditableNodeParam.cpp
    Created: 9 Apr 2016 7:38:45am
    Author:  Charlie Huguenard

  ==============================================================================
*/

#include "dctree_EditableNodeParam.h"

namespace DCTree
{
	EditableNodeParam::EditableNodeParam()
	{
	}

	EditableNodeParam::~EditableNodeParam()
	{
	}

	EditableNodeParamString::EditableNodeParamString()
	{
	}

	EditableNodeParamString::~EditableNodeParamString()
	{
	}

	EditableNodeParamInt::EditableNodeParamInt(): Value(0), HasRange(false), Min(0), Max(0)
	{
	}

	EditableNodeParamInt::~EditableNodeParamInt()
	{
	}

	EditableNodeParamFloat::EditableNodeParamFloat(): Value(0), HasRange(false), Min(0), Max(0)
	{
	}

	EditableNodeParamFloat::~EditableNodeParamFloat()
	{
	}
}
