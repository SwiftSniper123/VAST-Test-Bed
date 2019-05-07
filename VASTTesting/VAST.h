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
	VAST(string file, string dbName);
	~VAST();



	// parses the entire config sheet and gathers information for data maps
	void Parse();

	// returns a pointer to the established EventTree object created with config sheet specs
	EventTree* getEventTree();

	// getters for map data after Parse has been called
	storedMap* get_AVConfig();
	storedMap* get_AVRunData();
	storedMap* get_EnvConfig();
	storedMap* get_EnvRunData();
	dataMap* getVASTConfig();
	
	/* called after Parse.  Iterates through all components and calls custom generation functions implemented by user in main.*/
	void Register();

	/* Implemented in main by user of VAST to specify the types*/
	virtual AV* generateAV(string name, dataMap runData, dataMap configData);
	virtual Environment* generateEnv(string name, dataMap runData, dataMap configData);
	virtual ScenarioMetric* generateMetric(string id, dataMap map);

private:
	// maps of name to dataMap for every AV and Environment
	storedMap* _all_AVConfigs = nullptr;
	storedMap* _all_AVRunData = nullptr;
	storedMap* _all_EnvConfigs = nullptr;
	storedMap* _all_EnvRunData = nullptr;
	dataMap* _VASTConfigMap = nullptr;
	Array *av_array = new Array();
	
	EventTree* _EventTree = nullptr;
	string _file;
	string _dbName;	

	// helper function for parsing the remaining functions in the config sheet once the mandatory items are done
	void fillMap(string currentModule, dataMap &runData, string type, string key, string value);
};