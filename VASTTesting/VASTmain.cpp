//Virtual Autonomous System Testbed
//ODU Capstone 2018-2019

#include "VComponent.h"
#include <iostream>

using std::cout;
using std::cin;

void RunIntegrationTests();

int main(int argc, char **argv1)
{
	// Establish a integration testing mode, apart from primary VAST operation
	if (string(argv1[1]).compare("TEST") == 0)
	{
		cout << "Running in test mode." << endl;
		// Run integration tests here
		RunIntegrationTests();
	}
	//else if (string(argv1[1]).compare("DIAGNOSTIC") == 0)
	//{
	//	// run VAST in normal mode
	//}
	return 0;
}

void RunIntegrationTests()
{
	// SUMO test

	// other 

	// other other
}