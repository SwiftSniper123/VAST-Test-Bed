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

//class SimClock
//{
//public:
//	/* timeRatio, timeSlice, endTime*/
//	SimClock(double timeRatio, double timeSlice, double endTime)
//	{
//		_timeRatio = timeRatio;
//		_timeSlice = timeSlice;
//		_endTime = endTime;
//		_simClockTime = 0;
//	};
//
//	void advanceToNextTimeSlice()
//	{
//		// sleep(seconds + seconds - seconds) 
//		_sleep(_timeSlice*1000*_timeRatio);
//		_simClockTime += _timeSlice;
//	}
//
//	double getSimClockTime()
//	{
//		return _simClockTime;
//	};
//private:
//	double _timeRatio;
//	double _timeSlice;
//	double _endTime;
//	double _simClockTime;
//};

TEST(Test_EventTree, EventTreeStartAndStopClock)
{
	
	EventTree* et = new EventTree(0.001, ratio(1.0), 0.001);
	EXPECT_FALSE(et->running());
	EXPECT_EQ(et->getCurrentSimTime(), -1);

	EXPECT_NO_THROW(et->start());
	EXPECT_FALSE(et->running());

	// cleanup
	delete et;
}

TEST(Test_EventTree, ClockTime)
{
	clock_t t;
	t = clock();
	sleep_for(milliseconds(1));
	t = clock() - t;
	float timePassed =((float)t) / CLOCKS_PER_SEC;
	EXPECT_TRUE(timePassed < 0.002);
}

// 4/21/2019 - this is likely not measuring correctly at this time; timePassed says 22 seconds, but the test
// does not physically run for anywhere near that long.
//TEST(Test_EventTree, EventTreeFasterThanRealTime)
//{
//	// set the timeslice to 0.1 s, ratio smallest it can be set 0.01 (one real second per hundred seconds), replication length 1.0 s
//	// this means timeslices actually take 0.1*0.01 = 0.001 s in real time
//	// the replication duration should be 0.01 in real time
//	EventTree* et = new EventTree(0.1, ratio(0.01), 1.0);
//	clock_t t;
//	t = clock();
//	et->start();
//	t = clock() - t;
//	float timePassed =((float)t) / CLOCKS_PER_SEC;
//	
//	EXPECT_TRUE(t > 0.01);
//	EXPECT_EQ(t, 1.0);
//	EXPECT_EQ(CLOCKS_PER_SEC, 1000);
//	delete et;
//}
//	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.1 s on simClock
//	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.1)*10), 1);
//	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.2 s on simClock
//	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.2) * 10), 2);
//	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.3 s on simClock
//	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.3) * 10), 3);
//	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.4 s on simClock
//	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.4) * 10), 4);
//	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.5 s on simClock
//	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.5) * 10), 5);
//	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.6 s on simClock
//	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.6) * 10), 6);
//	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.7 s on simClock
//	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.7) * 10), 7);
//	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.8 s on simClock
//	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.8) * 10), 8);
//	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 0.9 s on simClock
//	EXPECT_EQ(floor((et->getCurrentSimTime() - 0.9) * 10), 9);
//	sleep_for(milliseconds(1)); // wait 1 timeslice to relative time 1.0 s on simClock
//	EXPECT_EQ(floor((et->getCurrentSimTime() - 1.0) * 10), 10);
//
//	// total runtime has been 0.01 s in relative real time
//	EXPECT_TRUE(difftime(time(0), startTime) - 0.01 < 0.000000001);
//
//}
/* Mock up class for use in the EventTreeSortEvents test case below.  Makes use of dataMap,
update, EventTree registration, and a getter to verify that update happened.*/
class MockComponent : public VComponent
{
public:
	MockComponent(string name, dataMap dataMap, bool leadComp)
	{
		_name = name;
		_myMap = dataMap; // copy
		_leadComp = leadComp;
	}

	/* Destructor clears the internal data map*/
	~MockComponent()
	{
		_myMap.clear();
		_genEvents.clear();
	}

