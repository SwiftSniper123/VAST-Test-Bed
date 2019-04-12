#include "gtest/gtest.h"
#include "EventTree.h"
#include "Environment.h"
#include "AV.h"
#include "Sensor.h"
#include "Obstacle.h"
#include "time.h"
#include <chrono> //C++ 14 required; sleep_for()
#include <thread>
#include <iostream>

using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;
using std::cout;

/*Default true test*/
TEST(DefaultTest, TrueInTestEventTree)
{
	ASSERT_TRUE(true);
}

/* EventTreeConstructor - Tests that the EventTree constructors properly create an EventTree and throw the InvalidArgumentException where arguments do not follow the proper input requirements.
Procedure: create EventTrees with improper arguments.
Pass: All expected exceptions were thrown, and all expected allowable inputs did not throw an exception.  Getters retrieve constructor inputs again.
Fail: Exceptions that were expected were not thrown, getters returned the wrong results.
*/
TEST(Test_EventTree, EventTreeConstructor)
{
	EventTree* e;
	// test constructor with a timeslice less than or equal to 0
	ASSERT_THROW(e = new EventTree(0, ratio(0.99), 1.0), InvalidArgumentException);
	ASSERT_THROW(e = new EventTree(-0.1, ratio(0.99), 1.0), InvalidArgumentException);

	// test constructor with a timeRatio less than or equal to 0, or greater than 1.0
	ASSERT_THROW(e = new EventTree(0.1, ratio(0), 1.0), InvalidArgumentException);
	ASSERT_THROW(e = new EventTree(0.1, ratio(-0.1), 1.0), InvalidArgumentException);
	ASSERT_THROW(e = new EventTree(0.1, ratio(1.1), 1.0), InvalidArgumentException);

	// test constructor with a end time less than or equal to 0
	ASSERT_THROW(e = new EventTree(0.1, ratio(0.99), 0.0), InvalidArgumentException);
	ASSERT_THROW(e = new EventTree(0.1, ratio(0.99), -1.0), InvalidArgumentException);
	
	// test special constructor with a databaseTimeUnit not in the approved list:
	//  s, m, h, d, w
	//ASSERT_THROW(e = new EventTree(0.1, ratio(1.0), 1.0, 'a'), InvalidArgumentException);
	//ASSERT_THROW(e = new EventTree(0.1, ratio(1.0), 1.0, 'b'), InvalidArgumentException);

	//// test that correct constructors create the EventTree
	ASSERT_NO_FATAL_FAILURE(e = new EventTree(0.1, ratio(0.99), 1.0));
	ASSERT_NO_FATAL_FAILURE(delete e);
	//ASSERT_NO_FATAL_FAILURE(e = new EventTree(0.1, ratio(0.99), 1.0, 's'));
	//ASSERT_NO_FATAL_FAILURE(delete e);
	//ASSERT_NO_FATAL_FAILURE(e = new EventTree(0.1, ratio(0.99), 1.0, 'm'));
	//ASSERT_NO_FATAL_FAILURE(delete e);
	//ASSERT_NO_FATAL_FAILURE(e = new EventTree(0.1, ratio(0.99), 1.0, 'h'));
	//ASSERT_NO_FATAL_FAILURE(delete e);
	//ASSERT_NO_FATAL_FAILURE(e = new EventTree(0.1, ratio(0.99), 1.0, 'd'));
	//ASSERT_NO_FATAL_FAILURE(delete e);
	//ASSERT_NO_FATAL_FAILURE(e = new EventTree(0.1, ratio(0.99), 1.0, 'w'));
	//ASSERT_NO_FATAL_FAILURE(delete e);

	//test getters retrieve the timeslice, timeratio
	e = new EventTree(0.1, 0.99, 1.0);
	ASSERT_EQ(e->getTimeSlice(), 0.1);
	ASSERT_EQ(e->getTimeRatio(), 0.99);
	ASSERT_EQ(e->getEndSimTime(), 1.0);
	delete e;

	e = new EventTree(0.5, 0.1, 2.0);
	ASSERT_EQ(e->getTimeSlice(), 0.5);
	ASSERT_EQ(e->getTimeRatio(), 0.1);
	ASSERT_EQ(e->getEndSimTime(), 2.0);
	
	// verify that time clock has not started
	ASSERT_EQ(e->getCurrentSimTime(), -1.0);
	delete e;

}

