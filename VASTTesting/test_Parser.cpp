#include "gtest/gtest.h"
#include "ParseXML.h"
#include "VType.h"

/*Default true test*/
TEST(Test_Parser, TrueInTestVASTLibrary)
{
	ASSERT_TRUE(true);
}

VAST *p = new VAST("C:/Users/jaron/Documents/test.xml");

/*Tests base VAST class constructor.*/
TEST(Test_Parser, Constructor)
{
	VAST *v;
	ASSERT_NO_FATAL_FAILURE(v = new VAST());

	ASSERT_NO_FATAL_FAILURE(delete v);
}


TEST(Test_Parser, Parse)
{
	p->Parse();

	//ASSERT_NO_FATAL_FAILURE(p->_AV.at(0))

	//VType test = p->_EnvMap.find("env_obstacle_port");
	//std::cout << p->env->_env_obstacle_port.value();
	//ASSERT_TRUE(p->env->_env_obstacle_port == Integer);
}