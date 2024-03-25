#include "ClientStub.h"

#include <arpa/inet.h>

#include <cstring>

ClientStub::ClientStub() {}

int ClientStub::Init(std::string ip, int port) {
	return socket.Init(ip, port);
}

int ClientStub::SendTransaction(Transaction txn) {
	char buffer[48];
	txn.Marshal(buffer);
	return socket.Send(buffer, txn.GetSize(), 0);
}

int ClientStub::SendAck() {
	int ack = 0;
	int size = 4;
	char buffer[size];
	
	int net_ack = htonl(ack);
	memcpy(buffer, &net_ack, sizeof(net_ack));
	return socket.Send(buffer, size, 0);
}
