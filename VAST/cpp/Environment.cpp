#include "..\h\Environment.h"
#include <iostream>

//Adding SUMO Socket connection stuff

Environment::Environment() {};

Environment::Environment(std::string simPath, vector3 dimensions, int numObstacles, int port)
{
	_simPath = simPath;
	_dimensions = dimensions;
	_numObstacles = numObstacles;
	_port = port;
}

Environment::~Environment()
{

}
void Environment::addObstacle()
{

}
/*void Environment::updateAV(Command update)
{

}*/
void Environment::update()
{

}



