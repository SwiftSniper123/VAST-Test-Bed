#pragma once

#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include "AV.h"
#include "Environment.h"
#include "VType.h"
#include "EventTree.h"

using namespace boost::property_tree;

class VAST
{
public:
	VAST();
	VAST(string file, string dbName);
	void Parse();
	void Register();
	
	EventTree *_EventTree;
	vector<AV*> _AVs;
	Environment *_Env;
	dataMap _ConfigMap;

	dataMap _EnvConfig;
	vector<dataMap> _AVConfigs;

private:
	dataMap _EnvMap;
	dataMap _AVMap;

	

	string _file;
	string _dbName;
	void fillMap(string currentModule, string type, string key, string value);
	
};