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
	Editor::Editor() : _draggingConnector(nullptr), _selectedNodeView(nullptr)
	{
		setBounds(0, 0, DCTREEEDITOR_WIDTH, DCTREEEDITOR_HEIGHT);
		addNodeView(ConcreteNodeType::ROOT, 100, 10);
		setWantsKeyboardFocus(true);
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
					g.drawLine(connectorPos.x, connectorPos.y, childPos.x, childPos.y, 2);
				}
			}
		}
	}

	void Editor::resized()
	{
	}

	void Editor::mouseDown(const MouseEvent& e)
	{
		_mousePosition = getLocalPoint(e.originalComponent, e.getPosition());

		// check if we clicked a connector
		if (e.mods.isLeftButtonDown())
		{
			_draggingConnector = dynamic_cast<NodeConnector *>(e.originalComponent);
		}
	}

	void Editor::mouseDrag(const MouseEvent& e)
	{
		_mousePosition = getLocalPoint(e.originalComponent, e.getPosition());

		if (_draggingConnector)
		{
			for (int i = 0; i < _nodeViews.size(); ++i)
			{
				_nodeViews[i]->Highlight(_nodeViews[i]->getBoundsInParent().contains(_mousePosition));
			}

			repaint();
		}
	}

	void Editor::mouseUp(const MouseEvent& e)
	{
		_mousePosition = getLocalPoint(e.originalComponent, e.getPosition());

		if (_draggingConnector)
		{
			auto parentNodeView = dynamic_cast<NodeView *>(_draggingConnector->getParentComponent());
			NodeView *childNodeView = nullptr;

			for (int i = 0; i < _nodeViews.size(); ++i)
			{
				_nodeViews[i]->Highlight(false);

				if (_nodeViews[i]->getBoundsInParent().contains(_mousePosition))
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
		else if (e.mouseWasClicked() && e.mods.isRightButtonDown())
		{
			PopupMenu p;
			p.addSectionHeader("Add Node");
			int count = static_cast<int>(ConcreteNodeType::COUNT);
			for (int i = 0; i < count; ++i)
			{
				p.addItem(i + 1, GetNodeDisplayName(static_cast<ConcreteNodeType>(i)));
			}

			const int result = p.show();

			if (result == 0)
			{
				DBG("Cancelled menu");
			}
			else
			{
				addNodeView(static_cast<ConcreteNodeType>(result - 1), _mousePosition.x, _mousePosition.y);
			}
		}
		else if (e.mouseWasClicked() && e.mods.isLeftButtonDown())
		{
			_selectedNodeView = nullptr;

			for (int i = 0; i < _nodeViews.size(); ++i)
			{
				if (_nodeViews[i]->getBoundsInParent().contains(_mousePosition))
				{
					_selectedNodeView = _nodeViews[i];
				}

				_nodeViews[i]->Highlight(_nodeViews[i] == _selectedNodeView);
			}

			repaint();
		}
	}

	bool Editor::keyPressed(const KeyPress& key)
	{
		if (key.isKeyCode(KeyPress::deleteKey) && _selectedNodeView != nullptr && _selectedNodeView->CanBeDeleted())
		{
			_nodeViews.remove(_nodeViews.indexOf(_selectedNodeView));
			return true;
		}

		return false;
	}

	void Editor::addNodeView(ConcreteNodeType nodeType, int x, int y)
	{
		auto nv = new NodeView(nodeType, x, y);
		_nodeViews.add(nv);
		addAndMakeVisible(nv);
		nv->addMouseListener(this, true);
	}
}
