#include "Environment.h"
#include "EventTree.h"

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

	_changingMap["Duration"] = dataMap["Duration"];
	_changingMap["TargetVelocity"] = dataMap["TargetVelocity"];

	_changingMap = callUpdateFunctions();

	temp->addEvent(this, t, _changingMap);
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

