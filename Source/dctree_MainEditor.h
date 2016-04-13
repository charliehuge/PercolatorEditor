/*
  ==============================================================================

    dctree_MainEditor.h
    Created: 13 Apr 2016 9:50:56am
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_MAINEDITOR_H_INCLUDED
#define DCTREE_MAINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include  "dctree_EditorContainer.h"

//==============================================================================
/*
*/
namespace DCTree
{
	class MainEditor : public TabbedComponent
	{
	public:
		MainEditor();
		~MainEditor();

		void NewTree();
		void LoadTree();
		void SaveTree() const;
		void SaveTreeAs() const;

	private:
		OwnedArray<EditorContainer> _editorContainers;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainEditor)
	};
}

#endif  // DCTREE_MAINEDITOR_H_INCLUDED
