/*
  ==============================================================================

    dctree_NodeParamTextBox.cpp
    Created: 10 Apr 2016 2:40:20pm
    Author:  charl_000

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_NodeParamTextBox.h"

//==============================================================================
namespace DCTree
{
	NodeParamTextBox::NodeParamTextBox(SerializableNodeParam* param): TextPropertyComponent(param->Name, 32, false), _param(param)
	{
	}

	NodeParamTextBox::~NodeParamTextBox()
	{
	}
}