/* EventTreeRegisterComponent - Tests that the EventTree can register the component data maps.
Procedure: create EventTree and verify registration of VComponents.
Pass: All expected exceptions were thrown, and all expected allowable inputs did not throw an exception.  Getters retrieve constructor inputs again.
Fail: Exceptions that were expected were not thrown, getters returned the wrong results.
*/
TEST(Test_EventTree, EventTreeRegisterComponent)
{
	EventTree* et = new EventTree(0.1, ratio(0.99), 1.0);
	// no components are added yet
	ASSERT_EQ(et->getNumberOfVComp(), 0);
	
	Environment* env = new Environment();
	AV* av1 = new AV();
	AV* av2 = new AV();
	Sensor* sensor = new Sensor();
	
	// add one, check the number
	et->registerComponent(env);
	ASSERT_EQ(et->getNumberOfVComp(), 1);

	// verify that adding a component twice does not actually register it
	et->registerComponent(env);
	ASSERT_EQ(et->getNumberOfVComp(), 1);

	et->registerComponent(av1);
	et->registerComponent(av2);
	et->registerComponent(sensor);
	ASSERT_EQ(et->getNumberOfVComp(), 4);

	delete et, env, av1, av2, sensor;
}

class SimClock
{
public:
	/* timeRatio, timeSlice, endTime*/
	SimClock(double timeRatio, double timeSlice, double endTime)
	{
		_timeRatio = timeRatio;
		_timeSlice = timeSlice;
		_endTime = endTime;
		_simClockTime = 0;
	};

	void advanceToNextTimeSlice()
	{
		// sleep(seconds + seconds - seconds) 
		_sleep(_timeSlice*1000*_timeRatio);
		_simClockTime += _timeSlice;
	}

	double getSimClockTime()
	{
		return _simClockTime;
	};
private:
	double _timeRatio;
	double _timeSlice;
	double _endTime;
	double _simClockTime;
};

TEST(Test_EventTree, EventTreeStartAndStopClock)
{
	
	EventTree* et = new EventTree(0.001, ratio(1.0), 0.001);
	EXPECT_FALSE(et->running());
	EXPECT_EQ(et->getCurrentSimTime(), -1);
	time_t startTime = time(0);
	time_t timer;

	et->start();
	EXPECT_TRUE(et->running());
	EXPECT_DOUBLE_EQ(et->getCurrentSimTime(), 0);

	// now push the clock forward indirectly, it should stop
	sleep_for(microseconds(1002)); // wait for just after relative time 0.001
	//EXPECT_DOUBLE_EQ(0.001, et->getCurrentSimTime());
	time(&timer);
	double transpired = difftime(timer, startTime);
	ASSERT_NEAR(0.001002, transpired, 0.000000001);
	EXPECT_FALSE(et->running());
	// try again debugging
	sleep_for(microseconds(1002)); // wait for just after relative time 0.001
	time(&timer);
	transpired = difftime(timer, startTime);
	ASSERT_NEAR(0.002004, transpired, 0.000000001);
	EXPECT_FALSE(et->running());

	// no other events are recorded, they just drop

	//sleep_for(10ns);
	//sleep_until(system_clock::now() + 1s);
	//EventTree* et = new EventTree(0.1, 0.99, 1.0);
	//Environment* env = new Environment();

}

TEST(Test_EventTree, EventTreeFasterThanRealTime)
{
	// set the timeslice to 0.1 s, ratio smallest it can be set 0.01 (one real second per hundred seconds), run length 1.0 s
	// this means timeslices actually take 0.1*0.01 = 0.001 s in real time
	// the run duration should be 0.01 in real time
	EventTree* et = new EventTree(0.1, ratio(0.01), 1.0);
	time_t startTime =time(0);
	et->start();
	EXPECT_TRUE(et->running());
	EXPECT_TRUE(et->getCurrentSimTime() < 0.000000001); // very close to relative time 0.0 s on simClock

	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.1 s on simClock
	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.1)*10), 1);
	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.2 s on simClock
	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.2) * 10), 2);
	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.3 s on simClock
	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.3) * 10), 3);
	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.4 s on simClock
	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.4) * 10), 4);
	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.5 s on simClock
	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.5) * 10), 5);
	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.6 s on simClock
	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.6) * 10), 6);
	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.7 s on simClock
	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.7) * 10), 7);
	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.8 s on simClock
	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.8) * 10), 8);
	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.9 s on simClock
	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.9) * 10), 9);
	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 1.0 s on simClock
	EXPECT_EQ(floor((et->getCurrentSimTime() - 1.0) * 10), 10);

	// total runtime has been 0.01 s in relative real time
	EXPECT_TRUE(difftime(time(0), startTime) - 0.01 < 0.000000001);

}

