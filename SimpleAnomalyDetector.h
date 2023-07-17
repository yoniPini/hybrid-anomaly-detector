/*
 * SimpleAnomalyDetector.h
 *
 *  Created on: 8 ����� 2020
 *      Author: Eli
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include "minCircle.h"

struct correlatedFeatures{
	float corrlation;
	float threshold;
	Point* center;
	Line lin_reg;
	Circle circle;
	string feature1,feature2;  // names of the correlated features
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
	vector<correlatedFeatures> cf;
	float threshold;
public:
	SimpleAnomalyDetector();
	SimpleAnomalyDetector(float t);
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
	void setCorrelationThreshold(float threshold){
		this->threshold=threshold;
	}

	// helper methods
protected:
	virtual void learnHelper(const TimeSeries& ts,float p/*pearson*/,string f1, string f2,Point** ps);
	virtual bool isAnomalous(float x, float y,correlatedFeatures c);
	Point** toPoints(vector<float> x, vector<float> y);
	float findThreshold(Point** ps,size_t len,Line rl);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
