#include "gtest/gtest.h"
#include "ProximitySensor.h"

/*Default true test*/
TEST(DefaultTest, TrueInProximitySensor)
{
	ASSERT_TRUE(true);
}

/* ConstructorAndDestructor - Tests that ProximitySensor is instantiable and destructable with no thrown exceptions.
Procedure: Create a ProximitySensor and delete it.
Pass: All objects can be created and deleted.
Fail: An object's default constructor or basic destructor contains some exception-throwing error.
*/
TEST(Test_ProximitySensor, ConstructorAndDestructor)
{
	ProximitySensor* ps;
	AV* av = new AV();
	dataMap sensorConfig;
	
	sensorConfig.emplace(SENSOR_LOC, new Vector3(0, 0, 0));
	sensorConfig.emplace(SENSOR_QUAD, new Integer(1));
	sensorConfig.emplace(SENSOR_DEPTH, new Double(50));
	
	EXPECT_THROW(ps = new ProximitySensor(nullptr, dataMap()), InvalidArgumentException);
	EXPECT_THROW(ps = new ProximitySensor(av, dataMap()), InvalidArgumentException);
	EXPECT_THROW(ps = new ProximitySensor(nullptr, sensorConfig), InvalidArgumentException);
	EXPECT_NO_THROW(ps = new ProximitySensor(av, sensorConfig));

	delete ps, av;
}

/* UpdateVsAVUpdate - Tests that ProximitySensor does nothing when update is called, but does perform under a call to AVUpdate.
Procedure: Create one ProximitySensor, call update and check getters.  Then call AVUpdate and check getters.
Pass: The AVUpdate function is the only one that causes a change to data.
Fail: AVUpdate is not sole modifier of ProximitySensor data.
*/
TEST(Test_ProximitySensor, UpdateVsAVUpdate)
{
	AV* av = new AV();
	dataMap sensorConfig;
	Vector3* psLoc = new Vector3(0, 0, 0);
	sensorConfig.emplace(SENSOR_LOC, psLoc);
	sensorConfig.emplace(SENSOR_QUAD, new Integer(4)); // pos x, neg y
	sensorConfig.emplace(SENSOR_DEPTH, new Double(50));
	ProximitySensor* ps = new ProximitySensor(av, sensorConfig);

	// test that nothing is set on these pieces of data
	EXPECT_EQ(ps->getClosestObstacle(), nullptr);
	EXPECT_EQ(ps->getClosestProximity(), nullptr);
	EXPECT_EQ(ps->getObstaclesList(), nullptr);

	// create update information
	dataMap updateMap;
	updateMap.emplace(SENSOR_QUAD, new Integer(1));
	updateMap.emplace(OBSTACLE_IDS, new Array("o1,o2,o3"));
	Vector3* pos_o1 = new Vector3(2.0, -1.0, 0.0);
	Vector3* pos_o2 = new Vector3(4.0, -111.0, 0.0);
	Vector3* pos_o3 = new Vector3(-2.0, 4.0, 0.0);
	vector<VType*> positions;
	positions.push_back(pos_o1);
	positions.push_back(pos_o2);
	positions.push_back(pos_o3);
	updateMap.emplace(OBSTACLE_POS, new Array(positions));

	// test that update call does not update data in ProximitySensor
	ps->update(0.0, updateMap);
	EXPECT_EQ(ps->getClosestObstacle(), nullptr);
	EXPECT_EQ(ps->getClosestProximity(), nullptr);
	EXPECT_EQ(ps->getObstaclesList(), nullptr);

	// test that AVUpdate call DOES update the data in the ProximitySensor
	ps->AVUpdate(0.0, updateMap);
	EXPECT_TRUE(ps->getClosestObstacle() != nullptr);
	EXPECT_TRUE(ps->getClosestProximity() != nullptr);
	EXPECT_TRUE(ps->getObstaclesList() != nullptr);

	double dist_o1 = sqrt(pow(psLoc->x() - pos_o1->x(), 2) + pow(psLoc->y() - pos_o1->y(), 2));
	EXPECT_TRUE(ps->getClosestObstacle()->first->value() == "o1");
	EXPECT_EQ(ps->getClosestObstacle()->second->value(), pos_o1->value());
	EXPECT_TRUE(abs(ps->getClosestProximity()->value() - dist_o1) < 0.000001);
	EXPECT_EQ(ps->getObstaclesList()->s_value(), "o1,o2,o3");

	// cleanup
	positions.clear();
	updateMap.clear();
	sensorConfig.clear();
	delete av, ps;
}

/* Test class to demonstrate the interaction with the AV avatar.*/
class ProximityTestAV : public AV
{

public:
	ProximityTestAV(dataMap dataMap) : AV("ProximityTestAV", dataMap)
	{
		
	}

	void update(timestamp time, dataMap updateMap) {};

	void sensorReporting(string key, VType* data)
	{
		_dataMap.emplace(key, data);
	};

	String* getClosestIDforTest()
	{
		String* reply;
		if(_dataMap.count(CLOSEST_ID) > 0)
			reply = (String*)_dataMap.at(CLOSEST_ID);
		else
		{
			reply = nullptr;
		}
			return reply;
	}

