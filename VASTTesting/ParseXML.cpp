#include "ParseXML.h"


//Configuration::Configuration() {};

/*Configuration Configuration::LoadConfig(string filelocation)
{

};*/

XMLParser::XMLParser(string file)
{
	_file = file;
}

string indent(int level) {
	string s;
	for (int i = 0; i < level; i++) s += "  ";
	return s;
}

void printTree(ptree &pt, int level) {
	if (pt.empty()) {
		cerr << "\"" << pt.data() << "\"";
	}

	else {
		if (level) cerr << endl;

		cerr << indent(level) << "{" << endl;
		//std::cout << "Test";
		for (ptree::iterator pos = pt.begin(); pos != pt.end();) {
			cerr << indent(level + 1) << "\"" << pos->first << "\": ";

			printTree(pos->second, level + 1);
			++pos;
			if (pos != pt.end()) {
				cerr << ",";
			}
			cerr << endl;
		}

		cerr << indent(level) << " }";
	}

	return;
}

void XMLParser::Parse()
{
	read_xml(_file, pt1);
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
										String v = new VType(key.get<string>("name"));
										_VASTMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "viz_option")
									{
										Boolean v = new VType(key.get<string>("name"));
										_VASTMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "time_ratio")
									{
										Integer v = new VType(key.get<string>("name"));
										_VASTMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "time_step")
									{
										Double v = new VType(key.get<std::string>("name"));
										_VASTMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "num_replications")
									{
										Integer v = new VType(key.get<std::string>("name"));
										_VASTMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "seeds")
									{
										Array v = new VType(key.get<std::string>("name"));
										_VASTMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "max_run_time")
									{
										Double v = new VType(key.get<std::string>("name"));
										_VASTMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "metrics")
									{
										Array v = new VType(key.get<std::string>("name"));
										_VASTMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}

								}
								
								if (_currentModule == "Environment")
								{
									if (_currentKey == "env_obstacle_port")
									{
										Integer v = new VType(key.get<std::string>("name"));
										_EnvMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "exe_location")
									{
										String v = new VType(key.get<std::string>("name"));
										_EnvMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "Env_bounds")
									{
										Vector3 v = new VType(key.get<std::string>("name"));
										_EnvMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else
									{
										size_t pos = _currentValue.find(_delim);
										string _valCopy = _currentValue;
										_type = _valCopy.substr(0, pos);
										_currentValue = _currentValue.substr(pos);

										fillMap(_currentModule, _type, _currentKey, _currentValue);
									}
								}
								else if (_currentModule == "AV")
								{
									if (_currentKey == "av_name")
									{
										String v = new VType(key.get<std::string>("name"));
										_AVMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "av_movement_port")
									{
										Integer v = new VType(key.get<std::string>("name"));
										_AVMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "exe_location")
									{
										String v = new VType(key.get<std::string>("name"));
										_AVMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "AV_location")
									{
										String v = new VType(key.get<std::string>("name"));
										_AVMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "AV_orientation")
									{
										Vector3 v = new VType(key.get<std::string>("name"));
										_AVMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "AV_bounds")
									{
										Vector3 v = new VType(key.get<std::string>("name"));
										_AVMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else if (_currentKey == "sensors")
									{
										Array v = new VType(key.get<std::string>("name"));
										_AVMap.insert(std::pair<std::string, VType>(_currentKey, v));
									}
									else
									{
										size_t pos = _currentValue.find(_delim);
										string _valCopy = _currentValue;
										_type = _valCopy.substr(0, pos);
										_currentValue = _currentValue.substr(pos);

										fillMap(_currentModule, _type, _currentKey, _currentValue);
									}
									//std::cout << "Placeholder";
								}

								
							}
						}
						
					}

					//BOOST_FOREACH(boost::property_tree::ptree::value_type const& value, key.get_child(""))
					//{
					//	std::string label = value.first;
					//	//std::cout << "value" << label <<"\n";
					//	boost::property_tree::ptree keyValue = value.second;

					//	std::string test = keyValue.get<std::string>("");
					//	//std::cout << test;
					//	/*
					//	if (label != "<xmlattr>")
					//	{
					//		//std::cout << pair.first << "\n";
					//		std::cout << value.first;
					//		//std::string key = keyValue.get<std::string>("type.key");
					//	}*/
					//}
				}
				//std::string value = test.get<std::string>("pair.value");

				//std::cout << "key: " << key << "value: " << value;

				//std::string label = v.second;
				//std::cout << label;
				
				/*if (label != "<xmlattr>")
				{
					std::string value = subtree.get<std::string>(label);
					std::cout << label << ":  " << value << std::endl;
				}*/
			}
			//std::cout << std::endl;

			if (_currentModule == "AV")
			{
				AvVector.push_back(_AVMap);
			}
		}
	}//
	
}

void XMLParser::fillMap(string currentModule, string type, string key, string value)
{
	char first = type.at(0);
	VType v;

	String s;
	Double d;
	Integer i;
	Boolean b;
	Vector3 vec;
	Array a;

	//std::cout << first;
	
	switch (first)
	{
	case 's':
	case 'S':
		s = new VType(value);
		v = s;
		break;
	case 'd':
	case 'D':
		d = new VType(value);
		v = d;
		break;
	case 'i':
	case 'I':
		i = new VType(value);
		v = i;
		break;
	case 'b':
	case 'B':
		b = new VType(value);
		v = b;
		break;
	case 'v':
	case 'V':
		vec = new VType(value);
		v = vec;
		break;
	case 'a':
	case 'A':
		a = new VType(value);
		v = a;
		break;
	}

	if (currentModule == "Environment")
	{
		XMLParser::_EnvMap.insert(std::pair<std::string, VType>(key, v));
	}
	else if (currentModule == "AV")
	{
		XMLParser::_AVMap.insert(std::pair<std::string, VType>(key, v));
	}
}