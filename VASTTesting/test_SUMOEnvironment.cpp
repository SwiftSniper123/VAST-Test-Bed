#include "gtest/gtest.h"
#include "VC_HEADERS.h"
#include "SUMOEnvironment.h"

using namespace VASTConstants;
/*Default true test*/
TEST(DefaultTest, TrueInSumoEnvironment)
{
	ASSERT_TRUE(true);
}

/* ConstructorAndDestructor - Tests that ProximitySensor is instantiable and destructable with no thrown exceptions.
Procedure: Create a ProximitySensor and delete it.
Pass: All objects can be created and deleted.
Fail: An object's default constructor or basic destructor contains some exception-throwing error.
*/
TEST(Test_SumoEnvironment, ConstructorAndDestructor)
{
	SumoEnvironment* se;

	// 0 config map
	dataMap envConfig;
	envConfig.emplace(EXE_LOCATION, new String());
	envConfig.emplace(CONFIG_LOCATION, new String());
	envConfig.emplace(ENV_OBSTACLE_PORT, new Integer());
	envConfig.emplace(ENV_BOUNDS, new Vector3());

	// 0 data map
	dataMap envRunData;
	envRunData.emplace(EXE_LOCATION, new String());
	envRunData.emplace(CONFIG_LOCATION, new String());
	envRunData.emplace(ENV_OBSTACLE_PORT, new Integer());
	envRunData.emplace(ENV_BOUNDS, new Vector3());

	//EXPECT_THROW(se = new SUMOEnvironment(nullptr, dataMap()), InvalidArgumentException);
	//EXPECT_THROW(se = new SUMOEnvironment(av, dataMap()), InvalidArgumentException);
	//EXPECT_THROW(se = new SUMOEnvironment(nullptr, sensorConfig), InvalidArgumentException);
	EXPECT_NO_THROW(se = new SumoEnvironment(envConfig, envRunData));

	delete se;
}