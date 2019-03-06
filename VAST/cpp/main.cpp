//Virtual Autonomous System Testbed
//ODU Capstone 2018-2019

#include "..\h\VComponent.h"
#include <iostream>
#include <map>
#include<variant>
#include "..\h\VType.h"
#include "..\h\AV.h"
#include "..\h\Environment.h"
#include "..\h\SumoTraciTestClass.h"
#include <Windows.h>


using namespace std;

class ExtendedVComponent
{
public:
	ExtendedVComponent(int id)
	{
		this_id = id;
	};

	void update()
	{
		cout << "\nExtendedVComponent" << this_id << " is updating." << endl;  
	};

private:
	int this_id;
};

int main(int argc, char **argv1)
{
	/*create the map
	need to extract information from configuration file
	user will input variable name, type, and intial value*/
	std::map<std::string, AVDataTypes> AVmap; //string is the name
	//declare variables
	AVDataTypes vehicleType("vehicleType", "std::string"); //std::string vehicleType;
	AVDataTypes maxVelocity("maxVelocity", "float"); // float maxVelocity;
	AVDataTypes currentVelocity("currentVelocity", "float"); // float currentVelocity;
	AVDataTypes maxAltitude("maxAltitude", "float"); // float maxAltitude;
	AVDataTypes currentAltitude("currentAltitude", "float"); // float currentAltitude;
	AVDataTypes currentPosition("currentPosition", "vector3"); // vector3 currentPosition;
	AVDataTypes initialPosition("initialPosition", "vector3"); // vector3 initialPosition;

	//initialize variables
	//NEED TO CREATE A NUMBER TYPE
	vehicleType.set("car");
	maxVelocity.set(10);
	currentVelocity.set(0);
	maxAltitude.set(100);
	currentAltitude.set(0);
	currentPosition.set(new vector3(0, 0, 0));
	initialPosition.set(new vector3(0, 0, 0));

	//add variables to AV map
	AVmap["vehicleType"] = vehicleType;
	AVmap["maxVelocity"] = maxVelocity;
	AVmap["currentVelocity"] = currentVelocity;
	AVmap["maxAltitude"] = maxAltitude;
	AVmap["currentAltitude"] = currentAltitude;
	AVmap["currentPosition"] = currentPosition;
	AVmap["initialPosition"] = initialPosition;

	SumoTraciRandomTestClass test;
	test.initialize();
	test.runSumo(); //There will be a 2.5 second pause while SUMO boots up
	test.runClient();
}
