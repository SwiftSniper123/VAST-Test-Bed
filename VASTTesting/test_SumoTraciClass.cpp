#include "pch.h"
#include "../VAST/h/TraciControlToSumo.h"

TEST(SumoTraciSetupTest, Constructor)
{
	TraciControlToSumo* test = nullptr;
	test = new TraciControlToSumo();
	EXPECT_TRUE(test != nullptr);
	delete test;
}

TEST(SumoTraciSetupTest, Destructor)
{

}

TEST(SumoTraciSetupTest, Initialize)
{}

TEST(SumoTraciSetupTest, RunSumo)
{}

TEST(SumoTraciSetupTest, RunClient)
{}
