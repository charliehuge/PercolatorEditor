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
	NodeView::NodeView(json jsonObject): _parent(nullptr), _isHighlighted(false)
	{
		// get the node type
		{
			auto it = jsonObject.find(DCT_JSON_NODETYPE);
			jassert(it != jsonObject.end());
			const auto typeName = it.value().get<std::string>();
			_nodeType = GetNodeTypeFromName(typeName);
			jassert(_nodeType != ConcreteNodeType::COUNT && _nodeType != ConcreteNodeType::INVALID);
		}

		int x, y;
		{
			auto it = jsonObject.find(DCT_JSON_EDITORPOS);
			jassert(it != jsonObject.end());
			auto pos = it.value();
			x = pos[0].get<int>();
			y = pos[1].get<int>();
		}

		Init(x, y);

		// assign the parameter values
		auto paramsIt = jsonObject.find(DCT_JSON_PARAMS);
		jassert(paramsIt != jsonObject.end());
		auto paramsJ = paramsIt.value();

		for (int i = 0; i < _nodeParams.size(); ++i)
		{
			auto pIt = paramsJ.find(_nodeParams[i]->Name);
			jassert(pIt != paramsJ.end());

			{
				auto intP = dynamic_cast<EditableNodeParamInt *>(_nodeParams[i]);

				if (intP)
				{
					intP->Value = pIt.value().get<int>();
					continue;
				}
			}
			{
				auto floatP = dynamic_cast<EditableNodeParamFloat *>(_nodeParams[i]);

				if (floatP)
				{
					floatP->Value = pIt.value().get<double>();
					continue;
				}
			}
			{
				auto stringP = dynamic_cast<EditableNodeParamString *>(_nodeParams[i]);

				if (stringP)
				{
					stringP->Value = pIt.value().get<std::string>();
					continue;
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

	json NodeView::ToJson() const
	{
		json j;
		j[DCT_JSON_NODETYPE] = GetNodeTypeName(_nodeType);
		j[DCT_JSON_EDITORPOS] = { getPosition().getX(), getPosition().getY() };		
		j[DCT_JSON_PARAMS] = {};
		for (int i = 0; i < _nodeParams.size(); ++i)
		{
			{
				auto intParam = dynamic_cast<EditableNodeParamInt *>(_nodeParams[i]);

				if (intParam)
				{
					j[DCT_JSON_PARAMS][intParam->Name] = intParam->Value;
					continue;
				}
			}

			{
				auto floatParam = dynamic_cast<EditableNodeParamFloat *>(_nodeParams[i]);

				if (floatParam)
				{
					j[DCT_JSON_PARAMS][floatParam->Name] = floatParam->Value;
					continue;
				}
			}
			{
				auto stringParam = dynamic_cast<EditableNodeParamString *>(_nodeParams[i]);

				if (stringParam)
				{
					j[DCT_JSON_PARAMS][stringParam->Name] = stringParam->Value;
					continue;
				}
			}
		}
		return j;
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
