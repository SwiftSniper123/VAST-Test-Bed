#include "gtest/gtest.h"
#include "VAST.h"

using namespace VASTConstants;
/*Default true test*/
TEST(Test_Parser, TrueInTestVASTLibrary)
{
	ASSERT_TRUE(true);
}

/*Tests base VAST class constructor.*/
TEST(Test_Parser, Constructor)
{
	VAST *v;
	ASSERT_NO_FATAL_FAILURE(v = new VAST("VASTConfig.xml", TEST_DATABASE_FILENAME));

	ASSERT_NO_FATAL_FAILURE(delete v);
}

//Verifies Parsing is constructing the AVs and Environments properly
TEST(Test_Parser, Parse)
{
	VAST *v;
	ASSERT_NO_THROW(v->Parse());

	// Populate datamap based on results from parsing
	// Commpare value to verify the configuration map was filled properly
	//dataMap env = p->_Env->getDataMap();
	//ASSERT_TRUE(env.size() > 0);
	//ASSERT_TRUE(Integer(env.at("env_obstacle_port")).value() == 12345);

	//ASSERT_NO_FATAL_FAILURE(p->_AV.at(0))

	//VType test = p->_EnvRun_Data.find("env_obstacle_port");
	//std::cout << p->env->_env_obstacle_port.value();
	//ASSERT_TRUE(p->env->_env_obstacle_port == Integer);
}