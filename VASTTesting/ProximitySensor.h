#pragma once
#include "Sensor.h"
#include "AV.h"
#include <cmath>

using std::pair;
typedef std::invalid_argument InvalidArgumentException;
typedef pair<String*, Vector3*> locatedObstacle;
typedef Double distance;

static const string SENSOR_LOC = "sensor_location"; 
static const string SENSOR_QUAD = "sensor_viewquadrant";
static const string SENSOR_DEPTH = "sensor_viewdepth";
static const string OBSTACLE_IDS = "obstacle_ids";
static const string OBSTACLE_POS = "obstacle_pos";
static const string CLOSEST_ID = "closest_obs";

/* Senses objects in the environment based on its orientation (i.e. Quad 1 is positive x and positive y),
as well as it can only sense out to a certain depth.  Obstacles outside that depth are not added to the 
ProximitySensor's list of obstacles in view.  When the AV position changes, the update to the sensor location
translates to the coordinate quadrant it can sense.  */
class ProximitySensor : public Sensor
{
private:
	AV* _owningAV = nullptr;

	dataMap _currentData;

	timestamp _lastUpdateTime = -1.0;

	locatedObstacle* _lastClosestObstacle = nullptr;

	distance* _lastClosestProximity = nullptr;
public:
	/*PoximitySensor constructor - sets owning AV avatar and the initial 
	state of the world.  Sets 	the name of this component as 
	"ProximitySensor."*/
	ProximitySensor(AV* owningAV, dataMap initialDataMap);

	/* Destructor clears data maps.*/
	~ProximitySensor();

	/* Does not update this Sensor; this sensor waits for a call to AVUpdate from its AV_Avatar.*/
	virtual void update(timestamp t, dataMap dataMap) { /* do nothing */};

	/* Replaces Update function; called by the owning AV avatar to update the Proximity Sensor with the latest timeslice*/
	void AVUpdate(timestamp t, dataMap dataMap);

	/* After AVUpdate brought in new obstacles, now reporting to AV sends the latest calculated proximity data to the AV avatar.*/
	void reportToAV();

	/* Resets the data for the Proximity Sensor to prepare for another replication.*/
	virtual void stopReplication(bool another, string runID) ;

	/* Assesses the obstacles and generates the list of obstacles in view, the closest 
	object, and the closest distance.*/
	void scan();

	/* Returns the latest set of obstacles in view of the Sensor.*/
	Array* getObstaclesInView() { return nullptr; };

	/* Returns the latest located obstacle with the closest distance in 3D space.*/
	pair<String*, Vector3*>* getClosestObstacle() { return nullptr; 	};

	/* Returns the latest, closest proximity.*/
	Double* getClosestProximity() { return nullptr; }
};