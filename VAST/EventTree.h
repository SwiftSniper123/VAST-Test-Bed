/* VAST 
Author: emf
*/
#ifndef EventTree_h
#define EventTree_h
#include "VComponent.h"

using std::string;
using std::map;

typedef double time;

class EventTree
{
private:
	void advanceClock()
	{};
	//void updateDatabase(time time, data data) {};

	//eventCleanup();
public:
	/* Function called by external components to provide Event changes to the EventTree.  
	source	- the VAST component sending the event
	time	- the time the event is posted
	dataMap	- the set of data effected in the event*/
	/*void addEvent(VComponent* source, time time, map<string, VType> dataMap)
	{
	};

	time getScenarioTime()
	{
		return time(0.0);
	};

	void registerComponent(VComponent* component)
	{

	};
	
	*/
};

#endif