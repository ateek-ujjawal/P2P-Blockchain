#include "Block.h"

#include <arpa/inet.h>

#include <cstring>
#include <iostream>

Block::Block(/* args */) {
	nonce = -1;
}

Block::Block(char* prev_hash, char* hash, int nonce, int transaction_amount, std::vector<Transaction> t_list) {
	memcpy(this->prev_hash, prev_hash, HASH_LENGTH);
	memcpy(this->hash, hash, HASH_LENGTH);
	this->nonce = nonce;
	this->transaction_amount = transaction_amount;
	this->t_list = t_list;
}

Block::~Block() {
}

char* Block::GetPrevHash() { return prev_hash; }
char* Block::GetHash() { return hash; }
int Block::GetNonce() { return nonce; }
int Block::GetTransactionAmount() { return transaction_amount; }
std::vector<Transaction> Block::GetTList() { return t_list; }

int Block::GetSize() {
	int size = 2 * HASH_LENGTH + sizeof(nonce) + sizeof(transaction_amount);
	for (auto& txn : t_list) {
		size += txn.GetSize();
	}

	return size;
}

void Block::Marshal(char* buffer) {
	int net_nonce = htonl(nonce);
	int net_transaction_amount = htonl(transaction_amount);
	int offset = 0;

	memcpy(buffer + offset, prev_hash, HASH_LENGTH);
	offset += HASH_LENGTH;
	memcpy(buffer + offset, hash, HASH_LENGTH);
	offset += HASH_LENGTH;
	memcpy(buffer + offset, &net_nonce, sizeof(net_nonce));
	offset += sizeof(net_nonce);
	memcpy(buffer + offset, &net_transaction_amount, sizeof(net_transaction_amount));
	offset += sizeof(net_transaction_amount);

	for (auto& txn : t_list) {
		int t_size = txn.GetSize();
		char t_buffer[t_size];
		txn.Marshal(t_buffer);
		memcpy(buffer + offset, t_buffer, t_size);
		offset += t_size;
	}
}
void Block::Unmarshal(char* buffer) {
	int net_nonce;
	int net_transaction_amount;
	int offset = 0;

	memcpy(prev_hash, buffer + offset, HASH_LENGTH);
	offset += HASH_LENGTH;
	memcpy(hash, buffer + offset, HASH_LENGTH);
	offset += HASH_LENGTH;
	memcpy(&net_nonce, buffer + offset, sizeof(net_nonce));
	offset += sizeof(net_nonce);
	memcpy(&net_transaction_amount, buffer + offset, sizeof(net_transaction_amount));
	offset += sizeof(net_transaction_amount);

	nonce = ntohl(net_nonce);
	transaction_amount = ntohl(net_transaction_amount);

	for (int i = 0; i < transaction_amount; i++) {
		Transaction txn;
		txn.Unmarshal(buffer + offset);
		offset += txn.GetSize();
		t_list.push_back(txn);
	}
}

bool Block::IsValid() {
	return (nonce != -1);
}

void Block::Print() {
	std::cout << "prev_hash " << prev_hash << std::endl;
	std::cout << "hash " << hash << std::endl;
	std::cout << "nonce " << nonce << std::endl;
	std::cout << "transactions " << std::endl;
	for (auto& txn : t_list) {
		txn.Print();
	}
}
