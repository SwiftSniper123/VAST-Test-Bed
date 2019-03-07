#pragma once
#include "Obstacle.h"
#include "vector3.h"
#include "VComponent.h"
#include "..\h\TraCIAPI.h"
#include "EventTree.h" 
#include "VType.h" 
#include <map>

class Environment : public VComponent

{

public:

	Environment(Vector3 _dimensions, int _numObstacles);
	Environment(Vector3 _dimensions, int _numObstacles, std::map<string, VType> _dataMap);
	/* Destructor for Environment object; deletes the obstacles referenced and the map contained inside of it. */
	~Environment();
	void AddObstacle(string name, Obstacle* obs);
	Vector3 getDimensions();
	int getNumObstacles();
	Obstacle* getObstacle(string _name);
	void update(double time, Event event);
private:
	Vector3 dimensions;
	int numObstacles;
	Obstacle **obstacles;
	std::map<string, VType> dataMap; // look up C++ std::map for functions 

};

class Client : public TraCIAPI {
public:
	Client() {};
	~Client() {};
};