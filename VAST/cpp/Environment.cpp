#include "..\h\Environment.h"
#include <iostream>


//Adding SUMO Socket connection stuff

Environment::Environment(vector3 _dimensions, int _numObstacles)
{

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



int main(int argc, char* argv[]) {
	Client client;
	client.connect("localhost", 1337);
	std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	std::cout << "run 5 steps ...\n";
	client.simulationStep(5 * 1000);
	std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	client.close();
}