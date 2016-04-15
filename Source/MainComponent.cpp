/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#define MENUID_NEW 1000
#define MENUID_LOAD 1001
#define MENUID_SAVE 1002
#define MENUID_SAVEAS 1003

#define MENUID_RUNTEST 2000
#define MENUID_STOPTEST 2001

//==============================================================================
MainContentComponent::MainContentComponent()
{
	addAndMakeVisible(_mainEditor);
	addAndMakeVisible(_menu = new MenuBarComponent(this));
	setSize(800, 600);
	setAudioChannels(0, 2);
}

MainContentComponent::~MainContentComponent()
{
	shutdownAudio();
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::antiquewhite);
}

void MainContentComponent::resized()
{
	auto area = getLocalBounds();
	auto menuArea = area.removeFromTop(getLookAndFeel().getDefaultMenuBarHeight());
	_menu->setBounds(menuArea);
	_mainEditor.setBounds(area);
}

StringArray MainContentComponent::getMenuBarNames()
{
	const char* const names[] = { "File", "Test", nullptr };

	return StringArray(names);
}

PopupMenu MainContentComponent::getMenuForIndex(int topLevelMenuIndex, const String& /*menuName*/)
{
	PopupMenu menu;

	if (topLevelMenuIndex == 0)
	{
		menu.addItem(MENUID_NEW, "New");
		menu.addItem(MENUID_LOAD, "Load");
		menu.addItem(MENUID_SAVE, "Save");
		menu.addItem(MENUID_SAVEAS, "Save As...");
	}
	else if (topLevelMenuIndex == 1)
	{
		menu.addItem(MENUID_RUNTEST, "Run");
		menu.addItem(MENUID_STOPTEST, "Stop");
	}

	return menu;
}

void MainContentComponent::menuItemSelected(int menuItemID, int /*topLevelMenuIndex*/)
{
	switch (menuItemID)
	{
	case MENUID_NEW:
		_mainEditor.NewTree();
		break;
	case MENUID_LOAD:
		_mainEditor.LoadTree();
		break;
	case MENUID_SAVE:
		_mainEditor.SaveTree();
		break;
	case MENUID_SAVEAS:
		_mainEditor.SaveTreeAs();
		break;
	case MENUID_RUNTEST:
		_testTimer->Start(_mainEditor.GetCurrentTabSerialized());
		break;
	case MENUID_STOPTEST:
		_testTimer->Stop();
		break;
	}
}

void MainContentComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	if (!_testTimer)
	{
		_testTimer = new TestTimer(sampleRate);
	}
}

void MainContentComponent::releaseResources()
{
}

void MainContentComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	bufferToFill.clearActiveBufferRegion();

	for (int cIdx = 0; cIdx < bufferToFill.buffer->getNumChannels(); ++cIdx)
	{
		float *const channelData = bufferToFill.buffer->getWritePointer(cIdx, bufferToFill.startSample);

		if (cIdx == 0)
		{
			_testTimer->ProcessBuffer(channelData, bufferToFill.numSamples);
		}
		else
		{
			memcpy(channelData, bufferToFill.buffer->getReadPointer(0), bufferToFill.numSamples * sizeof(float));
		}
	}
}
