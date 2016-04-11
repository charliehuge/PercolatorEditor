/*
  ==============================================================================

    dctree_Editor.cpp
    Created: 9 Apr 2016 6:59:13am
    Author:  Charlie Huguenard

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_Editor.h"

#define DCTREEEDITOR_WIDTH 4000
#define DCTREEEDITOR_HEIGHT 2000

namespace DCTree
{
	//==============================================================================
	Editor::Editor() : _draggingConnector(nullptr)
	{
		setBounds(0, 0, DCTREEEDITOR_WIDTH, DCTREEEDITOR_HEIGHT);
	}

	Editor::~Editor()
	{
	}

	void Editor::paint(Graphics& g)
	{
		g.fillAll(Colours::azure);
		g.setColour(Colours::lightgrey);
		g.drawRect(getLocalBounds(), 2);

		if (_draggingConnector)
		{
			g.setColour(Colours::darkgrey);
			auto pos = getLocalPoint(_draggingConnector, _draggingConnector->getWirePosition());
			g.drawLine(pos.x, pos.y, _mousePosition.x, _mousePosition.y, 2);
		}

		g.setColour(Colours::grey);

		for (int i = 0; i < _nodeViews.size(); ++i)
		{
			auto nv = _nodeViews[i];

			for (int j = 0; j < nv->GetNumChildren(); ++j)
			{
				auto child = nv->GetChild(j);
				auto connector = nv->GetConnector(j);

				if (child && connector)
				{
					auto connectorPos = getLocalPoint(connector, connector->getWirePosition());
					auto childPos = child->GetParentConnectionPosition();
					g.drawLine(connectorPos.x, connectorPos.y, childPos.x, childPos.y);
				}
			}
		}
	}

	void Editor::resized()
	{
	}

	void Editor::mouseDown(const MouseEvent& e)
	{
		if (e.originalComponent == this)
		{
			if (e.mods.isLeftButtonDown())
				addNodeView(ConcreteNodeType::PlayNote, e.position.x, e.position.y);
			else if (e.mods.isRightButtonDown())
				addNodeView(ConcreteNodeType::Sequence, e.position.x, e.position.y);
			else if (e.mods.isMiddleButtonDown())
				addNodeView(ConcreteNodeType::FiniteRepeater, e.position.x, e.position.y);
		}

		// check if we clicked a connector
		else if (e.mods.isLeftButtonDown())
		{
			_draggingConnector = dynamic_cast<NodeConnector *>(e.originalComponent);

			if (_draggingConnector)
			{
				_mousePosition = getLocalPoint(e.originalComponent, e.position);
			}
		}
	}

	void Editor::mouseDrag(const MouseEvent& e)
	{
		if (_draggingConnector)
		{
			_mousePosition = getLocalPoint(e.originalComponent, e.position);
			repaint();
		}
	}

	void Editor::mouseUp(const MouseEvent& e)
	{
		if (_draggingConnector)
		{
			auto parentNodeView = dynamic_cast<NodeView *>(_draggingConnector->getParentComponent());
			NodeView *childNodeView = nullptr;
			auto mp = getLocalPoint(e.originalComponent, e.getPosition());

			for (int i = 0; i < _nodeViews.size(); ++i)
			{
				if (_nodeViews[i]->getBoundsInParent().contains(mp))
				{
					childNodeView = _nodeViews[i];
					break;
				}
			}

			if (parentNodeView != nullptr && childNodeView != nullptr && parentNodeView != childNodeView)
			{
				int connectorIdx = parentNodeView->GetConnectorIndex(_draggingConnector);

				if (connectorIdx < parentNodeView->GetNumChildren())
				{
					parentNodeView->InsertChild(childNodeView, connectorIdx);
				}
				else
				{
					parentNodeView->AddChild(childNodeView);
				}
			}

			_draggingConnector = nullptr;
			repaint();
		}
	}

	void Editor::addNodeView(ConcreteNodeType nodeType, int x, int y)
	{
		auto nv = new NodeView(nodeType, x, y);
		_nodeViews.add(nv);
		addAndMakeVisible(nv);
		nv->addMouseListener(this, true);
	}
}