	/* Manadatory override of this base class; implement this to receive updates to the _myMap*/
	void update(timestamp time, dataMap updateMap)
	{

		_list = _genEvents.begin();
		dataMap::iterator it;
		for (auto mapIterator = updateMap.begin(); mapIterator != updateMap.end(); ++mapIterator)
		{
			it = _myMap.find(mapIterator->first);
			if (it != _myMap.end())
				it->second = mapIterator->second;
		}
		_numUpdates++;

		feaxConnection(time);
	}

	/* Fake correspondence with the outside process, that queries and waits for a response and then
	turns around that data to sent back through the EventTree through addEvent.*/
	void feaxConnection(timestamp messageTime)
	{
		// processing locally to compose the outgoing query, asking for a response, processing that message
		
		if (_leadComp)
		{
			double replyTime = getEventTree()->getTimeSlice();
			generateEvent(messageTime);
		}
		else
			generateEvent(messageTime);
	};

	/* Allows us to check the updates to _myMap*/
	int getUpdatedValue(string key)
	{
		Integer i = _myMap.at(key);
		return i.value();
	};

	dataMap getDataMap()
	{
		return _myMap;
	}

	void storeEvent(timestamp time, dataMap updateMap)
	{
		_genEvents.emplace(time, updateMap);
	};

	void generateEvent(timestamp now)
	{
		if (_list != _genEvents.end() && getEventTree()->getCurrentSimTime() >= 0.0)
		{
			timestamp record = _list->first;
			if (record < now)
			{
				_genEvents.erase(record);
			}
			else if (record < now + getEventTree()->getTimeSlice())
			{
				dataMap newData = _genEvents[record];
				for (auto updateDataIterator = newData.begin();
					updateDataIterator != newData.end();
					++updateDataIterator)
				{
					// write to the storedEvent data whether the key is found or not
					_myMap[updateDataIterator->first] = updateDataIterator->second;
				}

				_genEvents.erase(record);
				//_list++;
				getEventTree()->addEvent(this, record, newData); // send stored event
			}
			else // there are stored events, but they are not happening yet
			{
				getEventTree()->addEvent(this, now, dataMap()); // for this case, echo empty map
			}
		}
		else // there were no stored events, just echo the component map
		{
			getEventTree()->addEvent(this, now, dataMap());
		}
	}

	VComponent::VCType getVCType() 
	{
		if (_leadComp && _name.compare("av") != 0)
		{
			return Environment_Avatar;
		}
		else
		{
			return AV_Avatar;
		}
	}; 

	string getName()
	{
		return _name;
	};

	int getNumUpdates()
	{
		return _numUpdates;
	}

private:
	dataMap _myMap;
	map<timestamp, dataMap> _genEvents;
	map<timestamp, dataMap>::iterator _list;
	string _name;
	bool _leadComp;
	int _numUpdates = 0;
}; // end of MockComponent declaration/definition

TEST(Test_EventTree, EventTreeAddEvent)
{
	EventTree* et = new EventTree(0.1, ratio(1.0), 1.0);
	dataMap compData;
	compData.emplace("ooo", new String());
	MockComponent* env = new MockComponent("mockComponent", compData, true);
	try
	{
		// clock has not started yet, so the current sim time is -1.0
		EXPECT_EQ(et->getCurrentSimTime(), -1.0);

		// once the components are registered, the EventTree will not start running, will return false
		et->registerComponent(env);
		et->setFirstComponent(env);
		EXPECT_FALSE(et->running());

		//create data for an event to be run when it starts
		dataMap futureData;
		futureData.emplace("ooo", new String("the future"));
		
		// test that bad times throw exceptions 
		EXPECT_THROW(et->addEvent(env, -0.5, futureData), OutOfTimeException);
		EXPECT_THROW(et->addEvent(env, 1.5, futureData), OutOfTimeException);

		// test that good event time does not throw an exception
		EXPECT_NO_THROW(et->addEvent(env, 0.5, futureData));

		// test that a good replication with appropriate event throws no exceptions
		EXPECT_NO_THROW(et->start()); // goes until it stops
		
		// stopped
		EXPECT_FALSE(et->running());

	}
	catch (...)
	{
		delete et;// , env;
		FAIL(); // failed for another reason
	}
	delete et;// , env;
}