TEST(Test_EventTree, EventTreeAddEvent)
{
	EventTree* et = new EventTree(0.1, ratio(0.99), 1.0);
	Environment* env = new Environment();
	try
	{
		// clock has not started yet, so the current sim time is -1.0
		EXPECT_EQ(et->getCurrentSimTime(), -1.0);

		// once the components are registered, the EventTree can running, will return true if it can running
		et->registerComponent(env);
		EXPECT_TRUE(et->running());

		// tell the system to wait a bit, then check to see if the EventTree started
		sleep_for(milliseconds(300)); // 300ms -> 0.3 s * 100 sims / 99 s = 0.3030303...sims
		EXPECT_TRUE(et->running());
		EXPECT_TRUE(et->getCurrentSimTime() > 0.30); // expected passage of time

		// Expect that a negative event time throws an exception
		EXPECT_THROW(et->addEvent(env, -0.1, dataMap()), InvalidArgumentException);
		// Expect that an unregistered component throws an exception if it tries to add an event
		EXPECT_THROW(et->addEvent(new Environment(), 0.5, dataMap()), InvalidArgumentException);

		// Expect that a good event does not throw an exception
		EXPECT_NO_THROW(et->addEvent(env, 0.9, dataMap()));
		sleep_for(milliseconds(701)); // wait for 0.701 seconds longer, to go over the end time of 1.0
		EXPECT_FALSE(et->running()); // should  return false because the clock has stopped.
	}
	catch (...)
	{
		delete et, env; 
		FAIL();
	}
	delete et, env;
}

/* Mock up class for use in the EventTreeSortEvents test case below.  Makes use of dataMap, 
update, EventTree registration, and a getter to verify that update happened.*/
class MockComponent: public VComponent
{
public: 
	MockComponent(string name, dataMap dataMap)
	{
		_name = name;
		_myMap = dataMap; // copy
	}

	/* Destructor clears the internal data map*/
	~MockComponent()
	{
		_myMap.clear();
	}

	/* Manadatory override of this base class; implement this to receive updates to the _myMap*/
	void update(timestamp time, dataMap updateMap)
	{
		dataMap::iterator it;
		for (auto mapIterator = updateMap.begin(); mapIterator != updateMap.end(); ++mapIterator)
		{
			it = _myMap.find(mapIterator->first);
			if (it != _myMap.end())
				it->second = mapIterator->second;
		}
	}

	/* Allows us to check the updates to _myMap*/
	int getUpdatedValue(string key)
	{
		Integer i = _myMap.at(key);
		return i.value();
	};

	void registerEventTree(EventTree* et)
	{
		_et = et;
	};

	VComponent::VCType getVCType() { return Test_Avatar; }; // unused here

	string getName()
	{
		return _name;
	};

private:
	dataMap _myMap;
	EventTree* _et;
	string _name;
}; // end of MockComponent declaration/definition

