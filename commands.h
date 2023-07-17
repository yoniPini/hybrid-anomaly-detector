

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>

#include "HybridAnomalyDetector.h"
#include <fstream>
#include <vector>

using namespace std;

class DefaultIO {
public:
  virtual string read() = 0;
  virtual void write(string text) = 0;
  virtual void write(float f) = 0;
  virtual void read(float *f) = 0;
  virtual ~DefaultIO() {}

  // you may add additional methods here
};

// you may add here helper classes
typedef struct resources {
  TimeSeries *train;
  TimeSeries *test;
  HybridAnomalyDetector *hybrid;
  vector<AnomalyReport> ar;
} resources;

// you may edit this class
class Command {
protected:
  string description;
  DefaultIO *dio;

public:
  Command(DefaultIO *dio, const string d) : dio(dio), description(d) {}
  virtual void execute() = 0;
  virtual void disable() {}
  virtual void help() {}
  virtual const string getDescription() const { return description; }
  virtual ~Command() {}
};

class UploadTrainCommand : public Command {
  ofstream out;
  string fileName;

public:
  UploadTrainCommand(DefaultIO *dio, string FN)
      : Command(dio, "Please upload your local train CSV file.\n") {
    fileName = FN;
    out.open(fileName);
  }
  virtual void execute() {
    dio->write(getDescription());
    string line = dio->read();
    while (strcmp(&line[0], "done")) {
      out << line << endl;
      line = dio->read();
    }
    out.close();
    dio->write("Upload complete.\n");
  }
  virtual ~UploadTrainCommand() {
    if (out.is_open())
      out.close();
  }
};

class UploadTestCommand : public Command {
  ofstream out;
  string fileName;

public:
  UploadTestCommand(DefaultIO *dio, string FN)
      : Command(dio, "Please upload your local test CSV file.\n") {
    fileName = FN;
    out.open(fileName);
  }
  virtual void execute() {
    dio->write(getDescription());
    string line = dio->read();
    while (strcmp(&line[0], "done")) {
      out << line << endl;
      line = dio->read();
    }
    out.close();
    dio->write("Upload complete.\n");
  }
  virtual ~UploadTestCommand() {
    if (out.is_open())
      out.close();
  }
};

class UploadCommand : public Command {
  string trainFile;
  string testFile;
  bool isNewTrain;
  bool isNewTest;
  resources *res;
  Command **trainAndTest;

public:
  UploadCommand(DefaultIO *dio, resources *r)
      : Command(dio, "upload a time series csv file") {
    trainAndTest = new Command *[2];
    trainFile = "copyTrain.txt";
    testFile = "copyTest.txt";
    trainAndTest[0] = new UploadTrainCommand(dio, trainFile);
    trainAndTest[1] = new UploadTestCommand(dio, testFile);
    res = r;
    isNewTrain = false;
    isNewTest = false;
  }

  virtual void execute() {
    trainAndTest[0]->execute();
    if (isNewTrain) delete res->train;
    res->train = new TimeSeries(trainFile.c_str());
    isNewTrain = true;
    
    trainAndTest[1]->execute();
    if (isNewTest) delete res->test;
    res->test = new TimeSeries(testFile.c_str());
    isNewTest = true;
  }

  virtual ~UploadCommand() {
    delete (trainAndTest[0]);
    delete (trainAndTest[1]);
    delete[] trainAndTest;
  }
};

class AlgoSettings : public Command {
  float threshold;
  resources *res;
  bool isAlreadyAllocate;

public:
  AlgoSettings(DefaultIO *dio, resources *r)
      : Command(dio, "algorithm settings") {
    res = r;
    threshold = 0.9;
    res->hybrid = new HybridAnomalyDetector(threshold);
    isAlreadyAllocate = true;
  }
  virtual void execute() {
    dio->write("The current correlation threshold is ");
    dio->write(threshold);
    dio->write("\nType a new threshold\n");
    dio->read(&threshold);
    while (threshold < 0.0 || threshold > 1.0) {
      dio->write("please choose a value between 0 and 1.\n");
      dio->write("The current correlation threshold is ");
      dio->write(threshold);
      dio->write("\nType a new threshold\n");
      dio->read(&threshold);
    }
    if (isAlreadyAllocate) delete res->hybrid;
    res->hybrid = new HybridAnomalyDetector(threshold);
    isAlreadyAllocate = true;
  }
  virtual ~AlgoSettings() {}
};

class Detect : public Command {
resources *res;
public:
  Detect(DefaultIO *dio, resources *r) : Command(dio, "detect anomalies") { res = r;}
  virtual void execute() {
    res->hybrid->learnNormal(*(res->train));
    res->ar = res->hybrid->detect(*(res->test));
    dio->write("anomaly detection complete.\n");
  }
  virtual ~Detect() {}
};

class Display : public Command {
resources *res;

public:
  Display(DefaultIO *dio, resources *r) : Command(dio, "display results") { res = r;}
  virtual void execute() {
    for_each(res->ar.begin(), res->ar.end(), [this] (AnomalyReport a) {
      this->dio->write(to_string(a.timeStep) + "\t" + a.description + "\n");
    });
    dio->write("Done.\n");
  }
  virtual ~Display() {}
};

