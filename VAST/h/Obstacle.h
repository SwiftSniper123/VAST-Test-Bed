#pragma once
#include "vector3.h"
#include "VComponent.h"
class Obstacle : public VComponent
{
public:

	Obstacle(vector3 _position, vector3 _orientation, vector3 _dimensions);
	~Obstacle();
	void updateObstacle(vector3 _position, vector3 _orientation, vector3 _dimensions);
	void update();
private:
	vector3 position;
	vector3 orientation;
	vector3 dimensions;

};