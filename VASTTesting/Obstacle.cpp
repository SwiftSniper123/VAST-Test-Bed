#include "Obstacle.h"


Obstacle::Obstacle()
{

}

Obstacle::Obstacle(string name, dataMap obstacleData)
{
	_name = name;
	_dataMap = obstacleData;
}

Obstacle::~Obstacle()
{
	_dataMap.clear();
}

void Obstacle::update(timestamp time, dataMap updateMap)
{

}

void Obstacle::stopReplication(bool another, string runID)
{

}

VComponent::VCType Obstacle::getVCType()
{
	return Obstacle_Avatar;
}

string Obstacle::getName()
{
	return _name + "Obstacle";
}

dataMap Obstacle::getDataMap()
{
	return _dataMap;
}