#pragma once

#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

#include "VC_HEADERS.h"

using namespace boost::property_tree;

typedef void(*ConstructorFn)(dataMap config, dataMap run);
typedef exception VASTConfigurationError;

typedef map<string, dataMap> storedMap; // to provide data to EventTree

class VAST
{
public:
	VAST();
	VAST(string file, string dbName);
	~VAST();

	// parses the entire config sheet and gathers information for data maps
	void Parse();

	// returns a pointer to the established EventTree object created with config sheet specs
	EventTree* getEventTree();

	// getters for map data after Parse has been called
	storedMap* get_AVConfig(string name);
	storedMap* get_AVRunData(string name);
	storedMap* get_EnvConfig(string name);
	storedMap* get_EnvRunData(string name);
	dataMap* getVASTConfig();
	

	virtual AV* generateAV(string name, dataMap runData, dataMap configData);
	virtual Environment* generateEnv(string name, dataMap runData, dataMap configData);
	
private:

	// maps of name to dataMap for every AV and Environment
	storedMap* _all_AVConfigs;
	storedMap* _all_AVRunData;
	storedMap* _all_EnvConfigs;
	storedMap* _all_EnvRunData;
	dataMap* _VASTConfigMap;

	
	EventTree* _EventTree;
	// maybe store scenario metrics?
	/*vector<AV*>* _AVs;
	vector<Environment*>* _Env;*/ // no need to keep these anymore, components are not stored in VAST, but rather in main

	string _file;
	string _dbName;

	// helper function for parsing the remaining functions in the config sheet once the mandatory items are done
	void fillMap(string currentModule, dataMap runData, string type, string key, string value);

};