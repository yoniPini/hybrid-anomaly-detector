//g++ -pthread -std=c11
#include "Server.h"

Server::Server(int port)throw (const char*) {
	fd = socket(AF_INET, SOCK_STREAM,0);
	if (fd < 0) throw "unable to open socket";
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	if(bind(fd, (sockaddr *) &server, sizeof(server)) < 0) throw "unable to bind";
	if(listen(fd, 2) < 0) throw "unable to listen";
	isStopped = false;
}

void Server::start(ClientHandler& ch)throw(const char*){
	t = new thread([this, &ch] () {
		socklen_t clientSize = sizeof(aClient);
		while (!isStopped) {
			int clientFD = accept(fd, (sockaddr *) &aClient, &clientSize); // the problem is here in access functino
			//which is a blocking call and therefore  the thread doesnt get to check the condition.
			if (clientFD < 0) throw "unable to accept client";
			ch.handle(clientFD);
			close(clientFD);
		}
	});
}

void Server::stop() {
	isStopped = true;
	t->join(); // do not delete this!
	
	close(this->fd);
}

Server::~Server() {
}

