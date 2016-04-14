/*
  ==============================================================================

    dctree_TestRunner.h
    Created: 13 Apr 2016 12:27:40pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef DCTREE_TESTRUNNER_H_INCLUDED
#define DCTREE_TESTRUNNER_H_INCLUDED

#include "dctree_DCTree.h"

namespace DCTree
{
	class TestRunner
	{
	public:
		explicit TestRunner();
		~TestRunner();

		void Init(Node *root);

		Result Tick(double tickTime) const;

	private:
		Node *_root;
	};
}

#endif  // DCTREE_TESTRUNNER_H_INCLUDED
