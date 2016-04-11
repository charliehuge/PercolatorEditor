/*
  ==============================================================================

    NodeConnector.cpp
    Created: 10 Apr 2016 3:48:08pm
    Author:  charl_000

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_NodeConnector.h"

//==============================================================================
namespace DCTree
{
	NodeConnector::NodeConnector()
	{
	}

	NodeConnector::~NodeConnector()
	{
	}

	void NodeConnector::paint(Graphics& g)
	{
		g.setColour(Colours::darkgrey);
		g.fillEllipse(0, 0, getWidth(), getHeight());
	}

	void NodeConnector::resized()
	{
	}

	Point<int> NodeConnector::getWirePosition() const
	{
		return Point<int>(getWidth() / 2, getHeight() / 2);
	}
}
