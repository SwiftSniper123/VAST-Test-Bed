//Virtual Autonomous System Testbed
//ODU Capstone 2018-2019

#include "gtest/gtest.h"
#include <iostream>
#include <thread> // sleep_for

#include "SumoEnvironment.h"
#include "ProximitySensor.h"
#include "PythonAV.h"
#include "VAST.h"
#include <Windows.h>
#include <gtest/gtest.h>


using namespace VASTConstants;
using namespace std::this_thread;     // sleep_for, sleep_until

using std::cin;
using std::cout;
using std::endl;
using std::chrono::milliseconds;
using std::string;

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
		string fileName = "";
		cout << "Please type the configuration file location and name: ";
		cin >> fileName;

		string dbName = DATABASE_FILENAME;
		//cout << "\nIf desired, provide a database file name (default - VASTDatabase.db): ";
		//cin >> dbName;

		//parse file
		//VAST *v = new VAST(fileName, dbName);
		VAST *v = new VAST("VASTConfig.xml", "test.db");
		cout << "Parsing begins" << endl;
		v->Parse();
		cout << "Parsing ends" << endl;

		////////////////////////////////////////////////////////
		// this needs to get sorted out; all that's left is to get maps from VAST object and give them to the generate functions, then register components
		dataMap* getVASTConfig();
		
		storedMap* avConfig =  get_AVConfig(string name);
		storedMap* get_AVRunData(string name);
		storedMap* get_EnvConfig(string name);
		storedMap* get_EnvRunData(string name);
		

		v->generateAV(avMaps[0], v->_AVs[0]->getDataMap());
		v->generateEnvenvMap(envMap, v->_Env->getDataMap());
		//PythonVehicle *secondAV = new PythonVehicle(avMaps[1], v->_AVs[1]->getDataMap());
		ProximitySensor *proxSensor = new ProximitySensor(firstAV, v->_AVs[0]->getDataMap());

		v->_EventTree->registerComponent(sumo);
		v->_EventTree->registerComponent(firstAV);
		//v->_EventTree->registerComponent(secondAV);
		v->_EventTree->registerComponent(proxSensor);
		v->_EventTree->setFirstComponent(sumo);
		//////////////////////////////////////////////////////////

		sumo->openEnvironment();

		//v->_EventTree->start(); //May start event tree within sumo environment

		//run VAST
		cout << "VAST run begins" << endl;

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

AV* VAST::generateAV(string name, dataMap runData, dataMap configData)
{
	
	PythonVehicle *firstAV = new PythonVehicle(configData, runData);	

	// TODO instantiate sensor and hook it up
	// instantiate scenario metric
}

Environment* VAST::generateEnv(string name, dataMap runData, dataMap configData)
{
	SumoEnvironment *sumo = new SumoEnvironment(configData, runData);
}