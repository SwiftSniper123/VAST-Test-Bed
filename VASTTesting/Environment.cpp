#include "Environment.h"

Environment::Environment()
{

}

Environment::Environment(string name, dataMap environmentData)
{
	_name = name;
	_dataMap = environmentData;
}

Environment::~Environment()
{
	_dataMap.clear();
}

void Environment::update(timestamp t, dataMap dataMap)
{
	EventTree *temp = getEventTree();

	temp->addEvent(this, t, dataMap);
}

VComponent::VCType Environment::getVCType()
{
	return Environment_Avatar;
}

string Environment::getName()
{
	return _name + "Environment";
}

dataMap Environment::getDataMap()
{
	return _dataMap;
}

