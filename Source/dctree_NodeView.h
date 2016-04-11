/*
  ==============================================================================

    NodeView.h
    Created: 9 Apr 2016 6:59:45am
    Author:  Charlie Huguenard

  ==============================================================================
*/

#ifndef DCTREE_NODEVIEW_H_INCLUDED
#define DCTREE_NODEVIEW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_EditableNodeParam.h"
#include "dctree_NodeConnector.h"

//==============================================================================
/*
*/
namespace DCTree
{
	enum class ConcreteNodeType
	{
		UNDEFINED = 0,
		Repeater,
		FiniteRepeater,
		Sequence,
		Selector,
		PlayNote,
		StopNote
	};

	String GetNodeDisplayName(ConcreteNodeType nodeType);

	class NodeView : public Component
	{
	public:
		NodeView(ConcreteNodeType nodeType, int x, int y);
		~NodeView();

		void paint(Graphics&) override;
		void resized() override;
		void mouseDown(const MouseEvent &e) override;
		void mouseDrag(const MouseEvent &e) override;

		NodeConnector *GetConnector(int index) const;
		int GetConnectorIndex(NodeConnector *connector) const;
		void AddChild(NodeView *child);
		void InsertChild(NodeView *child, int index);
		void MoveChild(int current, int destination);
		void RemoveChild(NodeView *child);
		void RemoveChild(int childIndex);
		NodeView *GetChild(int index) const;
		int GetNumChildren() const;
		Point<int> GetParentConnectionPosition() const;

	private:
		void AddConnector();
		void RemoveConnector();
		void RefreshConnectorPositions() const;

		ComponentDragger _dragger;
		ComponentBoundsConstrainer _constrainer;
		PropertyPanel _propertyPanel;
		OwnedArray<NodeConnector> _connectors;

		ConcreteNodeType _nodeType;
		OwnedArray<EditableNodeParam> _nodeParams;
		Array<NodeView *> _children;
		NodeView *_parent;
		int _maxChildren;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NodeView)
	};
}

#endif  // DCTREE_NODEVIEW_H_INCLUDED
