#include "gtest/gtest.h"
#include "Environment.h"

/*Default true test*/
TEST(DefaultTest, TrueInTestVASTLibrary)
{
	ASSERT_TRUE(true);
}

/*Tests base Environment class constructor.*/
TEST(Test_Environment, Constructor)
{
	Environment* e = new Environment();

	ASSERT_NO_FATAL_FAILURE(delete e);
}
