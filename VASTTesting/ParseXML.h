#pragma once

#include <ole2.h>
#include <xmllite.h>
#include <stdio.h>
#include <shlwapi.h>
#include "AV.h"
#include "Environment.h"

class XMLParser
{
public:
	XMLParser(LPCWSTR configFile);
	HRESULT WriteAttributes(IXmlReader* pReader);
	int __cdecl Parse();
	void ParseDoc();

private:
	LPCWSTR _file;
	void ParseModule(HRESULT hr,
					IStream *pFileStream,
					IXmlReader *pReader,
					XmlNodeType nodeType,
					const WCHAR* pwszPrefix,
					const WCHAR* pwszLocalName,
					const WCHAR* pwszValue,
					UINT cwchPrefix);

	struct AV_Config {
		string _av_name;
		int _av_movement_port;
		string _exe_location;
		Vector3 _AV_location;
		Vector3 _AV_orientation;
		Vector3 _AV_bounds;
		string _metrics;
	};

	struct Environment_Config {
		int _env_obstacle_port;
		string _exe_location;
		Vector3 _env_bounds;
	};

	struct VAST_Config {
		string _output_file_location;
		bool _viz_option;
		int _time_ratio;
		float _time_step;
		int _num_replications;
		string seeds;
		float _max_run_time;
		string _metrics;
		AV_Config **_AVs;
		Environment_Config *_env_config;
	};

};

class Configuration
{
public:
	Configuration();
	//Configuration LoadConfig(string fileLocation);

private:
	struct AV_Config {
		string _av_name;
		int _av_movement_port;
		string _exe_location;
		Vector3 _AV_location;
		Vector3 _AV_orientation;
		Vector3 _AV_bounds;
		string _metrics;
	};

	struct Environment_Config {
		int _env_obstacle_port;
		string _exe_location;
		Vector3 _env_bounds;
	};

	struct VAST_Config {
		string _output_file_location;
		bool _viz_option;
		int _time_ratio;
		float _time_step;
		int _num_replications;
		string seeds;
		float _max_run_time;
		string _metrics;
		AV_Config **_AVs;
		Environment_Config *_env_config;
	};
	
};