#include "gtest/gtest.h"
#include "VC_HEADERS.h"
#include "time.h"
#include <chrono> //C++ 14 required; sleep_for()
#include <thread>
#include <iostream>

static const string testDatabaseName = "DeleteThisDatabase.db";

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
	ASSERT_THROW(e = new EventTree(0, ratio(0.99), 1.0, testDatabaseName), InvalidArgumentException);
	ASSERT_THROW(e = new EventTree(-0.1, ratio(0.99), 1.0, testDatabaseName), InvalidArgumentException);

	// test constructor with a timeRatio less than or equal to 0, or greater than 1.0
	ASSERT_THROW(e = new EventTree(0.1, ratio(0), 1.0, testDatabaseName), InvalidArgumentException);
	ASSERT_THROW(e = new EventTree(0.1, ratio(-0.1), 1.0, testDatabaseName), InvalidArgumentException);
	ASSERT_THROW(e = new EventTree(0.1, ratio(1.1), 1.0, testDatabaseName), InvalidArgumentException);

	// test constructor with a end time less than or equal to 0
	ASSERT_THROW(e = new EventTree(0.1, ratio(0.99), 0.0, testDatabaseName), InvalidArgumentException);
	ASSERT_THROW(e = new EventTree(0.1, ratio(0.99), -1.0, testDatabaseName), InvalidArgumentException);

	//// test that correct constructors create the EventTree
	ASSERT_NO_FATAL_FAILURE(e = new EventTree(0.1, ratio(0.99), 1.0, testDatabaseName));
	ASSERT_NO_FATAL_FAILURE(delete e);
	remove(testDatabaseName.c_str());

	//test getters retrieve the timeslice, timeratio
	e = new EventTree(0.1, 0.99, 1.0, testDatabaseName);
	ASSERT_EQ(e->getTimeSlice(), 0.1);
	ASSERT_EQ(e->getTimeRatio(), 0.99);
	ASSERT_EQ(e->getEndSimTime(), 1.0);
	delete e;
	remove(testDatabaseName.c_str());

	e = new EventTree(0.5, 0.1, 2.0, testDatabaseName);
	ASSERT_EQ(e->getTimeSlice(), 0.5);
	ASSERT_EQ(e->getTimeRatio(), 0.1);
	ASSERT_EQ(e->getEndSimTime(), 2.0);
	
	// verify that time clock has not started
	ASSERT_EQ(e->getCurrentSimTime(), -1.0);
	delete e;
	remove(testDatabaseName.c_str());
}

/* EventTreeRegisterComponent - Tests that the EventTree can register the component data maps.
Procedure: create EventTree and verify registration of VComponents.
Pass: All expected exceptions were thrown, and all expected allowable inputs did not throw an exception.  Getters retrieve constructor inputs again.
Fail: Exceptions that were expected were not thrown, getters returned the wrong results.
*/
TEST(Test_EventTree, EventTreeRegisterComponent)
{
	EventTree* et = new EventTree(0.1, ratio(0.99), 1.0, testDatabaseName);
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
	remove(testDatabaseName.c_str());
}

class TestMetric : public ScenarioMetric
{
public:
	TestMetric() {};
	void update(timestamp time, dataMap metricData) {};
	void stopReplication(bool another, string runID) {};
	VCType getVCType() 	{ return VCType::Test_Avatar; }
};

/*
TEST(Test_EventTree, EventTreeStartAndStopClock)
{
	dataMap compData;
	compData.emplace("ooo", new String());
	AV* av = new AV("av", compData);
	TestMetric* sm = new TestMetric();
	EventTree* et = new EventTree(0.001, ratio(1.0), 0.001, testDatabaseName);
	et->registerComponent(av);
	et->registerMetric(sm);
	EXPECT_FALSE(et->running());
	EXPECT_EQ(et->getCurrentSimTime(), -1);

	
	EXPECT_NO_THROW(et->start());
	EXPECT_FALSE(et->running());


	// cleanup
	delete et;
	remove(testDatabaseName.c_str());
}

/* Mock-up class for use in test cases below.  Makes use of dataMap,
update, EventTree registration, and a getter to verify that update happened.*/
class MockComponent : public VComponent
{
public:
	MockComponent(string name, dataMap dataMap, bool leadComp)
	{
		_name = name;
		_myMap = dataMap; 
		_initialMap = _myMap; // copy
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
			{
				it->second = mapIterator->second;
				_lastUpdateTime = time;
			}
		}
		_numUpdates++;

