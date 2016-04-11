/*
  ==============================================================================

    dctree_Node.h
    Created: 8 Apr 2016 4:17:03pm
    Author:  Charlie Huguenard

  ==============================================================================
*/

#ifndef DC_TREE_NODE_H_INCLUDED
#define DC_TREE_NODE_H_INCLUDED

namespace DCTree
{
	enum class Result
	{
		Success = 0,
		Failure,
		Running
	};

	class Node
	{
	public:
		Node();
		
		virtual ~Node();
		
		Result Tick(double tickTime);
		
		void Reset();
	
	protected:
		virtual Result OnStart() { return Result::Success; }
		
		virtual Result OnTick(double tickTime) = 0;
		
		virtual void OnStop() {}
	
	private:
		Result Start();
		
		void Stop();
		
		bool _started;
	};
}

#endif  // DC_TREE_NODE_H_INCLUDED
