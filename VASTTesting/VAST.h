#pragma once

#include "AV.h"
#include "Environment.h"

class VAST
{
public:

private:
	string _output_file_location;
	bool _viz_option;
	int _time_ratio;
	float _time_step;
	int _num_replications;
	string seeds;
	float _max_run_time;
	string _metrics;
	Environment *_env;
	AV *_AV;

};