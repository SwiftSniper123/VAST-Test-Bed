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

#include <map>
#include<variant>
#include "VType.h"
#include "AV.h"
#include "SumoEnvironment.h"
#include <Windows.h>
#include "VAST.h"
#include <gtest/gtest.h>

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
		cout << "\n\n=========Scenario Replications=========" << endl;
		//get file location and name
		std::string fileName = "";
		cout << "Please type the configuration file location and name: ";
		cin >> fileName;

		std:string dbName = "";
		cout << "\nIf desired, provide a database file name (default - VASTDatabase.db): ";
		cin >> dbName;

		//parse file
		VAST *v = new VAST(fileName, dbName);
		cout << "Parsing begins" << endl;
		v->Parse();
		cout << "Parsing ends" << endl;

		dataMap envMap = v->_EnvConfig;
		dataMap configMap = v->_ConfigMap;
		
		SumoEnvironment *sumo = new SumoEnvironment(envMap["config_location"]->s_value(), envMap["exe_location"]->s_value(), Integer(envMap["env_obstacle_port"]), Vector3(envMap["Env_bounds"]));

		for (int i = 0; i < v->_AVs.size(); i++)
		{
			sumo->addAV(v->_AVs[i]);
		}		

		v->_EventTree->registerComponent(sumo);
		v->_EventTree->setFirstComponent(sumo);
		v->_EventTree->start();

		//run VAST
		cout << "VAST run begins" << endl;
		//v->


		cout << "VAST run ends" << endl;
	}
	else if (mode == 2)
	{
		// Unit Tests
		cout << "\n\n=========Tests=========" << endl;
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