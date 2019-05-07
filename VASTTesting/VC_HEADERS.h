#pragma once
#include "AV.h"
#include "Environment.h"
#include "Sensor.h"
#include "Obstacle.h"
#include "ScenarioMetric.h"
#include "EventTree.h"
#include "VASTConstants.h"

//namespace VASTConstants
//{
//
//	// system
//	extern const char* DATABASE_FILENAME = "VASTDatabase.db";
//	extern const char* TEST_DATABASE_FILENAME = "DeleteThisDatabase.db";
//
//	// VAST Config
//	extern const std::string OUTPUT_FILE = "output_file_location";
//	extern const std::string VIZ = "viz_option";
//	extern const std::string RUN_ID = "run_id";
//	extern const std::string TIME_RATIO = "time_ratio";
//	extern const std::string TIME_STEP = "time_step";
//	extern const std::string NUM_REPLCATION = "num_replications";
//	extern const std::string SEEDS = "seeds";
//	extern const std::string MAX_RUN_TIME = "max_run_time";
//	extern const std::string AV_LIST = "av_list"; // also AV run data
//	extern const std::string METRICS = "metrics";
//
//	// Scenario Metric type ids
//	extern const std::string AVG_SPEED_METRIC_ID = "AvgSpeed";
//	extern const std::string AVG_ACCEL_METRIC_ID = "AvgAccel";
//	extern const std::string AVG_DECEL_METRIC_ID = "AvgDecel";
//	extern const std::string MAX_ACCEL_METRIC_ID = "MaxAccel";
//	extern const std::string MIN_ACCEL_METRIC_ID = "MinAccel";
//
//	// ScenarioMetric
//	extern const std::string ACCELERATION = "Acceleration";
//	extern const std::string MIN_ACCELERATION = "MinAcceleration";
//	extern const std::string AVG_ACCELERATION = "AvgAcceleration";
//	extern const std::string MAX_ACCELERATION = "MaxAcceleration";
//	extern const std::string AVG_DECELERATION = "AvgDeceleration";
//	extern const std::string AVG_SPEED = "AvgSpeed";
//	extern const std::string SPEED = "Speed";
//	extern const std::string POSITION = "Position";
//
//	// Environment Config
//	extern const std::string ENV_OBSTACLE_PORT = "env_obstacle_port";
//	extern const std::string EXE_LOCATION = "exe_location";
//	extern const std::string ENV_BOUNDS = "Env_bounds";
//	extern const std::string CONFIG_LOCATION = "config_location";
//	// Environment Run Data
//	extern const std::string OBSTACLE_IDS = "obstacle_ids";
//	extern const std::string OBSTACLE_POS = "obstacle_pos";
//	extern const std::string OBSTACLE_VEL = "obstacle_speeds";
//	extern const std::string OBSTACLE_ACC = "obstacle_accelerations";
//	extern const std::string DURATION = "Duration";
//	extern const std::string TARGET_VELOCITY = "TargetVelocity";
//
//	// AV Config
//	extern const std::string AV_NAME = "av_name";
//	extern const std::string AV_MOVEMENT_PORT = "av_movement_port";
//	extern const std::string AV_EXE_LOCATION = "av_exe_location";
//	extern const std::string AV_LOCATION = "AV_location";
//	extern const std::string AV_ORIENTATION = "AV_orientation";
//	extern const std::string AV_BOUNDS = "AV_bounds";
//	extern const std::string SENSORS = "sensors";
//	// AV Run Data
//	extern const std::string CLOSEST_ID = "closest_obs";
//	extern const std::string CLOSEST_POS = "closest_position";
//	extern const std::string CLOSEST_DIST = "closest_distance";
//
//	// parser
//	extern const std::string MODULE = "module";
//	extern const std::string VAST_MODULE = "VAST";
//	extern const std::string ENVIRONMENT_MODULE = "Environment";
//	extern const std::string AV_MODULE = "AV";
//	extern const std::string MAP = "map";
//	extern const std::string KEY = "key";
//	extern const std::string NAME = "name";
//	extern const std::string VALUE = "value";
//}