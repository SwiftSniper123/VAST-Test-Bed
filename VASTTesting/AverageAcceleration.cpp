#include "VC_Headers.h"
#include "AverageAcceleration.h"

 

void AverageAcceleration::calculate()
{
	for (int n = 0; n < numAVs; n++)
	{
		if (((Double*)_myMap[n][ACCELERATION])->value() > 0)
		{
			*sumAcceleration[n] += ((Double*)_myMap[n][ACCELERATION])->value();
			countAccelUpdates++;
			_initialMap[n][AVG_ACCELERATION] = new Double(sumAcceleration[n]->value() / countAccelUpdates);
		}
	}
}