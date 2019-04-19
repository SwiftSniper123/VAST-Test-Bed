#include "pch.h"
#include "TraciControlToSumo.h"
#include <string>

TEST(SumoTraciSetupTest, Constructor)
{
	TraciControlToSumo* test = nullptr;
	test = new TraciControlToSumo();
	EXPECT_TRUE(test != nullptr);
	delete test;
}


//This test fails because the user cannot input anything from a unit test
/*TEST(SumoTraciSetupTest, Initialize)
{
	TraciControlToSumo* test = new TraciControlToSumo();
	test->initialize();
	std::string testString;
	testString = test->getSumoCfgString();
	std::cout << testString;
	EXPECT_TRUE(!testString.empty());
	EXPECT_TRUE(test->getRandomFlag());

}*/

TEST(SumoTraciSetupTest, RunSumo)
{
	TraciControlToSumo* test = new TraciControlToSumo();
	test->initialize();
	test->runSumo();
	EXPECT_TRUE(test->getCreateProcessWorked());
}

//This test fails because the user cannot input anything from a unit test
/*TEST(SumoTraciSetupTest, RunClient)
{
	TraciControlToSumo* test = new TraciControlToSumo();
	test->initialize();
	test->runSumo();
	test->runClient()
	.
	.
	.
}*/
