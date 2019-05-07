#include "VC_Headers.h"
#include "MaximumAcceleration.h"

 

void MaximumAcceleration::calculate()
{
	for (int n = 0; n < numAVs; n++)
	{
		if (currentMax[n]->value() < ((Double*)_myMap[n][ACCELERATION])->value())
		{
			currentMax[n]->value(stod(_myMap[n][ACCELERATION]->s_value()));
			_myMap[n][MAX_ACCELERATION] = currentMax[n];
		}
	}
}
