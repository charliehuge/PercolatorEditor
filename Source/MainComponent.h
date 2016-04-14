/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_MainEditor.h"
#include "dctree_TestRunner.h"

class TestTimer : public HighResolutionTimer
{
public:
	TestTimer(): _elapsedTime(0)
	{}

	~TestTimer() {}

	void hiResTimerCallback() override
	{
		_elapsedTime += (INTERVAL / 1000.0);
		_testRunner.Tick(_elapsedTime);
	}

	void Start(const std::string &json)
	{
		_testRunner.Init(DCTree::CreateRuntimeTree(json));
		_elapsedTime = 0;
		startTimer(INTERVAL);
	}

	void Stop()
	{
		stopTimer();
	}

private:
	const int INTERVAL = 500;

	DCTree::TestRunner _testRunner;
	double _elapsedTime;
};
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component, public MenuBarModel
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;
	StringArray getMenuBarNames() override;
	PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
	void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

private:
	DCTree::MainEditor _mainEditor;
	ScopedPointer<MenuBarComponent> _menu;
	TestTimer _testTimer;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
