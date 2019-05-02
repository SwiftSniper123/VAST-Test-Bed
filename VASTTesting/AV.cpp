#include "AV.h"

AV::AV()
{

}

AV::~AV()
{
	_dataMap.clear();
}

AV::AV(string name, dataMap avData)
{
	_name = name;
	_dataMap = avData;
}

void AV::update(timestamp t, dataMap dataMap)
{
	EventTree *temp = getEventTree();

	_sensorDataMap["VehicleID"] = dataMap["VehicleID"];
	_sensorDataMap["Speed"] = dataMap["Speed"];
	_sensorDataMap["Position"] = dataMap["Position"];

	_sensorDataMap = callUpdateFunctions();

	temp->addEvent(this, t, _sensorDataMap);
}

VComponent::VCType AV::getVCType()
{
	return AV_Avatar;
}

string AV::getName()
{
	return _name + "AV";
}

dataMap AV::getDataMap()
{
	return _dataMap;
}

dataMap AV::getSensorDataMap()
{
	return _sensorDataMap;
}
