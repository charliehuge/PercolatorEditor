/*
  ==============================================================================

    NodeConnector.h
    Created: 10 Apr 2016 3:48:08pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_NODECONNECTOR_H_INCLUDED
#define DCTREE_NODECONNECTOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
namespace DCTree
{
	class NodeConnector : public Component
	{
	public:
		NodeConnector();
		~NodeConnector();

		void paint(Graphics&) override;
		void resized() override;

		Point<int> getWirePosition() const;

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NodeConnector)
	};
}

#endif  // DCTREE_NODECONNECTOR_H_INCLUDED