		feaxConnection(time);
	}

	void stopReplication(bool another, string runID)
	{
		if (another)
		{
			_numUpdates = 0;
			_genEvents = _initialEvents;
			_myMap = _initialMap;
			sleep_for(milliseconds(10)); // some time for restarting outside process
		}
	}

	/* Fake correspondence with the outside process, that queries and waits for a response and then
	turns around that data to sent back through the EventTree through addEvent.*/
	void feaxConnection(timestamp messageTime)
	{
		// processing locally to compose the outgoing query, asking for a response, processing that message
		
		if (_leadComp)
		{
			double replyTime = getEventTree()->getTimeSlice();
			generateEvent(messageTime + replyTime);
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
		_initialEvents.emplace(time, updateMap);
	};

	void generateEvent(timestamp now)
	{
		if (_list != _genEvents.end() && getEventTree()->getCurrentSimTime() >= 0.0)
		{
			timestamp record = _list->first;
			if (record < now)
			{
				_genEvents.erase(record);
				//_list++;
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

	timestamp getLastUpdateTime()
	{
		return _lastUpdateTime;
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
	dataMap _initialMap; // for resetting
	map<timestamp, dataMap> _genEvents;
	map<timestamp, dataMap> _initialEvents;
	map<timestamp, dataMap>::iterator _list;
	string _name;
	bool _leadComp;
	int _numUpdates = 0;
	timestamp _lastUpdateTime;
}; // end of MockComponent declaration/definition

/*
TEST(Test_EventTree, EventTreeAddEvent)
{
	EventTree* et = new EventTree(0.1, ratio(1.0), 1.0, testDatabaseName);
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
		
		// test that bad times throw exceptions; they are private so no access from here
		EXPECT_THROW(et->addEvent(env, -0.5, futureData), exception);
		//EXPECT_THROW(et->addEvent(env, 1.5, futureData), OutOfTimeException);

		// test that good event time does not throw an exception
		EXPECT_NO_THROW(et->addEvent(env, 0.5, futureData));

		// test that a good replication with appropriate event throws no exceptions
		EXPECT_NO_THROW(et->start()); // goes until it stops
		
		// stopped
		EXPECT_FALSE(et->running());

	}
	catch (...)
	{
		delete et;//, env;
		FAIL(); // failed for another reason
		remove(testDatabaseName.c_str());
	}
	delete et;// , env;
	remove(testDatabaseName.c_str());
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
	environment->storeEvent(0.11, updateMap1);

	MockComponent* av = new MockComponent("av", dataMap0, false);

	// now the EventTree can be initialized and can register the 3 components
	EventTree* et = new EventTree(0.1, ratio(1.0), 0.5, "");
	et->registerComponent(environment);
	et->registerComponent(av);
	et->setFirstComponent(av);
	try
	{
		// should start with three events in system
		EXPECT_NO_THROW(et->start());

		// verify that values are still 3
		EXPECT_EQ(av->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		EXPECT_EQ(av->getUpdatedValue(valueB), Integer(updateMap1.at(valueB)).value());
		EXPECT_EQ(av->getNumUpdates(), 5);
		EXPECT_EQ(av->getLastUpdateTime(), 0.11);
	}
	catch (...)
	{
		FAIL(); // failed for another reason
		delete et;// , environment, av;
		remove(testDatabaseName.c_str());
	}

	//cleanup
	delete et;// , environment, av;
	remove(testDatabaseName.c_str());
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
	EventTree* et = new EventTree(0.1, ratio(1.0), 0.5, testDatabaseName);
	et->registerComponent(environment);
	et->registerComponent(av);

	et->setFirstComponent(av);
	environment->storeEvent(0.1, updateMap1); // valueA = 1
	av->storeEvent(0.2, updateMap2); //  valueB = 2
	environment->storeEvent(0.4, updateMap3); // value A and B = 3

	try
	{
		
		// should start with three events in system
		EXPECT_NO_THROW(et->start());

		// verify that values are still 3
		EXPECT_EQ(environment->getUpdatedValue(valueA), Integer(updateMap3.at(valueA)).value());
		EXPECT_EQ(environment->getUpdatedValue(valueB), Integer(updateMap3.at(valueB)).value());
		EXPECT_EQ(av->getUpdatedValue(valueA), Integer(updateMap3.at(valueA)).value());
		EXPECT_EQ(av->getUpdatedValue(valueB), Integer(updateMap3.at(valueB)).value());
		
		EXPECT_EQ(environment->getNumUpdates(), 4);
		EXPECT_EQ(av->getNumUpdates(),5);
	}
	catch (...)
	{
		FAIL(); // failed for another reason
		delete et;//,environment, av;
		remove(testDatabaseName.c_str());
	}

	//cleanup
	delete et;// , environment, av;
	remove(testDatabaseName.c_str());
}

TEST(Test_EventTree, EventTreeMultipleRuns)
{
	// simple replications
	EventTree* et = new EventTree(0.1, ratio(1.0), 0.5, 10, testDatabaseName);
	EXPECT_NO_THROW(et->start());
	delete et;

	string valueA = "A";

	// define update maps for later
	dataMap updateMap1; // first updater will update valueA to 1, value B will stay 0
	updateMap1.emplace(valueA, new Integer(1));
	EXPECT_EQ(Integer(updateMap1.at(valueA)).value(), 1);

	// define the original value 0 map
	dataMap dataMap0;
	dataMap0.emplace(valueA, new Integer()); // initialize arbitrary map data to 0
	EXPECT_EQ(Integer(dataMap0.at(valueA)).value(), 0);
	
	// three different event creators
	MockComponent* environment = new MockComponent("environment", dataMap0, false);
	MockComponent* av = new MockComponent("av", dataMap0, true);

	et = new EventTree(0.1, ratio(1.0), 0.5, 10, "");
	et->registerComponent(environment);
	et->registerComponent(av);

	et->setFirstComponent(av);
	environment->storeEvent(0.1, updateMap1);

	try
	{
		// should start with three events in system
		EXPECT_NO_THROW(et->start());

		EXPECT_EQ(environment->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		EXPECT_EQ(av->getUpdatedValue(valueA), Integer(updateMap1.at(valueA)).value());
		
		EXPECT_EQ(environment->getNumUpdates(), 4);
		EXPECT_EQ(av->getNumUpdates(), 5);

		EXPECT_EQ(et->getCurrentSimTime(), -1);
	}
	catch (...)
	{
		FAIL(); // failed for another reason
		delete et;
		remove(testDatabaseName.c_str());
	}

	//cleanup
	delete et;
	remove(testDatabaseName.c_str());
}
*/