TEST(Test_EventTree, EventTreeSeveralComponentsAndEvents)
{
	string valueA = "A";
	string valueB = "B";

	// define update maps for later
	dataMap updateMap1; // first updater will update valueA to 1, value B will stay 0
	updateMap1.emplace(valueA, new Integer(1)); 
	EXPECT_EQ(Integer(updateMap1.at(valueA)).value(), 1);
	dataMap updateMap2; // second updater will update valueB to 2, value A will stay 1
	updateMap2.emplace(valueB, new Integer(2)); 
	EXPECT_EQ(Integer(updateMap2.at(valueB)).value(), 2);
	dataMap updateMap3; // both valueA and valueB will get updated to 3
	updateMap3.emplace(valueA, new Integer(3)); 
	updateMap3.emplace(valueB, new Integer(3)); 
	EXPECT_EQ(Integer(updateMap3.at(valueA)).value(), 3);
	EXPECT_EQ(Integer(updateMap3.at(valueB)).value(), 3);

	// define the original value 0 map
	dataMap dataMap0;
	dataMap0.emplace(valueA, new Integer()); // initialize arbitrary map data to 0
	dataMap0.emplace(valueB, new Integer()); // initialize arbitrary map data to 0
	EXPECT_EQ(Integer(dataMap0.at(valueA)).value(), 0);
	EXPECT_EQ(Integer(dataMap0.at(valueB)).value(), 0);

	// three different event creators
	MockComponent* env1 = new MockComponent("env1", dataMap0);
	MockComponent* env2 = new MockComponent("env2", dataMap0);
	MockComponent* env3 = new MockComponent("env3", dataMap0);
	
	// now the EventTree can be initialized and can register the 3 components
	EventTree* et = new EventTree(0.1, ratio(1.0), 0.5);
	et->registerComponent(env1);
	et->registerComponent(env2);
	et->registerComponent(env3);

	try
	{
		// clock has not started yet, so the current sim time is -1
		EXPECT_EQ(et->getCurrentSimTime(), -1);

		// once the components are registered, the EventTree can start, running will return true
		et->start();
		EXPECT_TRUE(et->running());
		sleep_for(milliseconds(1)); // wait a bit - close to t = 0

		// first event set for now (or 0.1) will update env2 and env3 with valueA = 1
		double firstEventTime = et->getCurrentSimTime() < et->getTimeSlice() ? 
			et->getCurrentSimTime() : et->getTimeSlice();
		et->addEvent(env1, firstEventTime, updateMap1);
		// check that all components have valueA = 1 and valueB = 0
		EXPECT_EQ(env1->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		EXPECT_EQ(env1->getUpdatedValue(valueB), 0);
		EXPECT_EQ(env2->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		EXPECT_EQ(env2->getUpdatedValue(valueB), 0);
		EXPECT_EQ(env3->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		EXPECT_EQ(env3->getUpdatedValue(valueB), 0);

		sleep_for(milliseconds(190)); // wait around until relative time .191 s, right before timeslice 2

		// second event set for later (0.2) will update env1 and env3 with valueB = 2
		double secondEventTime = 0.192;
		et->addEvent(env2, secondEventTime, updateMap2);
		
		sleep_for(milliseconds(10)); // wait around for relative time .201 s, right after timeslice 2

		// check that all components have valueA = 1 and valueB = 2; this is the new state at timeslice 2
		EXPECT_EQ(env1->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		EXPECT_EQ(env1->getUpdatedValue(valueB), Integer(updateMap2.at(valueB)).value());
		EXPECT_EQ(env2->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		EXPECT_EQ(env2->getUpdatedValue(valueB), Integer(updateMap2.at(valueB)).value());
		EXPECT_EQ(env3->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		EXPECT_EQ(env3->getUpdatedValue(valueB), Integer(updateMap2.at(valueB)).value());

		// third event set for right away, will update all components and all fields with 3 for next time slice
		et->addEvent(env3, et->getCurrentSimTime(), updateMap3); 

		// verify that values remain unchanged because the last event won't update until the next timeslice
		EXPECT_EQ(env1->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		EXPECT_EQ(env1->getUpdatedValue(valueB), Integer(updateMap2.at(valueB)).value());
		EXPECT_EQ(env2->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		EXPECT_EQ(env2->getUpdatedValue(valueB), Integer(updateMap2.at(valueB)).value());
		EXPECT_EQ(env3->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		EXPECT_EQ(env3->getUpdatedValue(valueB), Integer(updateMap2.at(valueB)).value());

		sleep_for(milliseconds(100)); // wait around for relative time .301 s, right after timeslice 3

		// verify that values updated to 3
		EXPECT_EQ(env1->getUpdatedValue(valueA), Integer(updateMap3.at(valueA)).value());
		EXPECT_EQ(env1->getUpdatedValue(valueB), Integer(updateMap3.at(valueB)).value());
		EXPECT_EQ(env2->getUpdatedValue(valueA), Integer(updateMap3.at(valueA)).value());
		EXPECT_EQ(env2->getUpdatedValue(valueB), Integer(updateMap3.at(valueB)).value());
		EXPECT_EQ(env3->getUpdatedValue(valueA), Integer(updateMap3.at(valueA)).value());
		EXPECT_EQ(env3->getUpdatedValue(valueB), Integer(updateMap3.at(valueB)).value());

		// no events, no updates, advance to timeslice 0.4 s
		sleep_for(milliseconds(100)); // relative time .401 s

		// verify that values are still 3
		EXPECT_EQ(env1->getUpdatedValue(valueA), Integer(updateMap3.at(valueA)).value());
		EXPECT_EQ(env1->getUpdatedValue(valueB), Integer(updateMap3.at(valueB)).value());
		EXPECT_EQ(env2->getUpdatedValue(valueA), Integer(updateMap3.at(valueA)).value());
		EXPECT_EQ(env2->getUpdatedValue(valueB), Integer(updateMap3.at(valueB)).value());
		EXPECT_EQ(env3->getUpdatedValue(valueA), Integer(updateMap3.at(valueA)).value());
		EXPECT_EQ(env3->getUpdatedValue(valueB), Integer(updateMap3.at(valueB)).value());

		// put in a "slow" event
		EXPECT_THROW(et->addEvent(env1, 0.01, updateMap1), OutOfTimeException);

		sleep_for(milliseconds(100)); // relative time .501 s
		// simClock will stop
		EXPECT_FALSE(et->running());

	}
	catch (...)
	{
		delete et, env1, env2, env3;
	}
	delete et, env1, env2, env3;
}

TEST(Test_EventTree, EventTreeEarlyStop)
{
	FAIL();
}

TEST(Test_EventTree, EventTreeMultipleRuns)
{
	FAIL();
}