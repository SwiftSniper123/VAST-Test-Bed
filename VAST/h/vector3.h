#pragma once
/*struct that stores x, y, and z values in a common variable*/
struct vector3
{
	/*default constructor to initialize variables to zero*/
	vector3()
	{ 
		x, y, z = 0; 
	};
	/*constructor to see values of x, y, and z*/
	vector3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	/*variables that store the x, y, and z values*/
	float x, y, z;
};
