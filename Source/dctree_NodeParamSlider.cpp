/*
  ==============================================================================

    dctree_NodeParamSlider.cpp
    Created: 10 Apr 2016 2:38:56pm
    Author:  charl_000

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_NodeParamSlider.h"

//==============================================================================
namespace DCTree
{
	NodeParamSlider::NodeParamSlider(SerializableNodeParam* param): SliderPropertyComponent(param->Name, -999, 999, 0), _param(param)
	{
		switch (param->Type)
		{
		case NodeParamType::Int: 
			if (param->HasRange)
			{
				slider.setRange(param->IntMin, param->IntMax, 1);
			}
			slider.setValue(param->IntValue);
			break;
		case NodeParamType::Double: 
			if (param->HasRange)
			{
				slider.setRange(param->DoubleMin, param->DoubleMax);
			}
			slider.setValue(param->DoubleValue);
			break;
		case NodeParamType::Result:
		case NodeParamType::String:
			jassertfalse;
		}
	}

	NodeParamSlider::~NodeParamSlider()
	{
	}

	void NodeParamSlider::setValue(double newValue)
	{
		switch (_param->Type)
		{
		case NodeParamType::Int:
			_param->IntValue = static_cast<int>(newValue);
			break;
		case NodeParamType::Double: 
			_param->DoubleValue = newValue;
			break;
		case NodeParamType::String:
		case NodeParamType::Result:
			jassertfalse;
			break;
		}
	}

	double NodeParamSlider::getValue() const
	{
		switch (_param->Type)
		{
		case NodeParamType::Int:
			return _param->IntValue;
		case NodeParamType::Double:
			return _param->DoubleValue;
		case NodeParamType::String:
		case NodeParamType::Result:
			jassertfalse;
		}

		return 0;
	}
}
