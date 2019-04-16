#pragma once
#include "VComponent.h"
#include "VType.h"

/*base class for all metrics in VAST*/
class ScenarioMetric : public VComponent
{
public:
	/* default constructor*/
	ScenarioMetric() {};

	/* returns the statistical metric value as defined by the extending metric 
	class.*/
	virtual VType calculate();
};
