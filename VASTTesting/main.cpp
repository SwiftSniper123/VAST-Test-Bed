//Virtual Autonomous System Testbed
//ODU Capstone 2018-2019

#include "gtest/gtest.h"
#include <iostream>
#include <thread> // sleep_for
using namespace std::this_thread;     // sleep_for, sleep_until

using std::cin;
using std::cout;
using std::endl;
using std::chrono::milliseconds;


int main(int argc, char **argv1)
{
	// ask user for config file

	// ask user if VAST should be run for scenario replications or for testing
	int mode;
	cout << "Run VAST in which of the following modes: \n"
		<< " 1 - Scenario Replications\n"
		<< " 2 - Tests" << endl;
	cin >> mode;
	
	if (mode == 1)
	{
		// build components
	}
	else if (mode == 2)
	{
		// Unit Tests
		cout << "\n\n=========Testing=========" << endl;
		::testing::InitGoogleTest(&argc, argv1);
		RUN_ALL_TESTS();
		cout << "Test output file location: VASTTesting\\testOutput\\VAST_tests.xml" << endl;
		// Integration Tests

	}
	else
	{
		cout << "Entered " << mode << " which is not an option." << endl;
		sleep_for(milliseconds(1500));
	}
	return 0;
}

/*
void sumoConnection() {
	Client client;
	client.connect("localhost", 1337);
	std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	std::cout << "run 5 steps ...\n";
	client.simulationStep(5 * 1000);
	std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	client.close();
} */