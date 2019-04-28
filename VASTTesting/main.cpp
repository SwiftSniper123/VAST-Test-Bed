//Virtual Autonomous System Testbed
//ODU Capstone 2018-2019

#include "gtest/gtest.h"


int main(int argc, char **argv1)
{
	::testing::InitGoogleTest(&argc, argv1);
	RUN_ALL_TESTS();
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