class UploadAnomaliesCommand : public Command {
  ofstream out;
  string fileName;

public:
  UploadAnomaliesCommand(DefaultIO *dio, string FN)
      : Command(dio, "Please upload your local anomalies file.\n") {
    fileName = FN;
  }
  virtual void execute() {
    out.open(fileName);
    dio->write(getDescription());
    string line = dio->read();
    while (strcmp(&line[0], "done")) {
      out << line << endl;
      line = dio->read();
    }
    out.close();
    dio->write("Upload complete.\n");
  }
  virtual ~UploadAnomaliesCommand() {
    if (out.is_open())
      out.close();
  }
};

class UploadAndAnalyze : public Command {
resources *res;
Command *command;
string fileName;
public:
  UploadAndAnalyze(DefaultIO *dio, resources *r)
      : Command(dio, "upload anomalies and analyze results") {
    fileName = "copyAnomalies.txt";
    command = new UploadAnomaliesCommand(dio, fileName);
    res = r;
  }
  
  virtual void execute() {
    command->execute();
    size_t size = res->test->getRowSize();
    bool arr[size];
    for(int i = 0; i < size;i++)
      arr[i] = false;
    
    float realPos = (float) analyzeHelper(arr), pos = 0.0, tp = 0.0, fp = 0.0;
    float realSizeAnomalies = sizeAnomalies(arr, size);
    if (!res->ar.empty()) {
      auto tempo = res->ar.begin();
      int indexI = tempo->timeStep, indexJ = indexI;
      pos++;
      for (auto iter = res->ar.begin() + 1; iter != res->ar.end(); iter++) {
        if ((tempo->description == iter->description) && (iter->timeStep - tempo->timeStep == 1)) {
          indexJ++;
          if (iter + 1 == res->ar.end())
            tp += isTruePositive(arr, indexI, indexJ);
        }
        else {
          tp += isTruePositive(arr, indexI, indexJ);
          indexI = indexJ = iter->timeStep;
          pos++;
        }
        tempo = iter;
      }
    }
    fp = pos - tp;
    float truePositiveRate = tp / realPos, falsePositiveRate = fp / (size - realSizeAnomalies);
    truePositiveRate = cutFraction(truePositiveRate);
    falsePositiveRate = cutFraction(falsePositiveRate);
    dio->write("True Positive Rate: ");
    dio->write(truePositiveRate);
    dio->write("\n");
    dio->write("False Positive Rate: ");
    dio->write(falsePositiveRate);
    dio->write("\n");
  }
  
  int analyzeHelper(bool *arr) {
    int pos = 0;
    ifstream in(fileName);
    string tempo, sNum;
    stringstream ss;
    while (in >> tempo) {
      ss = stringstream(tempo);     
      pos++;
      getline(ss, sNum, ',');
      float begin = stof(sNum);
      getline(ss, sNum, ',');
      float end = stof(sNum);
      for (size_t i = begin; i <= end; i++)
        arr[i] = true;
    }
    in.close();
    return pos;
  }
  
  int isTruePositive(bool *arr, int indexI, int indexJ) {
    for (int i = indexI; i <= indexJ; i++)
      if (arr[i] == true)
        return 1;
    return 0;          
  }
  
  int sizeAnomalies(bool *arr, size_t size) {
    int sum = 0;
    for(int i = 0; i < size; i++)
      if (arr[i] == true) sum++;
    return sum;    
  }
  
  float cutFraction(float num) {
      if (num >= 1.0)
        return 1.0;
      int n = (num * 1000);
      return ((float) n) / 1000;
  }
  
  virtual ~UploadAndAnalyze() {}
};

class Exit : public Command {
resources *res;

public:
  Exit(DefaultIO *dio, resources *r) : Command(dio, "exit") { res = r;}
  virtual void execute() {}
  virtual ~Exit() {
    try {
      delete res->test;
    } catch(const std::exception& e) {}
    try {
      delete res->train;
    } catch(const std::exception& e) {}
    try {
      delete res->hybrid;
    } catch(const std::exception& e) {}
  }
};

class Menu : public Command {
  vector<Command *> menu;
  resources res;

public:
  Menu(DefaultIO *dio)
      : Command(dio, "Welcome to the Anomaly Detection Server.\nPlease choose "
                     "an option:") {
    menu.push_back(new UploadCommand(dio, &res));
    menu.push_back(new AlgoSettings(dio, &res));
    menu.push_back(new Detect(dio, &res));
    menu.push_back(new Display(dio, &res));
    menu.push_back(new UploadAndAnalyze(dio, &res));
    menu.push_back(new Exit(dio, &res));
  }

  virtual void execute() {
    int i;
    do {
      dio->write(this->getDescription() + "\n");
      i = 1;
      for_each(menu.begin(), menu.end(), [this, &i](Command *c) {
        this->dio->write(to_string(i++) + "." + c->getDescription() + "\n");
      });
      float option;
      dio->read(&option);
      i = ((int)option) - 1;
      menu.at(i)->execute();
    } while (i != 5);
  }

  virtual ~Menu() {
    for_each(menu.begin(), menu.end(), [](Command *c) { delete c; });
    delete res.test;
    delete res.train;
    delete res.hybrid;
  }
};

// implement here your command classes

#endif /* COMMANDS_H_ */
