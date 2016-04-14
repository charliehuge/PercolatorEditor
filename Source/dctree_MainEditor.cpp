/*
  ==============================================================================

    dctree_MainEditor.cpp
    Created: 13 Apr 2016 9:50:56am
    Author:  charl_000

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_MainEditor.h"

//==============================================================================
namespace DCTree
{
	MainEditor::MainEditor(): TabbedComponent(TabbedButtonBar::Orientation::TabsAtTop)
	{
		NewTree();
	}

	MainEditor::~MainEditor()
	{
	}

	Colour getRandomColour()
	{
		return Colour(Random::getSystemRandom().nextFloat(), 0.1f, 0.9f, 1.0f);
	}

	void MainEditor::NewTree()
	{
		addTab("new tree", getRandomColour(), new EditorContainer(), true);
	}

	void MainEditor::LoadTree()
	{
		auto ec = new EditorContainer();
		if (ec->Load())
		{
			addTab(ec->GetName(), getRandomColour(), ec, true);
			repaint();
		}
		else
		{
			delete ec;
		}
	}

	void MainEditor::SaveTree() const
	{
		auto ec = dynamic_cast<EditorContainer*>(getCurrentContentComponent());

		if (ec)
		{
			ec->Save();
		}
	}

	void MainEditor::SaveTreeAs() const
	{
		auto ec = dynamic_cast<EditorContainer*>(getCurrentContentComponent());

		if (ec)
		{
			ec->SaveAs();
		}
	}

	std::string MainEditor::GetCurrentTabSerialized() const
	{
		auto ec = dynamic_cast<EditorContainer*>(getCurrentContentComponent());

		if (ec)
		{
			return ec->GetSerialized();
		}

		return "";
	}
}
