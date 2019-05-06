//Virtual Autonomous System Testbed
//ODU Capstone 2018-2019

#include "gtest/gtest.h"
#include <iostream>
#include <thread> // sleep_for
#include <Windows.h>
#include <gtest/gtest.h>

#include "SumoEnvironment.h"
#include "ProximitySensor.h"
#include "PythonAV.h"
//#include "AverageSpeed.h" // waiting for Christine's classes emf 5/5/2019
//#include "AverageAcceleration.h"
//#include "AverageDeacceleration.h"
//#include "MaximumAcceleration.h"
//#include "MinimumAcceleration.h"
#include "VAST.h"

using namespace VASTConstants;
using namespace std::this_thread;     // sleep_for, sleep_until

using std::cin;
using std::cout;
using std::endl;
using std::chrono::milliseconds;
using std::string;

void unit_test(int argc, char **argv1);

int main(int argc, char **argv1)
{
	// ask user for config file
	//get file location and name
	string fileName = "";
	cout << "Please type the configuration file location and name: ";
	cin >> fileName;
	cin.clear();

	string dbName;
	cout << "\nIf desired, provide a database file name (Press enter to keep the default - VASTDatabase.db): ";
	cin >> dbName;
	cin.clear();

	//parse file
	//VAST *v = new VAST(fileName, dbName);
	VAST *v = new VAST(fileName, dbName);
	cout << "Parsing begins...";
	v->Parse();
	cout << " parsing finished." << endl << endl;

	// ask user if VAST should be run for scenario replications or for testing
	int mode;
	cout << "Run VAST in which of the following modes: \n\n"
		<<  "  1 - Scenario Replications\n\n"
		<<  "  2 - Verification Testing of VAST System\n\n" 
		<<  "  Mode: ";
	cin >> mode;
	
	if (mode == 1)
	{
		cout << "\n\n===========Scenario Replications===========" << endl;		
		v->Register();				
		
		//run VAST
		cout << "VAST scenario replication run begins..." << endl;
		v->getEventTree()->start();
		cout << "VAST scenario run ends with " << v->getEventTree()->getNumberOfEarlyStops() << " failures." << endl;
	}
	else if (mode == 2)
	{
		cout << "\n\n====Verification Testing of VAST System====" << endl;
		// Unit Tests
		unit_test(argc, argv1);

		// Integration Tests

	}
	else
	{
		cout << "Entered " << mode << " which is not an option." << endl;
		sleep_for(milliseconds(1500));
	}
	return 0;
}


AV* VAST::generateAV(string name, dataMap runData, dataMap configData)
{	
	PythonVehicle* av = new PythonVehicle(configData, runData);
	getEventTree()->registerComponent(av);

	ProximitySensor* sensor = new ProximitySensor(av, runData);
	return av;	
}

Environment* VAST::generateEnv(string name, dataMap runData, dataMap configData)
{
	Environment* env = new SumoEnvironment(configData, runData);
	getEventTree()->registerComponent(env);
	getEventTree()->setFirstComponent(env); // NOTE: last in list of env gets to be "first" or lead component
	return env;
}

ScenarioMetric* VAST::generateMetric(string id)
{
	ScenarioMetric* metric = nullptr;
	// AvgSpeed, AvgAccel, AvgDecel // waiting for Christine's classes, emf 5/5/2019
	/*if (id == AVG_SPEED_METRIC_ID)
	{
		metric = new AverageSpeed();
	}
	else if (id == AVG_ACCEL_METRIC_ID)
	{
		metric = new AverageAcceleration();
	}
	else if (id == AVG_DECEL_METRIC_ID)
	{
		metric = new AverageDeacceleration();
	}
	else if (id == MAX_ACCEL_METRIC_ID)
	{
		metric = new MaximumAcceleration();
	}
	else if (id == MIN_ACCEL_METRIC_ID)
	{
		metric = new MinimumAcceleration();
	}
	else
	{
		stringstream ss;
		ss << "Cannot establish scenario metric by the name of " << id;
		throw VASTConfigurationError(ss.str().c_str());
	}*/
	getEventTree()->registerMetric(metric);
}

void unit_test(int uargc, char **uargv1)
{
	try
	{
		::testing::InitGoogleTest(&uargc, uargv1);
		RUN_ALL_TESTS();
		cout << "Test output file location: VASTTesting\\testOutput\\VAST_tests.xml" << endl;
	}
	catch (...)
	{
		cerr << "VAST unit tests threw an exception during verification.\n";
	}
}