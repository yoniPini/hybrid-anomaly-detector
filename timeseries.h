/*
 * timeseries.h
 *
 *  Created on: 26 ����� 2020
 *      Author: Eli
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <algorithm>
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string.h>
#include <vector>


using namespace std;

class TimeSeries {
  map<string, vector<float>> ts;
  vector<string> atts;
  size_t dataRowSize;

public:
  TimeSeries(const char *CSVfileName);

  const vector<float> &getAttributeData(string name) const;

  const vector<string> &gettAttributes() const;

  size_t getRowSize() const;

  ~TimeSeries() {}
};

#endif /* TIMESERIES_H_ */
