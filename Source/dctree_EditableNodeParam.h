/*
  ==============================================================================

    dctree_EditableNodeParam.h
    Created: 9 Apr 2016 7:38:45am
    Author:  Charlie Huguenard

  ==============================================================================
*/

#ifndef DCTREE_NODEPARAM_H_INCLUDED
#define DCTREE_NODEPARAM_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

namespace DCTree
{
	class EditableNodeParam
	{
	public:
		EditableNodeParam();

		virtual ~EditableNodeParam();

		String Name;
	};

	class EditableNodeParamString : public EditableNodeParam
	{
	public:
		EditableNodeParamString();

		~EditableNodeParamString();

		String Value;
	};

	class EditableNodeParamInt : public EditableNodeParam
	{
	public:
		EditableNodeParamInt();

		~EditableNodeParamInt();

		int Value;
		bool HasRange;
		int Min;
		int Max;
	};

	class EditableNodeParamFloat : public EditableNodeParam
	{
	public:
		EditableNodeParamFloat();

		~EditableNodeParamFloat();

		float Value;
		bool HasRange;
		float Min;
		float Max;
	};
}

#endif  // DCTREE_NODEPARAM_H_INCLUDED
