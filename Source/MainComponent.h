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
#include "Subtractinator.h"

class TestTimer : public HighResolutionTimer
{
public:
	explicit TestTimer(double sampleRate): _elapsedTime(0)
	{
		_instrument = new DCSynths::Subtractinator(static_cast<int>(sampleRate));
		_instrument->SetParameter(0, 0.1);
		_instrument->SetParameter(1, 0.1);
		_instrument->SetParameter(2, 0.1);
		_instrument->SetParameter(3, 0.7);
		_instrument->SetParameter(4, 0.5);
		_instrument->SetParameter(5, 0.1);
		_instrument->SetParameter(6, 0.1);
		_instrument->SetParameter(7, 0.2);
		_instrument->SetParameter(8, 0.2);
	}

	~TestTimer() {}

	void hiResTimerCallback() override
	{
		_elapsedTime += (INTERVAL / 1000.0);
		_testRunner.Tick(_elapsedTime);
	}

	void Start(const std::string &json)
	{
		_testRunner.Init(DCTree::CreateRuntimeTree(json, _instrument));
		_elapsedTime = 0;
		startTimer(INTERVAL);
	}

	void Stop()
	{
		stopTimer();
		_instrument->ReleaseNote(0, 0);
	}

	void ProcessBuffer(float *buffer, int numSamples) const
	{
		_instrument->ProcessBuffer(buffer, numSamples);
	}

private:
	const int INTERVAL = 125;

	DCTree::TestRunner _testRunner;
	double _elapsedTime;
	ScopedPointer<DCSynths::Instrument> _instrument;
};
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent, public MenuBarModel
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
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void releaseResources() override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

private:
	DCTree::MainEditor _mainEditor;
	ScopedPointer<MenuBarComponent> _menu;
	ScopedPointer<TestTimer> _testTimer;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
