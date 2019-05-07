#pragma once
#include <string>
	
// system
extern const char* DATABASE_FILENAME;
extern const char* TEST_DATABASE_FILENAME;

// VAST Config
extern const std::string OUTPUT_FILE;
extern const std::string VIZ;
extern const std::string RUN_ID;
extern const std::string TIME_RATIO;
extern const std::string TIME_STEP;
extern const std::string NUM_REPLCATION;
extern const std::string SEEDS;
extern const std::string MAX_RUN_TIME;
extern const std::string AV_LIST; // also AV run data
extern const std::string METRICS;

// Scenario Metric type ids
extern const std::string AVG_SPEED_METRIC_ID;
extern const std::string AVG_ACCEL_METRIC_ID;
extern const std::string AVG_DECEL_METRIC_ID;
extern const std::string MAX_ACCEL_METRIC_ID;
extern const std::string MIN_ACCEL_METRIC_ID;

// ScenarioMetric
extern const std::string ACCELERATION;
extern const std::string MIN_ACCELERATION;
extern const std::string AVG_ACCELERATION;
extern const std::string MAX_ACCELERATION;
extern const std::string AVG_DECELERATION;
extern const std::string AVG_SPEED;
extern const std::string SPEED;
extern const std::string POSITION;

// Environment Config
extern const std::string ENV_OBSTACLE_PORT;
extern const std::string EXE_LOCATION;
extern const std::string ENV_BOUNDS;
extern const std::string CONFIG_LOCATION;
// Environment Run Data
extern const std::string OBSTACLE_IDS;
extern const std::string OBSTACLE_POS;
extern const std::string OBSTACLE_VEL;
extern const std::string OBSTACLE_ACC;
extern const std::string DURATION;
extern const std::string TARGET_VELOCITY;

// AV Config
extern const std::string AV_NAME;
extern const std::string AV_MOVEMENT_PORT;
extern const std::string AV_EXE_LOCATION;
extern const std::string AV_LOCATION;
extern const std::string AV_ORIENTATION;
extern const std::string AV_BOUNDS;
extern const std::string SENSORS;
// AV Run Data
extern const std::string CLOSEST_ID;
extern const std::string CLOSEST_POS;
extern const std::string CLOSEST_DIST;

// parser
extern const std::string MODULE;
extern const std::string VAST_MODULE;
extern const std::string ENVIRONMENT_MODULE;
extern const std::string AV_MODULE;
extern const std::string MAP;
extern const std::string KEY;
extern const std::string NAME;
extern const std::string VALUE;
