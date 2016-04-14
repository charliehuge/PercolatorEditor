/*
  ==============================================================================

    dctree_NodeParamTextBox.h
    Created: 10 Apr 2016 2:40:20pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_NODEPARAMTEXTBOX_H_INCLUDED
#define DCTREE_NODEPARAMTEXTBOX_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_DCTree.h"

//==============================================================================
/*
*/
namespace DCTree
{
	class NodeParamTextBox : public TextPropertyComponent
	{
	public:
		explicit NodeParamTextBox(SerializableNodeParam *param);
		~NodeParamTextBox();


	private:
		SerializableNodeParam *_param;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NodeParamTextBox)
	};
}

#endif  // DCTREE_NODEPARAMTEXTBOX_H_INCLUDED
