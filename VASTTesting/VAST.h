#pragma once

#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include "VC_HEADERS.h"

using namespace boost::property_tree;

typedef void(*ConstructorFn)(dataMap config, dataMap run);

class VAST
{
public:
	VAST();
	VAST(string file, string dbName);

	void Parse();
	//void RegisterComps(vector<AV> AVs, Environment Env);
	//void RegisterMetrics(ScenarioMetric sn);

	EventTree *_EventTree;
	vector<AV*> _AVs;
	Environment *_Env;

	vector<dataMap> getAVConfigs();
	dataMap getEnvConfig();
	dataMap getConfig();

private:
	//ConstructorFn _EnvConstructor;
	//ConstructorFn _AVConstructor;
	//ConstructorFn _SensorConstructor;

	dataMap _EnvMap;
	dataMap _AVMap;
	dataMap _SensorMap;

	vector<dataMap> _AVConfigs;
	dataMap _EnvConfig;
	dataMap _ConfigMap;
	dataMap _SensorConfig;

	string _file;
	string _dbName;
	void fillMap(string currentModule, string type, string key, string value);

};