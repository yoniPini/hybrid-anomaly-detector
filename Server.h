
#ifndef SERVER_H_
#define SERVER_H_

#include "commands.h"
#include "CLI.h"
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>


using namespace std;

class SocketIO : public DefaultIO {
	int clientFD;
	char buffer[1024];
	public:
	SocketIO(int c) : clientFD(c) {}
	
	virtual string read() {
		bzero(buffer, 1024);
		char c, line = '\n';
		recv(clientFD, &c,1, 0);
		int i = 0;
		while (c != line) {
			buffer[i] = c;
			i++;
			recv(clientFD, &c,1, 0);
		}
		
		string s(buffer);
		return s;
	}

  virtual void read(float *f) {
		bzero(buffer, 1024);
		char c, line = '\n';
		recv(clientFD, &c,1, 0);
		int i = 0;
		while (c != line) {
			buffer[i] = c;
			i++;
			recv(clientFD, &c,1, 0);
		}

		string s(buffer);
		*f = stof(s);
	}

  virtual void write(string text) {
		//send(clientFD, &(text[0]), text.size(), 0);
		send(clientFD, text.c_str(), text.size(), 0);
	}

  virtual void write(float f) {
		string s = ftoString(f);
		//send(clientFD, &(s[0]), s.size(), 0);
		send(clientFD, s.c_str(), s.size(), 0);
	}

	string ftoString(float f) {
		int num = f;
		char c = '.';
		string s = to_string(num);
		s.push_back(c);
		f -= num;
		f++;
		string tempo = to_string((int)(f * 1000));
		tempo.erase(tempo.begin());
		s.append(tempo);

		auto iter = s.end() - 1;
		while (iter != s.begin() + 2) {
			if (*iter == '0')
				s.erase(iter);
			else {break;}
			iter--;
		}
		return s;
	}
	
};


// edit your ClientHandler interface here:
class ClientHandler {
protected:
	DefaultIO *dio;
public:
  virtual void handle(int clientID) = 0;
};

// you can add helper classes here and implement on the cpp file

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler : public ClientHandler {
public:
  virtual void handle(int clientID) {
		dio = new SocketIO(clientID);
		CLI cli(dio);
    cli.start();
		delete dio;
	}
};

// implement on Server.cpp
class Server {
  thread *t; // the thread to run the start() method in
  int fd;
  sockaddr_in server;
  sockaddr_in aClient;
  volatile bool isStopped;
  // you may add data members

public:
  Server(int port) throw(const char *);
  virtual ~Server();
  void start(ClientHandler &ch) throw(const char *);
  void stop();
};

#endif /* SERVER_H_ */
