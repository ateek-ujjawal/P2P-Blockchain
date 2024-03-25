#include "ServerStub.h"
#include <cstring>
#include <arpa/inet.h>

ServerStub::ServerStub() {}

void ServerStub::Init(std::unique_ptr<SSocket> socket) {
	this->socket = std::move(socket);
}

int ServerStub::ReceiveAck() {
	char buffer[4];
	int ack = -1;
	
	if (socket->Recv(buffer, sizeof(int), 0)) {
		memcpy(&ack, buffer, sizeof(int));
		ack = ntohl(ack);
	}
	
	return ack;
}

Transaction ServerStub::ReceiveTransaction() {
	char buffer[48];
	Transaction transaction;
	
	if(socket->Recv(buffer, transaction.GetSize(), 0)) {
		transaction.Unmarshal(buffer);
	}
	
	return transaction;
}
