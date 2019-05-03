#include "VAST.h"

VAST::VAST() {}

VAST::VAST(string file, string dbName)
{
	_file = file;
	if (dbName.empty())
	{
		_dbName = "VASTDatabase.db";
	}
	else
		_dbName = dbName;
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
	//printTree(pt1, 0);

	//std::cout << "\n\n" << pt1.get<std::string>("VAST.module.map.pair.key");
	
	//std::cout << pt1.size;
	//std::cout << pt1.get_child("VAST.module");

	BOOST_FOREACH(ptree::value_type const& node, pt1.get_child("VAST"))
	{
		//cerr << node.first.data. << "\n";
		boost::property_tree::ptree subtree = node.second;

		if (node.first == "module")
		{
			dataMap _AVConfig;
			_currentModule = subtree.get<std::string>("<xmlattr>.module");
			//std::cout << "module: " << subtree.get<std::string>("<xmlattr>.module") << "\n";

			BOOST_FOREACH(boost::property_tree::ptree::value_type const& map, subtree.get_child("map"))
			{
				boost::property_tree::ptree subMap = map.second;

				BOOST_FOREACH(boost::property_tree::ptree::value_type const& pair, subMap.get_child(""))
				{
					//std::cout << "pair: " << pair.first << "\n";
					boost::property_tree::ptree key = pair.second;

					std::string label = pair.first;
					if (label != "<xmlattr>")
					{
						if (label == "key")
						{
							//std::cout << "Key: " << key.get<std::string>("name") << "\n";
							_currentKey = key.get<std::string>("name");
						}
						
						else if (label == "value")
						{
							_currentValue = key.get<std::string>("name");
							//std::cout << "Value: " << _currentValue << "\n";

							if (_currentValue != "")
							{
								if (_currentModule == "VAST")
								{
									if (_currentKey == "output_file_location")
									{
										String *v = new String(new VType(key.get<string>("name")));
										_ConfigMap.insert(namedData(_currentKey, v));
									}
									else if (_currentKey == "viz_option")
									{
										Boolean *v = new Boolean(new VType(key.get<string>("name")));
										_ConfigMap.insert(namedData(_currentKey, v));
									}
									else if (_currentKey == "time_ratio")
									{
										Integer *v = new Integer(new VType(key.get<string>("name")));
										_ConfigMap.insert(namedData(_currentKey, v));
									}
									else if (_currentKey == "time_step")
									{
										Double *v = new Double(new VType(key.get<string>("name")));
										_ConfigMap.insert(namedData(_currentKey, v));
									}
									else if (_currentKey == "num_replications")
									{
										Integer *v = new Integer(new VType(key.get<string>("name")));
										_ConfigMap.insert(namedData(_currentKey, v));
									}
									else if (_currentKey == "seeds")
									{
										Array *v = new Array(new VType(key.get<string>("name")));
										_ConfigMap.insert(namedData(_currentKey, v));
									}
									else if (_currentKey == "max_run_time")
									{
										Double *v = new Double(new VType(key.get<string>("name")));
										_ConfigMap.insert(namedData(_currentKey, v));
									}
									else if (_currentKey == "metrics")
									{
										Array *v = new Array(new VType(key.get<string>("name")));
										_ConfigMap.insert(namedData(_currentKey, v));
									}
								}
								
								if (_currentModule == "Environment")
								{
									if (_currentKey == "env_obstacle_port")
									{
										Integer *v = new Integer(new VType(key.get<string>("name")));
										_EnvConfig.insert(namedData(_currentKey, v));
									}
									else if (_currentKey == "exe_location")
									{
										String *v = new String(new VType(key.get<string>("name")));
										_EnvConfig.insert(namedData(_currentKey, v));
									}
									else if (_currentKey == "Env_bounds")
									{
										Vector3 *v = new Vector3(new VType(key.get<string>("name")));
										_EnvConfig.insert(namedData(_currentKey, v));
									}
									else if (_currentKey == "config_location")
									{
										String *v = new String(new VType(key.get<string>("name")));
										_EnvConfig.insert(namedData(_currentKey, v));
									}
									else
									{
										size_t pos = _currentValue.find(_delim);
										size_t endPos = _currentValue.find(_delim) + 2;
										string _valCopy = _currentValue;
										_type = _valCopy.substr(0, pos);
										_currentValue = _currentValue.substr(endPos);

										fillMap(_currentModule, _type, _currentKey, _currentValue);
									}
								}

								else if (_currentModule == "AV")
								{
									if (_currentKey == "av_name")
									{
										string val = key.get<string>("name");
										if (!val.empty()) {
											String *v = new String(new VType(val));
											_AVConfig.insert(namedData(_currentKey, v));
										}
									}
									else if (_currentKey == "av_movement_port")
									{
										string val = key.get<string>("name");
										if (!val.empty()) {
											Integer *v = new Integer(new VType(val));
											_AVConfig.insert(namedData(_currentKey, v));
										}
									}
									else if (_currentKey == "exe_location")
									{
										string val = key.get<string>("name");
										if (!val.empty()) {
											String *v = new String(new VType(val));
											_AVConfig.insert(namedData(_currentKey, v));
										}
									}
									else if (_currentKey == "AV_location")
									{
										string val = key.get<string>("name");
										if (!val.empty()) {
											String *v = new String(new VType(val));
											_AVMap.insert(namedData(_currentKey, v));
										}
									}
									else if (_currentKey == "AV_orientation")
									{
										string val = key.get<string>("name");
										if (!val.empty()) {
											Vector3 *v = new Vector3(new VType(val));
											_AVMap.insert(namedData(_currentKey, v));
										}
									}
									else if (_currentKey == "AV_bounds")
									{
										string val = key.get<string>("name");
										if (!val.empty()) {
											Vector3 *v = new Vector3(new VType(val));
											_AVMap.insert(namedData(_currentKey, v));
										}
									}
									else if (_currentKey == "sensors")
									{
										string val = key.get<string>("name");
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

										fillMap(_currentModule, _type, _currentKey, _currentValue);
									}
								}
							}
						}
					}
				}
			}
			//std::cout << std::endl;

			if (_currentModule == "AV")
			{
				string name = _AVConfig.at("av_name")->s_value();
				_AVConfigs.push_back(_AVConfig);
				AV *av = new AV(name, _AVMap);
				_AVs.push_back(av);
			}
			else if (_currentModule == "Environment")
			{
				_Env = new Environment("Environment", _EnvMap);
			}
			else if (_currentModule == "VAST")
			{
				if (Integer(_ConfigMap["num_replications"]).value() > 1)
				{
					_EventTree = new EventTree(Double(_ConfigMap["time_Step"]).value(), int(Double(_ConfigMap["time_ratio"]).value() * 100), Double(_ConfigMap["max_run_time"]).value(), Integer(_ConfigMap["num_replications"]).value(), _dbName);
				}
				else
					_EventTree = new EventTree(Double(_ConfigMap["time_Step"]).value(), int(Double(_ConfigMap["time_ratio"]).value() * 100), Double(_ConfigMap["max_run_time"]).value(), _dbName);
			}
		}
	}//
	//registerMetrics();
}

void VAST::Register()
{
	_EventTree->registerComponent(_Env);
	for (int i = 0; i < _AVs.size(); i++)
	{
		_EventTree->registerComponent(_AVs[i]);
	}
}

void VAST::fillMap(string currentModule, string type, string key, string value)
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

	if (currentModule == "Environment")
	{
		_EnvMap.insert(namedData(key, v));
	}
	else if (currentModule == "AV")
	{
		_AVMap.insert(namedData(key, v));
	}
}