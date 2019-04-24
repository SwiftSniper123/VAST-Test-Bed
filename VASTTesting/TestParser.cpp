#include "gtest/gtest.h"
#include "ParseXML.h"
#include "VType.h"

/*Default true test*/
TEST(Test_Parser, TrueInTestVASTLibrary)
{
	ASSERT_TRUE(true);
}

/*Tests base Environment class constructor.*/
TEST(Test_Parser, Constructor)
{
	XMLParser* p = new XMLParser("File");

	ASSERT_NO_FATAL_FAILURE(delete p);
}

TEST(Test_Parser, Parse)
{
	XMLParser* p = new XMLParser("C:/Users/jaron/Documents/test.xml");
	p->Parse();

	//VType test = p->_EnvMap.find("env_obstacle_port");
	//std::cout << p->env->_env_obstacle_port.value();
	//ASSERT_TRUE(p->env->_env_obstacle_port == Integer);
}