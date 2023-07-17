
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector(): SimpleAnomalyDetector(0.5) {}

HybridAnomalyDetector::HybridAnomalyDetector(float t): SimpleAnomalyDetector(t) {}

HybridAnomalyDetector::~HybridAnomalyDetector() {}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
  vector<AnomalyReport> vecAR;
  for (correlatedFeatures tempo : cf) {
    if (tempo.corrlation > 0.9) {
        vector<float> x = ts.getAttributeData(tempo.feature1);
        vector<float> y = ts.getAttributeData(tempo.feature2);
        for (int i = 0; i < y.size(); i++)
          if (isAnomalous(x[i], y[i], tempo))
            vecAR.push_back(AnomalyReport(tempo.feature1 + "-" + tempo.feature2, i + 1));
    }
    else if (tempo.corrlation > 0.5) {
        vector<float> x = ts.getAttributeData(tempo.feature1);
        vector<float> y = ts.getAttributeData(tempo.feature2);
        for (int i = 0; i < y.size(); i++)
          if (isAnomalousCircle(x[i], y[i], tempo.circle))
            vecAR.push_back(AnomalyReport(tempo.feature1 + "-" + tempo.feature2, i + 1));
    }
  }
  return vecAR;
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
  SimpleAnomalyDetector::learnNormal(ts);
  for (int i = 0; i < cf.size(); i++) {
    if (cf[i].corrlation < threshold && cf[i].corrlation >= 0.5) {
      cf[i].circle = findMinCircle(ts.getAttributeData(cf[i].feature1).data(),
                                   ts.getAttributeData(cf[i].feature2).data(),
                                   ts.getAttributeData(cf[i].feature1).size());
      cf[i].circle.radius *= 1.1;
    }
  }
}
