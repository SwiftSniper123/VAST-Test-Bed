#include "VAST.h"

using namespace VASTConstants;

VAST::VAST() {}

VAST::VAST(string file, string dbName)
{
	_file = file;
	if (dbName.empty())
	{
		_dbName = DATABASE_FILENAME;
	}
	else
		_dbName = dbName;
}

VAST::~VAST()
{
	delete _EventTree;
	_AVs->clear();
	_Env->clear();

	_all_AVConfigs->clear();
	_all_AVRunData->clear();
	_all_EnvConfigs->clear();
	_all_EnvRunData->clear();
	_VASTConfigMap->clear();
	//_SensorConfig->clear();
}

vector<AV*>* VAST::getAVs()
{
	return _AVs;
}

vector<Environment*>* VAST::getEnvs()
{
	return _Env;
}

dataMap* VAST::getVASTConfig()
{
	return _VASTConfigMap;
}

EventTree* VAST::getEventTree()
{
	return _EventTree;
}

void VAST::Parse()
{
	ptree pt1;
	string _currentKey;
	string _currentModule;
	string _currentValue;
	string _type;
	const string _delim = ", ";
	
	try {
		read_xml(_file, pt1);
	}
	catch(const std::exception& e) {
		std::cout << "Invalid File. Ending.";
		return;
	}
	try 
	{
		//printTree(pt1, 0);

		//std::cout << "\n\n" << pt1.get<string>("VAST.module.map.pair.key");

		//std::cout << pt1.size;
		//std::cout << pt1.get_child("VAST.module");

		BOOST_FOREACH(ptree::value_type const& node, pt1.get_child(VAST_MODULE))
		{
			//cerr << node.first.data. << "\n";
			boost::property_tree::ptree subtree = node.second;

			if (node.first == MODULE)
			{
				dataMap _AVConfig;
				dataMap _AVRun_Data;
				dataMap _EnvConfig;
				dataMap _EnvRun_Data;
				_currentModule = subtree.get<string>("<xmlattr>.module");
				//std::cout << "module: " << subtree.get<string>("<xmlattr>.module") << "\n";

				BOOST_FOREACH(boost::property_tree::ptree::value_type const& map, subtree.get_child(MAP))
				{
					boost::property_tree::ptree subMap = map.second;

					BOOST_FOREACH(boost::property_tree::ptree::value_type const& pair, subMap.get_child(""))
					{
						//std::cout << "pair: " << pair.first << "\n";
						boost::property_tree::ptree key = pair.second;

						string label = pair.first;
						if (label != "<xmlattr>")
						{
							if (label == KEY)
							{
								//std::cout << "Key: " << key.get<string>("name") << "\n";
								_currentKey = key.get<string>(NAME);
							}

							else if (label == VALUE)
							{
								_currentValue = key.get<string>(NAME);
								//std::cout << "Value: " << _currentValue << "\n";

								if (_currentValue != "")
								{
									if (_currentModule == VAST_MODULE)
									{
										if (_currentKey == OUTPUT_FILE)
										{
											String *v = new String(key.get<string>(NAME));
											_VASTConfigMap->insert(namedData(_currentKey, v));
										}
										else if (_currentKey == VIZ)
										{
											Boolean *v = new Boolean(key.get<bool>(NAME));
											_VASTConfigMap->insert(namedData(_currentKey, v));
										}
										else if (_currentKey == TIME_RATIO)
										{
											Integer *v = new Integer(key.get<int>(NAME));
											_VASTConfigMap->insert(namedData(_currentKey, v));
										}
										else if (_currentKey == TIME_STEP)
										{
											Double *v = new Double(key.get<double>(NAME));
											_VASTConfigMap->insert(namedData(_currentKey, v));
										}
										else if (_currentKey == NUM_REPLCATION)
										{
											Integer *v = new Integer(key.get<int>(NAME));
											_VASTConfigMap->insert(namedData(_currentKey, v));
										}
										else if (_currentKey == SEEDS)
										{
											Array *v = new Array(key.get<string>(NAME));
											_VASTConfigMap->insert(namedData(_currentKey, v));
										}
										else if (_currentKey == MAX_RUN_TIME)
										{
											Double *v = new Double(key.get<double>(NAME));
											_VASTConfigMap->insert(namedData(_currentKey, v));
										}
										else if (_currentKey == METRICS)
										{
											Array *v = new Array(key.get<string>(NAME));
											_VASTConfigMap->insert(namedData(_currentKey, v));
										}
									}

									if (_currentModule == ENVIRONMENT_MODULE)
									{
										if (_currentKey == ENV_OBSTACLE_PORT)
										{
											Integer *v = new Integer(key.get<int>(NAME));
											_EnvConfig.insert(namedData(_currentKey, v));
										}
										else if (_currentKey == EXE_LOCATION)
										{
											String *v = new String(key.get<string>(NAME));
											_EnvConfig.insert(namedData(_currentKey, v));
										}
										else if (_currentKey == ENV_BOUNDS)
										{
											Vector3 *v = new Vector3(key.get<string>(NAME));
											_EnvConfig.insert(namedData(_currentKey, v));
										}
										else if (_currentKey == CONFIG_LOCATION)
										{
											String *v = new String(key.get<string>(NAME));
											_EnvConfig.insert(namedData(_currentKey, v));
										}
										else
										{
											// gathering run data fields
											size_t pos = _currentValue.find(_delim);
											size_t endPos = _currentValue.find(_delim) + 2;
											string _valCopy = _currentValue;
											_type = _valCopy.substr(0, pos);
											_currentValue = _currentValue.substr(endPos);

											fillMap(_currentModule, _EnvRun_Data, _type, _currentKey, _currentValue);
										}
									}

									else if (_currentModule == AV_MODULE)
									{
										if (_currentKey == AV_NAME)
										{
											string val = key.get<string>(NAME);
											if (!val.empty()) {
												String *v = new String(val);
												_AVConfig.insert(namedData(_currentKey, v));												
											}
										}
										else if (_currentKey == AV_MOVEMENT_PORT)
										{
											string val = key.get<string>(NAME);
											if (!val.empty()) {
												Integer *v = new Integer(new VType(val));
												_AVConfig.insert(namedData(_currentKey, v));
											}
										}
										else if (_currentKey == AV_EXE_LOCATION)
										{
											string val = key.get<string>(NAME);
											if (!val.empty()) {
												String *v = new String(new VType(val));
												_AVConfig.insert(namedData(_currentKey, v));
											}
										}
										else if (_currentKey == AV_LOCATION)
										{
											string val = key.get<string>(NAME);
											if (!val.empty()) {
												String *v = new String(new VType(val));
												_AVRun_Data.insert(namedData(_currentKey, v));
											}
										}
										else if (_currentKey == AV_ORIENTATION)
										{
											string val = key.get<string>(NAME);
											if (!val.empty()) {
												Vector3 *v = new Vector3(new VType(val));
												_AVRun_Data.insert(namedData(_currentKey, v));
											}
										}
										else if (_currentKey == AV_BOUNDS)
										{
											string val = key.get<string>(NAME);
											if (!val.empty()) {
												Vector3 *v = new Vector3(new VType(val));
												_AVRun_Data.insert(namedData(_currentKey, v));
											}
										}
										else if (_currentKey == SENSORS)
										{
											string val = key.get<string>(NAME);
											if (!val.empty()) {
												Array *v = new Array(new VType(val));
												_AVConfig.insert(namedData(_currentKey, v));
											}
										}
										else
										{
											size_t pos = _currentValue.find(_delim);
											size_t endPos = _currentValue.find(_delim) + 2;
											string _valCopy = _currentValue;
											_type = _valCopy.substr(0, pos);
											_currentValue = _currentValue.substr(endPos);

											fillMap(_currentModule, _AVRun_Data, _type, _currentKey, _currentValue);
										}
									}
								}
							}
						}
					}
				}
				//std::cout << std::endl;

				// instantiation and other aggregate data joining
				if (_currentModule == AV_MODULE)
				{
					string name = _AVConfig.at(AV_NAME)->s_value();

					// add the AV name to the VAST Config list of AV IDs used by other components like ScenarioMetric
					if (_VASTConfigMap->find(AV_LIST) != _VASTConfigMap->end() &&
						_VASTConfigMap->at(AV_LIST)->isA(ARRAY_TYPE))
					{
						Array* av_array = ((Array*)_VASTConfigMap->at(AV_LIST));
						av_array->add(new String(name));
					}

					// add the AV name to the AV Run data list of AV IDs used by other components like ScenarioMetric
					if (_AVRun_Data.find(AV_LIST) != _AVRun_Data.end() &&
						_AVRun_Data[AV_LIST]->isA(ARRAY_TYPE))
					{
						Array* av_array = ((Array*)_AVRun_Data[AV_LIST]);
						av_array->add(new String(name));
					}
					
					// store the config and run data under this AV ID
					_all_AVConfigs->emplace(name, _AVConfig);
					_all_AVRunData->emplace(name, _AVRun_Data);
				}
				else if (_currentModule == ENVIRONMENT_MODULE)
				{
					// create an Environment ID based upon the list size, then store config and run data
					string ID = "" + (int(_all_EnvConfigs->size()) + 1);
					_all_EnvConfigs->emplace(ID, _EnvConfig);
					_all_EnvRunData->emplace(ID, _EnvRun_Data);
				}
				else if (_currentModule == VAST_MODULE)
				{					
					_EventTree = new EventTree(
						((Double*)_VASTConfigMap->at(TIME_STEP))->value(),
						((Double*)_VASTConfigMap->at(TIME_RATIO))->value(),
						((Double*)_VASTConfigMap->at(MAX_RUN_TIME))->value(),
						((Integer*)_VASTConfigMap->at(NUM_REPLCATION))->value(),
						_dbName);					
				}
			}
		}
	}
	catch (...)
	{
		throw VASTConfigurationError("VAST configuration has run into an error.");
	}
	//
	//registerMetrics();
}

