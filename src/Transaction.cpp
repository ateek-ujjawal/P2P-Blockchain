#include <cstring>
#include <iostream>

#include <arpa/inet.h>
#include "Transaction.h"

Transaction::Transaction() {
	id = -1;
	amount = -1;
}

int Transaction::GetID() { return id; }
char* Transaction::GetSender() { return sender; }
char* Transaction::GetReceiver() { return receiver; }
int Transaction::GetAmount() { return amount; }

int Transaction::GetSize() {
	return sizeof(id) + NAME_SIZE + NAME_SIZE + sizeof(amount);
}

void Transaction::SetTransaction(int txn_id, char *txn_sender, int sender_len, char *txn_receiver, int receiver_len, int txn_amount) {
	id = txn_id;
	
	if(sender != NULL) {
		memset(sender, 0, NAME_SIZE);
		memcpy(sender, txn_sender, sender_len);
	}
	
	if (receiver != NULL) {
		memset(receiver, 0, NAME_SIZE);
		memcpy(receiver, txn_receiver, receiver_len);
	}
	
	amount = txn_amount;
}

void Transaction::Marshal(char *buffer) {
	int net_id = htonl(id);
	int net_amount = htonl(amount);
	int offset = 0;
	
	memcpy(buffer + offset, &net_id, sizeof(net_id));
	offset += sizeof(net_id);
	memcpy(buffer + offset, sender, NAME_SIZE);
	offset += NAME_SIZE;
	memcpy(buffer + offset, receiver, NAME_SIZE);
	offset += NAME_SIZE;
	memcpy(buffer + offset, &net_amount, sizeof(net_amount));
}

void Transaction::Unmarshal(char *buffer) {
	int net_id;
	int net_amount;
	
	int offset = 0;
	memcpy(&net_id, buffer + offset, sizeof(net_id));
	offset += sizeof(net_id);
	memcpy(sender, buffer + offset, NAME_SIZE);
	offset += NAME_SIZE;
	memcpy(receiver, buffer + offset, NAME_SIZE);
	offset += NAME_SIZE;
	memcpy(&net_amount, buffer + offset, sizeof(net_amount));
	
	id = ntohl(net_id);
	amount = ntohl(net_amount);
}