TEST(Test_EventTree, EventTreeTwoComponentsAndEvent)
{
	string valueA = "A";
	string valueB = "B";

	// define update maps for later
	dataMap updateMap1; // first updater will update valueA to 1, value B will stay 0
	updateMap1.emplace(valueA, new Integer(1));
	updateMap1.emplace(valueB, new Integer(3));
	EXPECT_EQ(Integer(updateMap1.at(valueA)).value(), 1);
	EXPECT_EQ(Integer(updateMap1.at(valueB)).value(), 3);

	// define the original value 0 map
	dataMap dataMap0;
	dataMap0.emplace(valueA, new Integer()); // initialize arbitrary map data to 0
	dataMap0.emplace(valueB, new Integer()); // initialize arbitrary map data to 0
	EXPECT_EQ(Integer(dataMap0.at(valueA)).value(), 0);
	EXPECT_EQ(Integer(dataMap0.at(valueB)).value(), 0);

	// two different event creators
	MockComponent* environment = new MockComponent("environment", dataMap0, true);
	// first event set for 0.1 will update av
	environment->storeEvent(0.01, updateMap1);

	MockComponent* av = new MockComponent("av", dataMap0, false);

	// now the EventTree can be initialized and can register the 3 components
	EventTree* et = new EventTree(0.1, ratio(1.0), 0.5);
	et->registerComponent(environment);
	et->registerComponent(av);
	//et->setFirstComponent(av);
	try
	{
		// should start with three events in system
		EXPECT_NO_THROW(et->start());

		// verify that values are still 3
		EXPECT_EQ(av->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		EXPECT_EQ(av->getUpdatedValue(valueB), Integer(updateMap1.at(valueB)).value());
	}
	catch (...)
	{
		FAIL(); // failed for another reason
		delete et;// , environment, av;
	}

	//cleanup
	delete et;// , environment, av;
}

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
	MockComponent* environment = new MockComponent("environment", dataMap0, false);
	MockComponent* av = new MockComponent("av", dataMap0, true);

	// now the EventTree can be initialized and can register the 3 components
	EventTree* et = new EventTree(0.1, ratio(1.0), 0.5);
	et->registerComponent(environment);
	et->registerComponent(av);

	et->setFirstComponent(av);
	environment->storeEvent(0.01, updateMap1); // valueA = 1
	av->storeEvent(0.192, updateMap2); //  valueB = 2
	environment->storeEvent(0.4, updateMap3); // value A and B = 3

	try
	{
		
		// should start with three events in system
		EXPECT_NO_THROW(et->start());

		// verify that values are still 3
		EXPECT_EQ(environment->getUpdatedValue(valueA), Integer(updateMap2.at(valueA)).value());
		EXPECT_EQ(environment->getUpdatedValue(valueB), Integer(updateMap2.at(valueB)).value());
		EXPECT_EQ(av->getUpdatedValue(valueA), Integer(updateMap3.at(valueA)).value());
		EXPECT_EQ(av->getUpdatedValue(valueB), Integer(updateMap3.at(valueB)).value());
		
		EXPECT_EQ(environment->getNumUpdates(), 2);
		EXPECT_EQ(av->getNumUpdates(), 2);
	}
	catch (...)
	{
		FAIL(); // failed for another reason
		delete et;//,environment, av;
	}

	//cleanup
	delete et;// , environment, av;
}

TEST(Test_EventTree, EventTreeMultipleRuns)
{
	EventTree* et = new EventTree(0.1, ratio(1.0), 0.5, 2);
	EXPECT_NO_THROW(et->start());
	delete et;
}