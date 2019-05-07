#pragma once
#include <string>

namespace
{
	typedef const char* VASTDefault;
	typedef const std::string VASTConstant;
	
	// system
	VASTDefault DATABASE_FILENAME = "VASTDatabase.db";
	VASTDefault TEST_DATABASE_FILENAME = "DeleteThisDatabase.db";

  // VAST Config
	VASTConstant OUTPUT_FILE = "output_file_location";
	VASTConstant VIZ = "viz_option";
	VASTConstant RUN_ID = "run_id";
	VASTConstant TIME_RATIO = "time_ratio";
	VASTConstant TIME_STEP = "time_step";
	VASTConstant NUM_REPLCATION = "num_replications";
	VASTConstant SEEDS = "seeds";
	VASTConstant MAX_RUN_TIME = "max_run_time";
	VASTConstant AV_LIST = "av_list"; // also AV run data
	VASTConstant METRICS = "metrics";
		
	// Scenario Metric type ids
	VASTConstant AVG_SPEED_METRIC_ID = "AvgSpeed";
	VASTConstant AVG_ACCEL_METRIC_ID = "AvgAccel";
	VASTConstant AVG_DECEL_METRIC_ID = "AvgDecel";
	VASTConstant MAX_ACCEL_METRIC_ID = "MaxAccel";
	VASTConstant MIN_ACCEL_METRIC_ID = "MinAccel";
	
	// ScenarioMetric
	VASTConstant ACCELERATION = "Acceleration";
	VASTConstant MIN_ACCELERATION = "MinAcceleration";
	VASTConstant AVG_ACCELERATION = "AvgAcceleration";
	VASTConstant MAX_ACCELERATION = "MaxAcceleration";
	VASTConstant AVG_DECELERATION = "AvgDeceleration";
	VASTConstant AVG_SPEED = "AvgSpeed";
	VASTConstant SPEED = "Speed";
	VASTConstant POSITION = "Position";

	// Environment Config
	VASTConstant ENV_OBSTACLE_PORT = "env_obstacle_port";
	VASTConstant EXE_LOCATION = "exe_location";
	VASTConstant ENV_BOUNDS = "Env_bounds";
	VASTConstant CONFIG_LOCATION = "config_location";
	// Environment Run Data
	VASTConstant OBSTACLE_IDS = "obstacle_ids";
	VASTConstant OBSTACLE_POS = "obstacle_pos";
	VASTConstant OBSTACLE_VEL = "obstacle_speeds";
	VASTConstant OBSTACLE_ACC = "obstacle_accelerations";
	VASTConstant DURATION = "Duration";
	VASTConstant TARGET_VELOCITY = "TargetVelocity";

	// AV Config
	VASTConstant AV_NAME = "av_name";
	VASTConstant AV_MOVEMENT_PORT = "av_movement_port";
	VASTConstant AV_EXE_LOCATION = "av_exe_location";
	VASTConstant AV_LOCATION = "AV_location";
	VASTConstant AV_ORIENTATION = "AV_orientation";
	VASTConstant AV_BOUNDS = "AV_bounds";
	VASTConstant SENSORS = "sensors";
	// AV Run Data
	VASTConstant CLOSEST_ID = "closest_obs";
	VASTConstant CLOSEST_POS = "closest_position";
	VASTConstant CLOSEST_DIST = "closest_distance";

	// parser
	VASTConstant MODULE = "module";
	VASTConstant VAST_MODULE = "VAST";
	VASTConstant ENVIRONMENT_MODULE = "Environment";
	VASTConstant AV_MODULE = "AV";
	VASTConstant MAP = "map";
	VASTConstant KEY = "key";
	VASTConstant NAME = "name";
	VASTConstant VALUE = "value";
}