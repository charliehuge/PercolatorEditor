/*
  ==============================================================================

    dctree_TestRunner.cpp
    Created: 13 Apr 2016 12:27:40pm
    Author:  charl_000

  ==============================================================================
*/

#include "JuceHeader.h"
#include "dctree_TestRunner.h"

namespace DCTree
{
	TestRunner::TestRunner() : _root(nullptr)
	{
	}

	TestRunner::~TestRunner()
	{
		if (_root)
		{
			delete _root;
		}
	}

	void TestRunner::Init(Node* root)
	{
		if (_root)
		{
			delete _root;
		}

		_root = root;
	}

	Result TestRunner::Tick(double tickTime) const
	{
		if (_root)
		{
			auto result = _root->Tick(tickTime);
			DBG("Ticked");
			return result;
		}

		DBG("No Root");
		return Result::Failure;
	}
}
