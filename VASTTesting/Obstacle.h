#pragma once
#include "VComponent.h"

class Obstacle : public VComponent
{
public:

	Obstacle(Vector3 _position, Vector3 _orientation, Vector3 _dimensions);
	~Obstacle();
	void updateObstacle(Vector3 _position, Vector3 _orientation, Vector3 _dimensions);
	void update(timestamp t, dataMap* dataMap) {};
	void update();
private:
	Vector3 position;
	Vector3 orientation;
	Vector3 dimensions;

};