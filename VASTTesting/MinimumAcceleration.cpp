#include "VC_Headers.h"
#include "MinimumAcceleration.h"

using namespace VASTConstants;

void MinimumAcceleration::calculate()
{
	for (int n = 0; n < numAVs; n++)
	{
		if (currentMin[n]->value() > ((Double*)_myMap[n][ACCELERATION])->value())
		{
			currentMin[n]->value(((Double*)_myMap[n][ACCELERATION])->value());
			_myMap[n][MIN_ACCELERATION] = currentMin[n];
		}
	}
}