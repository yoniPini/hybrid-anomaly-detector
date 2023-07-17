#include "timeseries.h"

  
  TimeSeries::TimeSeries(const char *CSVfileName) {
    ifstream in(CSVfileName);
    string head;
    in >> head;
    string att;
    stringstream hss(head);
    while (getline(hss, att, ',')) {
      vector<float> v;
      ts[att] = v;
      atts.push_back(att);
    }

    while (!in.eof()) {
      string line;
      in >> line;
      string val;
      stringstream lss(line);
      int i = 0;
      while (getline(lss, val, ',')) {
        ts[atts[i]].push_back(stof(val));
        i++;
      }
    }
    in.close();

    dataRowSize = ts[atts[0]].size();
  }


	size_t TimeSeries::getRowSize() const {
		return dataRowSize;
	}

  const vector<string>& TimeSeries::gettAttributes()const{
		return atts;
	}

  const vector<float>& TimeSeries::getAttributeData(string name)const{
		return ts.at(name);
	}
