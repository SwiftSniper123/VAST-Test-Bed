#pragma once
#include "vector3.h"
#include "EventTree.h"
#include "VComponent.h"
#include <string>

//using std::string;

class Obstacle : public VComponent
{
public:

	/* Constructor for Obstacle class.

		 vector3   _position      This is the initial position of the obstacle.

		 Vector3  _orientation  This is the initial orientation of the obstacle, according to the 3D model.

		 Vector3  _dimensions  This is the bounding box dimensions for this obstacle.

	*/

	Obstacle(vector3 _position, vector3 _orientation, vector3 _dimensions);
	/* The destructor for the Obstacle object.  When this is called, the object is deleted. */
	~Obstacle();
	//void updateObstacle(vector3 _position, vector3 _orientation, vector3 _dimensions); 
	void update(double time, Event evt);
	std::string getName();
	vector3 getPosition();
	vector3 getOrientation();
	vector3 getDimension();
private:
	std::string name;
	vector3 position;
	vector3 orientation;
	vector3 dimensions;
};