#include "VC_Headers.h"
#include "AverageDeacceleration.h"

using namespace VASTConstants;

void AverageDeacceleration::calculate()
{
	for (int n = 0; n < numAVs; n++)
	{
		if (((Double*)_myMap[n][ACCELERATION])->value() < 0)
		{
			*sumDeacceleration[n] += ((Double*)_myMap[n][ACCELERATION])->value();
			countDecelUpdates++;
			_initialMap[n][AVG_DECELERATION] = new Double(sumDeacceleration[n]->value() / countDecelUpdates);
		}
	}
}