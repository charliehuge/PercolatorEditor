/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (800, 600);
	addAndMakeVisible(_mainEditor);
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
	_mainEditor.setBounds(getBounds());
}
