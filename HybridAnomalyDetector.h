

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	HybridAnomalyDetector(float t);
	virtual ~HybridAnomalyDetector();
	virtual	vector<AnomalyReport> detect(const TimeSeries &ts);
	virtual void learnNormal(const TimeSeries &ts);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
