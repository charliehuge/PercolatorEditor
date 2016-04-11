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
	NodeParamSlider::NodeParamSlider(EditableNodeParamFloat* param) 
		: SliderPropertyComponent(param->Name, param->HasRange ? param->Min : -9999, param->HasRange ? param->Max : 9999, 0),
		_param(param)
	{
	}

	NodeParamSlider::NodeParamSlider(EditableNodeParamInt* param)
		: SliderPropertyComponent(param->Name, param->HasRange ? param->Min : -9999, param->HasRange ? param->Max : 9999, 1),
		_param(param)
	{
	}

	NodeParamSlider::~NodeParamSlider()
	{
	}

	void NodeParamSlider::setValue(double newValue)
	{
		auto intParam = dynamic_cast<EditableNodeParamInt *>(_param);

		if (intParam)
		{
			intParam->Value = static_cast<int>(newValue);
			return;
		}

		auto floatParam = dynamic_cast<EditableNodeParamFloat *>(_param);

		if (floatParam)
		{
			floatParam->Value = static_cast<float>(newValue);
			return;
		}
	}
}
