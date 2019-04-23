#include "gtest/gtest.h"
#include "ParseXML.h"

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
	XMLParser* p = new XMLParser("C:/Users/jaron/Downloads/test.xml");
	p->Parse();
}