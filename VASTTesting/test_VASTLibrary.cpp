#include "gtest/gtest.h"
#include "VC_HEADERS.h"

/*Default true test*/
TEST(DefaultTest, TrueInTestVASTLibrary)
{
	ASSERT_TRUE(true);
}

/* ConstructorAndDestructor - Tests that each base class is instantiable and destructable with no thrown exceptions.
Procedure: Create one of each extended VComponent and delete them.
Pass: All objects can be created and deleted.
Fail: An object's default constructor or basic destructor contains some exception-throwing error.
*/
TEST(Test_VASTLibrary, ConstructorAndDestructor)
{
	AV* av; 
	Environment* e;
	Obstacle* o;
	Sensor* s;	
	
	// Test that the object type can be instantiated.
	ASSERT_NO_FATAL_FAILURE(av = new AV());
	ASSERT_NO_FATAL_FAILURE(e = new Environment());
	ASSERT_NO_FATAL_FAILURE(o = new Obstacle());
	ASSERT_NO_FATAL_FAILURE(s = new Sensor());

	// Test that the object can be destroyed.
	ASSERT_NO_FATAL_FAILURE(delete av);
	ASSERT_NO_FATAL_FAILURE(delete e);
	ASSERT_NO_FATAL_FAILURE(delete o);
	ASSERT_NO_FATAL_FAILURE(delete s);
}

/* DataSettingConstructor - Tests that each base class can instantiate with the data constructor, with no thrown exceptions.
Procedure: Create one of each extended VComponent with the data constructor and give them test parameters.
Pass: All objects can be created, verified, and deleted.
Fail: An object's data constructor or getter functions contain some exception-throwing error.
*/
TEST(Test_VASTLibrary, DataSettingConstructor)
{
	AV* av;
	Environment* e;
	Obstacle* o;
	Sensor* s;	
	
	// test name that is not default component name
	string testName = "TestDataSettingConstructor";

	// test data that is not the default empty map
	dataMap testData;
	testData.emplace("1", new String("test data"));
	
	// create 
	/* Test that the object type can be instantiated.*/
	ASSERT_NO_FATAL_FAILURE(av = new AV(testName, testData));
	ASSERT_NO_FATAL_FAILURE(e = new Environment(testName, testData));
	ASSERT_NO_FATAL_FAILURE(o = new Obstacle(testName, testData));
	ASSERT_NO_FATAL_FAILURE(s = new Sensor(testName, testData));

	// verify name
	EXPECT_EQ(av->getName(), testName + "AV");
	EXPECT_EQ(e->getName(), testName + "Environment");
	EXPECT_EQ(o->getName(), testName + "Obstacle");
	EXPECT_EQ(s->getName(), testName + "Sensor");

	// verify data
	EXPECT_EQ(av->getDataMap(), testData);
	EXPECT_EQ(e->getDataMap(), testData);
	EXPECT_EQ(o->getDataMap(), testData);
	EXPECT_EQ(s->getDataMap(), testData);

	// delete
	delete av;
	delete e;
	delete o;
	delete s;

	// original map was not deleted because a copy was given to each object
	EXPECT_EQ(testData["1"]->s_value(), "test data");
	testData.clear();
}

/* GetVTComponentType - Tests that each base class knows what type it is.
Procedure: Create one of each extended VComponent and check the VCType.
Pass: All objects are identifiable by VCType.
Fail: An object's VCType is different that anticipated.
*/
TEST(Test_VASTLibrary, GetVTComponentType)
{
	VComponent::VCType;
	AV* av = new AV();
	Environment* e = new Environment();
	Obstacle* o = new Obstacle();
	Sensor* s = new Sensor();

	// test that the proper enum type is assigned to the base class
	EXPECT_EQ(av->getVCType(), VComponent::VCType::AV_Avatar);
	EXPECT_EQ(e->getVCType(), VComponent::VCType::Environment_Avatar);
	EXPECT_EQ(o->getVCType(), VComponent::VCType::Obstacle_Avatar);
	EXPECT_EQ(s->getVCType(), VComponent::VCType::Sensor_Avatar);

	// cleanup
	delete av;
	delete e;
	delete o;
	delete s;
}
