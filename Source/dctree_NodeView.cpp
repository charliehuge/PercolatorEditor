/*
  ==============================================================================

    NodeView.cpp
    Created: 9 Apr 2016 6:59:45am
    Author:  Charlie Huguenard

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_NodeView.h"
#include "dctree_NodeParamSlider.h"

#define NODEVIEW_WIDTH 200
#define NODEVIEW_PADDING 8
#define NODEVIEW_PARAMS_YOFFSET 28
#define NODECONNECTOR_SIZE 12



//==============================================================================
namespace DCTree
{
	NodeView::NodeView(const SerializableNode &sNode): _serializableNode(sNode), _parent(nullptr), _isHighlighted(false)
	{
		Init(sNode.x, sNode.y);
	}

	NodeView::NodeView(ConcreteNodeType nodeType, int x, int y) : _parent(nullptr), _isHighlighted(false)
	{
		if (nodeType == ConcreteNodeType::ROOT)
		{
			_serializableNode.NodeType = ConcreteNodeType::ROOT;
			_serializableNode.MaxChildren = 1;
		}
		else
		{
			_serializableNode = CreateDefaultSerializableNode(nodeType);
		}

		jassert(_serializableNode.NodeType != ConcreteNodeType::INVALID);

		Init(x, y);
	}

	void NodeView::Init(int x, int y)
	{
		_canBeDeleted = _serializableNode.NodeType != ConcreteNodeType::ROOT;

		Array<PropertyComponent *> pcs;

		for (int i = 0; i < _serializableNode.Params.size(); ++i)
		{
			auto param = _serializableNode.Params[i];

			switch (param.Type)
			{
			case NodeParamType::Int: 
			case NodeParamType::Double: 
				pcs.add(new NodeParamSlider(&param));
				break;
			case NodeParamType::String: break;
			case NodeParamType::Result: break;
			default: break;
			}
		}

		_propertyPanel.addProperties(pcs);
		addAndMakeVisible(_propertyPanel);

		if (_serializableNode.MaxChildren > 0)
		{
			AddConnector();
		}

		setBounds(x, y, NODEVIEW_WIDTH, _propertyPanel.getTotalContentHeight() + NODEVIEW_PARAMS_YOFFSET + NODEVIEW_PADDING * 2);
	}

	NodeView::~NodeView()
	{
	}

	void NodeView::paint(Graphics& g)
	{
		g.setColour(Colours::lightgrey);
		g.fillRoundedRectangle(0.0f, 0.0f, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 4);

		g.setColour(Colours::darkgrey);
		g.drawRoundedRectangle(0.0f, 0.0f, static_cast<float>(getWidth()), static_cast<float>(getHeight()), 4.0f, _isHighlighted ? 3.0f : 1.0f);

		g.setColour(Colours::darkgrey);
		g.setFont(18);
		g.drawText(_serializableNode.GetDisplayName(), getLocalBounds().reduced(NODEVIEW_PADDING), Justification::topLeft);
	}

	void NodeView::resized()
	{
		_constrainer.setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(), getWidth());
		_propertyPanel.setBounds(NODEVIEW_PADDING, NODEVIEW_PARAMS_YOFFSET, getWidth() - NODEVIEW_PADDING * 2, getHeight() - NODEVIEW_PARAMS_YOFFSET - NODEVIEW_PADDING * 2);
		RefreshConnectorPositions();
	}

	void NodeView::mouseDown(const MouseEvent& e)
	{
		if (e.mods.isLeftButtonDown())
		{
			_dragger.startDraggingComponent(this, e);
		}
	}

	void NodeView::mouseDrag(const MouseEvent& e)
	{
		if (e.mods.isLeftButtonDown())
		{
			_dragger.dragComponent(this, e, &_constrainer);
			getParentComponent()->repaint();
		}
	}

	NodeConnector* NodeView::GetConnector(int index) const
	{
		if (index >= 0 && index < _connectors.size())
		{
			return _connectors[index];
		}

		return nullptr;
	}

	int NodeView::GetConnectorIndex(NodeConnector* connector) const
	{
		return _connectors.indexOf(connector);
	}

	void NodeView::InsertChild(NodeView* child, int index)
	{
		jassert(_serializableNode.MaxChildren > 0);

		if (child->_parent)
		{
			child->_parent->RemoveChild(child);
		}

		child->_parent = this;
		
		{
			auto idx = _children.indexOf(child);
			if (idx >= 0)
			{
				_children.remove(idx);
			}
		}

		while (_children.size() >= _serializableNode.MaxChildren)
		{
			RemoveChild(_children.size() - 1);
		}

		_children.insert(index, child);

		if (_children.size() < _serializableNode.MaxChildren - 1)
		{
			AddConnector();
		}
	}

	void NodeView::MoveChild(int current, int destination)
	{
		if (_serializableNode.MaxChildren <= 1 || current < 0 || destination < 0 || current >= _serializableNode.MaxChildren || destination >= _serializableNode.MaxChildren)
		{
			return;
		}

		auto moving = _children.remove(current);

		if (current < destination)
		{
			--destination;
		}

		_children.insert(destination, moving);
	}

	void NodeView::RemoveChild(NodeView* child)
	{
		jassert(child != nullptr);

		RemoveChild(_children.indexOf(child));
	}

	void NodeView::RemoveChild(int childIndex)
	{
		jassert(childIndex > -1 && childIndex < _children.size());

		_children[childIndex]->_parent = nullptr;
		_children.remove(childIndex);

		if (_connectors.size() > 1)
		{
			RemoveConnector();
		}
	}

	void NodeView::RemoveAllChildren()
	{
		while (_children.size() > 0)
		{
			RemoveChild(0);
		}
	}

	void NodeView::RemoveParent()
	{
		if (_parent)
		{
			_parent->RemoveChild(this);
		}
	}

	NodeView* NodeView::GetChild(int index) const
	{
		if (index >= 0 && index < _children.size())
		{
			return _children[index];
		}

		return nullptr;
	}

	int NodeView::GetNumChildren() const
	{
		return _children.size();
	}

	Point<int> NodeView::GetParentConnectionPosition() const
	{
		auto boundsInParent = getBoundsInParent();
		return Point<int>(boundsInParent.getX() + boundsInParent.getWidth() / 2, boundsInParent.getY());
	}

	void NodeView::Highlight(bool highlight)
	{
		_isHighlighted = highlight;
	}

	bool NodeView::CanBeDeleted() const
	{
		return _canBeDeleted;
	}

	SerializableNode NodeView::GetSerializableNode() const
	{
		return _serializableNode;
	}

	void NodeView::AddConnector()
	{
		auto connector = new NodeConnector();
		addAndMakeVisible(connector);
		connector->setSize(NODECONNECTOR_SIZE, NODECONNECTOR_SIZE);
		_connectors.add(connector);
		RefreshConnectorPositions();
	}

	void NodeView::RemoveConnector()
	{
		_connectors.removeLast();
		RefreshConnectorPositions();
	}

	void NodeView::RefreshConnectorPositions() const
	{
		for (int i = 0; i < _connectors.size(); ++i)
		{
			_connectors[i]->setTopLeftPosition(NODEVIEW_PADDING + static_cast<int>(i * NODECONNECTOR_SIZE * 1.2f), getHeight() - _connectors[i]->getHeight() / 2);
		}
	}
}
