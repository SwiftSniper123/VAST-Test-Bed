#include "..\h\Environment.h"
#include <iostream>


//Adding SUMO Socket connection stuff

Environment::Environment(Vector3 _dimensions, int _numObstacles)
{

}

Environment::Environment(Vector3 _dimensions, int _numObstacles, std::map<string, VType> _dataMap)
{

}
Environment::~Environment()
{

}
void Environment::AddObstacle(string name, Obstacle*obs)
{

}

Obstacle *Environment::getObstacle(string _name)
{
	return nullptr;
}

Vector3 Environment::getDimensions()
{
	return Vector3{};
}

int Environment::getNumObstacles()
{
	return 0;
}

void Environment::update(double time, Event event)
{

}



