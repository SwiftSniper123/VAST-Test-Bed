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

}

void AV::stopReplication(bool another, string runID)
{

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