/*
void VAST::Register()
{
	_EventTree->registerComponent(_Env);
	for (int i = 0; i < _AVs.size(); i++)
	{
		_EventTree->registerComponent(_AVs[i]);
	}
}*/

void VAST::fillMap(string currentModule, dataMap run_Data, string type, string key, string value)
{
	char first = type.at(0);
	VType *v;

	String *s;
	Double *d;
	Integer *i;
	Boolean *b;
	Vector3 *vec;
	Array *a;

	//std::cout << first;
	
	switch (first)
	{
	case 's':
	case 'S':
		s = new String(new VType(value));
		v = s;
		break;
	case 'd':
	case 'D':
		d = new Double(new VType(value));
		v = d;
		break;
	case 'i':
	case 'I':
		i = new Integer(new VType(value));
		v = i;
		break;
	case 'b':
	case 'B':
		b = new Boolean(new VType(value));
		v = b;
		break;
	case 'v':
	case 'V':
		vec = new Vector3(new VType(value));
		v = vec;
		break;
	case 'a':
	case 'A':
		a = new Array(new VType(value));
		v = a;
		break;
	}

	if (currentModule == ENVIRONMENT_MODULE)
	{
		run_Data.insert(namedData(key, v));  // TODO: this should be multiple env run data maps
	}
	else if (currentModule == AV_MODULE)
	{
		run_Data.insert(namedData(key, v)); // TODO: this should be multiple av run data maps
	}
}