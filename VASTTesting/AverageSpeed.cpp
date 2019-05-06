#include "VC_Headers.h"
#include "AverageSpeed.h"

using namespace VASTConstants;

void AverageSpeed::calculate()
{
	for (int n = 0; n < numAVs; n++)
	{
		*sumAvgSpeed[n] += ((Double*)_myMap[n][SPEED])->value();
		countSpeedUpdates++;
		_myMap[n][AVG_SPEED] = new Double(sumAvgSpeed[n]->value() / countSpeedUpdates);
	}
}