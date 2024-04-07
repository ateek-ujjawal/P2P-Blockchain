#include "PeerStub.h"

#include <arpa/inet.h>

#include <cstring>

PeerStub::PeerStub(/* args */) {
}

PeerStub::~PeerStub() {
}

int PeerStub::Init(std::string ip, int port) {
	return socket.Init(ip, port);
}

int PeerStub::SendTransaction(Transaction txn) {
	char buffer[48];
	txn.Marshal(buffer);
	return socket.Send(buffer, txn.GetSize(), MSG_NOSIGNAL);
}

int PeerStub::SendBlock(Block blk) {
	int sz = blk.GetSize();
	int net_sz = htonl(sz);
	char sizebuf[4];
	
	memcpy(sizebuf, &net_sz, sizeof(net_sz));
	int response = socket.Send(sizebuf, sizeof(net_sz), MSG_NOSIGNAL);
	
	if(response) {
		Block b = blk;
		char buffer[BLOCK_MAX_SIZE];
		blk.Marshal(buffer);
		return socket.Send(buffer, sz, MSG_NOSIGNAL);
	} else 
		return response;
}

int PeerStub::SendAck(int ack) {
	int size = 4;
	char buffer[size];

	int net_ack = htonl(ack);
	memcpy(buffer, &net_ack, sizeof(net_ack));
	return socket.Send(buffer, size, MSG_NOSIGNAL);
}
