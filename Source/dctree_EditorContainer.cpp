/*
  ==============================================================================

    dctree_EditorContainer.cpp
    Created: 13 Apr 2016 9:51:24am
    Author:  charl_000

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_EditorContainer.h"

//==============================================================================
namespace DCTree
{
	EditorContainer::EditorContainer()
	{
		_viewport.setViewedComponent(&_treeEditor);
		addAndMakeVisible(_viewport);
	}

	EditorContainer::~EditorContainer()
	{
	}

	void EditorContainer::resized()
	{
		_viewport.setBounds(getBounds());
	}

	bool EditorContainer::Load()
	{
		FileChooser fc("Load from", File::getSpecialLocation(File::userDesktopDirectory), "*.dctree");

		if (fc.browseForFileToOpen())
		{
			_associatedFile = fc.getResult();
			_treeEditor.Deserialize(DeserializeTree(_associatedFile.loadFileAsString().toStdString()));
			repaint();
			return true;
		}

		return false;
	}

	bool EditorContainer::Save()
	{
		if (_associatedFile == File::nonexistent)
		{
			return SaveAs();
		}

		_associatedFile.replaceWithText(SerializeTree(_treeEditor.Serialize(), true));
		
		return true;
	}

	bool EditorContainer::SaveAs()
	{
		FileChooser fc("Save to", File::getSpecialLocation(File::userDesktopDirectory), "*.dctree");

		if (fc.browseForFileToSave(true))
		{
			_associatedFile = fc.getResult();
			_associatedFile.replaceWithText(SerializeTree(_treeEditor.Serialize(), true));
			return true;
		}

		return false;
	}

	String EditorContainer::GetName() const
	{
		return _associatedFile != File::nonexistent ? _associatedFile.getFileNameWithoutExtension() : "New Tree";
	}

	std::string EditorContainer::GetSerialized() const
	{
		return SerializeTree(_treeEditor.Serialize());
	}
}
