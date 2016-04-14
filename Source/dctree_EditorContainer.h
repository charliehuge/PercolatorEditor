/*
  ==============================================================================

    dctree_EditorContainer.h
    Created: 13 Apr 2016 9:51:24am
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_EDITORCONTAINER_H_INCLUDED
#define DCTREE_EDITORCONTAINER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_Editor.h"

//==============================================================================
/*
*/
namespace DCTree
{
	class EditorContainer : public Component
	{
	public:
		EditorContainer();
		~EditorContainer();

		void resized() override;

		bool Load();

		bool Save();

		bool SaveAs();

		String GetName() const;

		std::string GetSerialized() const;

	private:
		Viewport _viewport;
		Editor _treeEditor;
		File _associatedFile;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorContainer)
	};
}

#endif  // DCTREE_EDITORCONTAINER_H_INCLUDED
