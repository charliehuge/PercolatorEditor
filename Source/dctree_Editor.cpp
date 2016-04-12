/*
  ==============================================================================

    dctree_Editor.cpp
    Created: 9 Apr 2016 6:59:13am
    Author:  Charlie Huguenard

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_Editor.h"
#include "dctree_Serialization.h"

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
			g.drawLine(static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(_mousePosition.x), static_cast<float>(_mousePosition.y), 2);
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
					g.drawLine(static_cast<float>(connectorPos.x), static_cast<float>(connectorPos.y), static_cast<float>(childPos.x), static_cast<float>(childPos.y), 2);
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
				parentNodeView->InsertChild(childNodeView, connectorIdx);
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
			_selectedNodeView->RemoveParent();
			_selectedNodeView->RemoveAllChildren();
			_nodeViews.remove(_nodeViews.indexOf(_selectedNodeView));
			_selectedNodeView = nullptr;
			repaint();
			return true;
		}

		if (key.isKeyCode(KeyPress::escapeKey))
		{
			json js = ToJson();
			FileChooser fc("Save to", File::getSpecialLocation(File::userDesktopDirectory), "*.dctree");

			if (fc.browseForFileToSave(true))
			{
				File dctFile(fc.getResult());
				dctFile.replaceWithText(js.dump(4));
			}
		}

		if (key.isKeyCode(KeyPress::homeKey))
		{
			FileChooser fc("Load from", File::getSpecialLocation(File::userDesktopDirectory), "*.dctree");

			if (fc.browseForFileToOpen())
			{
				File dctFile(fc.getResult());
				FromJson(json::parse(dctFile.loadFileAsString().toStdString()));
				repaint();
			}
		}

		return false;
	}

	json Editor::ToJson() const
	{
		json js;

		for (int i = 0; i < _nodeViews.size(); ++i)
		{
			js += _nodeViews[i]->ToJson();
		}

		// add the children
		for (int i = 0; i < _nodeViews.size(); ++i)
		{
			auto nv = _nodeViews[i];
			json children;

			for (int j = 0; j < nv->GetNumChildren(); ++j)
			{
				children += _nodeViews.indexOf(nv->GetChild(j));
			}

			js[i][DCT_JSON_CHILDREN] = children;
		}

		return js;
	}

	void Editor::FromJson(json jsonObject)
	{
		_nodeViews.clear();
		_draggingConnector = nullptr;
		_selectedNodeView = nullptr;

		for (size_t i = 0; i < jsonObject.size(); ++i)
		{
			addNodeView(jsonObject[i]);
		}

		for (int i = 0; i < _nodeViews.size(); ++i)
		{
			auto nv = _nodeViews[i];
			auto nJs = jsonObject[i];
			auto childJs = nJs[DCT_JSON_CHILDREN];

			for (size_t j = 0; j < childJs.size(); ++j)
			{
				jassert(childJs[j].is_number_integer());
				nv->InsertChild(_nodeViews[childJs[j].get<int>()], j);
			}
		}
	}

	void Editor::addNodeView(ConcreteNodeType nodeType, int x, int y)
	{
		auto nv = new NodeView(nodeType, x, y);
		_nodeViews.add(nv);
		addAndMakeVisible(nv);
		nv->addMouseListener(this, true);
	}

	void Editor::addNodeView(json jsonObject)
	{
		auto nv = new NodeView(jsonObject);
		_nodeViews.add(nv);
		addAndMakeVisible(nv);
		nv->addMouseListener(this, true);
	}
}