	Double* getClosestDistanceForTest()
	{
		Double* reply;
		if (_dataMap.count(CLOSEST_DIST) > 0)
			reply = (Double*)_dataMap.at(CLOSEST_DIST);
		else
		{
			reply = nullptr;
		}
		return reply;
	}
};

/* ReportToAV - Tests that ProximitySensor sends a report to the AV that updates the AV map upon receipt, without the update system.
Procedure: Create one ProximitySensor and one ProximityTestAV, call update and check getters.  Then call reportToAV and check getters.
Pass: The reportToAV function causes a change to AV data.
Fail: The function reportToAV is not modifying AV data.
*/
TEST(Test_ProximitySensor, ReportToAV)
{
	// set up AV
	dataMap updateMap;
	updateMap.emplace(OBSTACLE_IDS, new Array("o1,o2,o3"));
	Vector3* pos_o1 = new Vector3(3.0, -4.0, 0.0);
	Vector3* pos_o2 = new Vector3(4.0, -111.0, 0.0);
	Vector3* pos_o3 = new Vector3(-2.0, 4.0, 0.0);
	vector<VType*> positions;
	positions.push_back(pos_o1);
	positions.push_back(pos_o2);
	positions.push_back(pos_o3);
	updateMap.emplace(OBSTACLE_POS, new Array(positions));
	ProximityTestAV* av = new ProximityTestAV(updateMap);
	
	// set up PS
	dataMap sensorConfig;
	sensorConfig.emplace(SENSOR_LOC, new Vector3(0, 0, 0));
	sensorConfig.emplace(SENSOR_DEPTH, new Double(50));
	sensorConfig.emplace(SENSOR_QUAD, new Integer(4)); // pos x, neg y
	ProximitySensor* ps = new ProximitySensor(av, sensorConfig);
	
	// initial AV values for closest ID and Distance are nullptr
	EXPECT_EQ(av->getClosestIDforTest(), nullptr);
	EXPECT_EQ(av->getClosestDistanceForTest(), nullptr);

	// test that sensor is reporting to AV what closest dimension and obstacle id are
	ps->AVUpdate(0.0, updateMap);
	EXPECT_TRUE(av->getClosestIDforTest() != nullptr);
	EXPECT_TRUE(av->getClosestDistanceForTest() != nullptr);

	EXPECT_EQ(av->getClosestIDforTest()->value(), "o1");
	EXPECT_EQ(av->getClosestDistanceForTest()->value(), 5.0);

	// cleanup
	updateMap.clear();
	sensorConfig.clear();
	delete av, ps;
}

/* StopReplication - Tests that ProximitySensor resets data upon a call to stopReplication.
Procedure: Create one ProximitySensor, update, call getters.  Then call stopReplication and check getters.
Pass: The stopReplication function reset the relevant data.
Fail: stopReplication did not reset the data as predicted.
*/
TEST(Test_ProximitySensor, StopReplication)
{
	AV* av = new AV();
	dataMap sensorConfig;
	Vector3* psLoc = new Vector3(0, 0, 0);
	sensorConfig.emplace(SENSOR_LOC, psLoc);
	sensorConfig.emplace(SENSOR_QUAD, new Integer(4)); // pos x, neg y
	sensorConfig.emplace(SENSOR_DEPTH, new Double(50));
	ProximitySensor* ps = new ProximitySensor(av, sensorConfig);

	// create update information
	dataMap updateMap;
	updateMap.emplace(SENSOR_QUAD, new Integer(1));
	updateMap.emplace(OBSTACLE_IDS, new Array("o1,o2,o3"));
	Vector3* pos_o1 = new Vector3(2.0, -1.0, 0.0);
	Vector3* pos_o2 = new Vector3(4.0, -111.0, 0.0);
	Vector3* pos_o3 = new Vector3(-2.0, 4.0, 0.0);
	vector<VType*> positions;
	positions.push_back(pos_o1);
	positions.push_back(pos_o2);
	positions.push_back(pos_o3);
	updateMap.emplace(OBSTACLE_POS, new Array(positions));
	// call update
	ps->AVUpdate(0.0, updateMap);
	// verify that data changed
	EXPECT_TRUE(ps->getClosestObstacle() != nullptr);
	EXPECT_TRUE(ps->getClosestProximity() != nullptr);
	EXPECT_TRUE(ps->getObstaclesList() != nullptr);

	ps->stopReplication(true, "2");

	// test that stopReplication set relevant data back to nullptr
	EXPECT_EQ(ps->getClosestObstacle(), nullptr);
	EXPECT_EQ(ps->getClosestProximity(), nullptr);
	EXPECT_EQ(ps->getObstaclesList(), nullptr);
	// and did not set back other persistent data
	EXPECT_TRUE(!ps->getDataMap().empty());
	EXPECT_TRUE(ps->getDataMap().size() == sensorConfig.size());
	EXPECT_EQ(sensorConfig.at(SENSOR_LOC)->s_value(), psLoc->s_value());
	EXPECT_EQ(sensorConfig.at(SENSOR_QUAD)->s_value(), "4"); // pos x, neg y
	EXPECT_EQ(sensorConfig.at(SENSOR_DEPTH)->s_value(), "50");

	// cleanup
	positions.clear();
	updateMap.clear();
	sensorConfig.clear();
	delete av, ps;
}