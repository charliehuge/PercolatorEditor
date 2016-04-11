/*
  ==============================================================================

    dctree_NodeParamSlider.h
    Created: 10 Apr 2016 2:38:56pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_NODEPARAMSLIDER_H_INCLUDED
#define DCTREE_NODEPARAMSLIDER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "dctree_EditableNodeParam.h"

//==============================================================================
/*
*/
namespace DCTree
{
	class NodeParamSlider : public SliderPropertyComponent
	{
	public:
		explicit NodeParamSlider(EditableNodeParamFloat *param);
		explicit NodeParamSlider(EditableNodeParamInt *param);
		~NodeParamSlider();

		void setValue(double newValue) override;
		
	private:
		EditableNodeParam *_param;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NodeParamSlider)
	};
}

#endif  // DCTREE_NODEPARAMSLIDER_H_INCLUDED
