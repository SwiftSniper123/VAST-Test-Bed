#include "VASTConstants.h"

// system
const char* DATABASE_FILENAME = "VASTDatabase.db";
const char* TEST_DATABASE_FILENAME = "DeleteThisDatabase.db";

// VAST Config
const std::string OUTPUT_FILE = "output_file_location";
const std::string VIZ = "viz_option";
const std::string RUN_ID = "run_id";
const std::string TIME_RATIO = "time_ratio";
const std::string TIME_STEP = "time_step";
const std::string NUM_REPLCATION = "num_replications";
const std::string SEEDS = "seeds";
const std::string MAX_RUN_TIME = "max_run_time";
const std::string AV_LIST = "av_list"; //This does not exist in the config file? // also AV run data
const std::string METRICS = "metrics";

// Scenario Metric type ids
const std::string AVG_SPEED_METRIC_ID = "AvgSpeed";
const std::string AVG_ACCEL_METRIC_ID = "AvgAccel";
const std::string AVG_DECEL_METRIC_ID = "AvgDecel";
const std::string MAX_ACCEL_METRIC_ID = "MaxAccel";
const std::string MIN_ACCEL_METRIC_ID = "MinAccel";

// ScenarioMetric
const std::string ACCELERATION = "Acceleration";
const std::string MIN_ACCELERATION = "MinAcceleration";
const std::string AVG_ACCELERATION = "AvgAcceleration";
const std::string MAX_ACCELERATION = "MaxAcceleration";
const std::string AVG_DECELERATION = "AvgDeceleration";
const std::string AVG_SPEED = "AvgSpeed";
const std::string SPEED = "Speed";
const std::string POSITION = "Position";

// Environment Config
const std::string ENV_OBSTACLE_PORT = "env_obstacle_port";
const std::string EXE_LOCATION = "exe_location";
const std::string ENV_BOUNDS = "Env_bounds";
const std::string CONFIG_LOCATION = "config_location";
// Environment Run Data
const std::string OBSTACLE_IDS = "obstacle_ids";
const std::string OBSTACLE_POS = "obstacle_pos";
const std::string OBSTACLE_VEL = "obstacle_speeds";
const std::string OBSTACLE_ACC = "obstacle_accelerations";
const std::string DURATION = "Duration";
const std::string TARGET_VELOCITY = "TargetVelocity";

// AV Config
const std::string AV_NAME = "av_name";
const std::string AV_MOVEMENT_PORT = "av_movement_port";
const std::string AV_EXE_LOCATION = "exe_location";
const std::string AV_LOCATION = "AV_location";
const std::string AV_ORIENTATION = "AV_orientation";
const std::string AV_BOUNDS = "AV_bounds";
const std::string SENSORS = "sensors";
// AV Run Data
const std::string CLOSEST_ID = "closest_obs";
const std::string CLOSEST_POS = "closest_position";
const std::string CLOSEST_DIST = "closest_distance";

// parser
const std::string MODULE = "module";
const std::string VAST_MODULE = "VAST";
const std::string ENVIRONMENT_MODULE = "Environment";
const std::string AV_MODULE = "AV";
const std::string MAP = "map";
const std::string KEY = "key";
const std::string NAME = "name";
const std::string VALUE = "value";