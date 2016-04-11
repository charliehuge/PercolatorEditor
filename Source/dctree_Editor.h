/*
  ==============================================================================

    dctree_Editor.h
    Created: 9 Apr 2016 6:59:13am
    Author:  Charlie Huguenard

  ==============================================================================
*/

#ifndef DCTREE_EDITOR_H_INCLUDED
#define DCTREE_EDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_NodeView.h"

//==============================================================================
/*
*/
namespace DCTree
{
	class Editor : public Component
	{
	public:
		Editor();
		~Editor();

		void paint(Graphics&) override;
		void resized() override;
		void mouseDown(const MouseEvent &e) override;
		void mouseDrag(const MouseEvent &e) override;
		void mouseUp(const MouseEvent &e) override;

	private:
		void addNodeView(ConcreteNodeType nodeType, int x, int y);

		OwnedArray<NodeView> _nodeViews;
		NodeConnector *_draggingConnector;
		Point<float> _mousePosition;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Editor)
	};
}

#endif  // DCTREE_EDITOR_H_INCLUDED