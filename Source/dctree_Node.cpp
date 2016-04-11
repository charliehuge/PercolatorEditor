/*
  ==============================================================================

    dctree_Node.cpp
    Created: 8 Apr 2016 4:17:03pm
    Author:  Charlie Huguenard

  ==============================================================================
*/

#include "dctree_Node.h"

namespace DCTree
{
	Node::Node() : _started(false) {}

	Node::~Node() {}

	Result Node::Tick(double tickTime)
	{
		auto result = Start();

		if (result == Result::Failure)
		{
			Stop();
			return result;
		}
		
		result = OnTick(tickTime);
		
		if (result != Result::Running) Stop();
		
		return result;
	}

	void Node::Reset()
	{
		Stop();
	}

	Result Node::Start()
	{
		if (_started) return Result::Success;
		
		_started = true;
		
		return OnStart();
	}

	void Node::Stop()
	{
		_started = false;
		
		OnStop();
	}
}
