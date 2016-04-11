/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (600, 400);

	_viewport.setViewedComponent(&_treeEditor);
	addAndMakeVisible(_viewport);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::antiquewhite);
}

void MainContentComponent::resized()
{
	_viewport.setBounds(getBounds());
}
