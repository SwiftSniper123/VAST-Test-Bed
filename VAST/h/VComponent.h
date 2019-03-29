#pragma once

/*class that all components of VAST inherit from*/
class VComponent
{
public: 
	/* The base class constructor for all VAST components.*/
	VComponent() {}

	/* The virtual update function*/
	virtual void update() = 0;
};