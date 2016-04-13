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

#define NODE_COMPOSITE_MAX_CHILDREN 16
#define NODE_DECORATOR_MAX_CHILDREN 1
#define NODE_LEAF_MAX_CHILDREN 0
#define NODEVIEW_WIDTH 200
#define NODEVIEW_PADDING 8
#define NODEVIEW_PARAMS_YOFFSET 28
#define NODECONNECTOR_SIZE 12



//==============================================================================
namespace DCTree
{
	NodeView::NodeView(SerializableNode sNode): _parent(nullptr), _isHighlighted(false)
	{
		_nodeType = sNode.NodeType;

		Init(sNode.x, sNode.y);

		for (size_t i = 0; i < sNode.Params.size(); ++i)
		{
			auto sParam = sNode.Params[i];

			for (int j = 0; j < _nodeParams.size(); ++j)
			{
				auto nParam = _nodeParams[j];

				if (nParam->Name.compare(sParam.Name) == 0)
				{
					switch (sParam.Type)
					{
					case NodeParamType::Int: 
						dynamic_cast<EditableNodeParamInt *>(nParam)->Value = sParam.IntValue;
						break;
					case NodeParamType::Double: 
						dynamic_cast<EditableNodeParamFloat*>(nParam)->Value = sParam.DoubleValue;
						break;
					case NodeParamType::String: 
						dynamic_cast<EditableNodeParamString*>(nParam)->Value = sParam.StringValue;
						break;
					default: break;
					}
				}
			}
		}

		_propertyPanel.refreshAll();
	}

	NodeView::NodeView(ConcreteNodeType nodeType, int x, int y) : _nodeType(nodeType), _parent(nullptr), _isHighlighted(false)
	{
		jassert(nodeType != ConcreteNodeType::COUNT);
		Init(x, y);
	}

	void NodeView::Init(int x, int y)
	{
		_canBeDeleted = _nodeType != ConcreteNodeType::ROOT;

		switch (_nodeType)
		{
		case ConcreteNodeType::ROOT:
			_maxChildren = 1;
			break;
		case ConcreteNodeType::Repeater:
			_maxChildren = NODE_DECORATOR_MAX_CHILDREN;
			break;
		case ConcreteNodeType::FiniteRepeater:
			_maxChildren = NODE_DECORATOR_MAX_CHILDREN;
			{
				auto repeatParam = new EditableNodeParamInt();
				repeatParam->Name = "NumRepeats";
				repeatParam->HasRange = true;
				repeatParam->Min = 1;
				repeatParam->Max = 32;
				repeatParam->Value = 2;
				_nodeParams.add(repeatParam);
			}
			break;
		case ConcreteNodeType::Sequence:
			_maxChildren = NODE_COMPOSITE_MAX_CHILDREN;
			break;
		case ConcreteNodeType::Selector:
			_maxChildren = NODE_COMPOSITE_MAX_CHILDREN;
			break;
		case ConcreteNodeType::PlayNote:
			_maxChildren = NODE_LEAF_MAX_CHILDREN;
			{
				auto noteParam = new EditableNodeParamInt();
				noteParam->Name = "Note";
				noteParam->HasRange = true;
				noteParam->Min = 0;
				noteParam->Max = 127;
				noteParam->Value = 60;
				_nodeParams.add(noteParam);
			}
			break;
		case ConcreteNodeType::StopNote:
			_maxChildren = NODE_LEAF_MAX_CHILDREN;
			{
				auto noteParam = new EditableNodeParamInt();
				noteParam->Name = "Note";
				noteParam->HasRange = true;
				noteParam->Min = 0;
				noteParam->Max = 127;
				noteParam->Value = 60;
				_nodeParams.add(noteParam);
			}
			break;
		default: break;
		}

		Array<PropertyComponent *> pcs;

		for (int i = 0; i < _nodeParams.size(); ++i)
		{
			auto intParam = dynamic_cast<EditableNodeParamInt *>(_nodeParams[i]);

			if (intParam)
			{
				pcs.add(new NodeParamSlider(intParam));
				continue;
			}

			auto floatParam = dynamic_cast<EditableNodeParamFloat *>(_nodeParams[i]);

			if (floatParam)
			{
				pcs.add(new NodeParamSlider(floatParam));
				continue;
			}
		}

		_propertyPanel.addProperties(pcs);
		addAndMakeVisible(_propertyPanel);

		if (_maxChildren > 0)
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
		g.drawText(GetNodeDisplayName(_nodeType), getLocalBounds().reduced(NODEVIEW_PADDING), Justification::topLeft);
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
		jassert(_maxChildren > 0);

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

		while (_children.size() >= _maxChildren)
		{
			RemoveChild(_children.size() - 1);
		}

		_children.insert(index, child);

		if (_children.size() < _maxChildren - 1)
		{
			AddConnector();
		}
	}

	void NodeView::MoveChild(int current, int destination)
	{
		if (_maxChildren <= 1 || current < 0 || destination < 0 || current >= _maxChildren || destination >= _maxChildren)
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

	SerializableNode NodeView::Serialize() const
	{
		SerializableNode sNode;
		sNode.NodeType = _nodeType;
		sNode.x = getX();
		sNode.y = getY();

		for (int i = 0; i < _nodeParams.size(); ++i)
		{
			SerializableNodeParam p;
			p.Name = _nodeParams[i]->Name;

			{
				auto intParam = dynamic_cast<EditableNodeParamInt *>(_nodeParams[i]);

				if (intParam)
				{
					p.Type = NodeParamType::Int;
					p.IntValue = intParam->Value;
					sNode.Params.push_back(p);
					continue;
				}
			}

			{
				auto floatParam = dynamic_cast<EditableNodeParamFloat *>(_nodeParams[i]);

				if (floatParam)
				{
					p.Type = NodeParamType::Double;
					p.DoubleValue = floatParam->Value;
					sNode.Params.push_back(p);
					continue;
				}
			}
			{
				auto stringParam = dynamic_cast<EditableNodeParamString *>(_nodeParams[i]);

				if (stringParam)
				{
					p.Type = NodeParamType::String;
					p.StringValue = stringParam->Value;
					sNode.Params.push_back(p);
					continue;
				}
			}
		}

		return sNode;
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
