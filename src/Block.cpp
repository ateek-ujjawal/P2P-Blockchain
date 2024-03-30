#include "Block.h"
#include "sha256.h"

#include <cstring>
#include <iostream>
#include <arpa/inet.h>

#define HASH_LENGTH 256

Block::Block(/* args */) {
}

Block::~Block() {
}

char* Block::GetPrevHash() { return prev_hash; }
int Block::GetNonce() { return nonce; }
int Block::GetTransactionAmount() { return transaction_amount; }
std::vector<Transaction> Block::GetTList() { return t_list; }

int Block::GetSize() {
	int size = HASH_LENGTH + sizeof(nonce);
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
	memcpy(buffer + offset, &net_nonce, sizeof(net_nonce));
	offset += sizeof(net_nonce);
	memcpy(buffer + offset, &net_transaction_amount, sizeof(net_transaction_amount));
	offset += sizeof(net_transaction_amount);

	for (auto& txn : t_list) {
		int t_size = txn.GetSize();
		char t_buffer[t_size];
		txn.Marshal(buffer);
		memcpy(buffer + offset, t_buffer, t_size);
		offset += t_size;
	}
}
void Block::Unmarshal(char* buffer) {
	int net_nonce;
	int net_transaction_amount;
	int offset = 0;

	memcpy(prev_hash, buffer + offset, sizeof(HASH_LENGTH));
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

void Block::AddTransactions(std::vector<Transaction> txn_list) {
	t_list = txn_list;
}

void Block::ResetTransactions() {
	t_list.clear();
}

std::string Block::GenerateHash() {
	std::string input;
	for (auto& txn : t_list) {
		input += std::to_string(txn.GetID()) + std::string(txn.GetSender()) + std::string(txn.GetReceiver()) + std::to_string(txn.GetAmount());
	}
	input += std::to_string(nonce);
	return sha256(input);
}

//Generate Proof-of-work with given difficulty
//TODO: Nonce here starts from zero but need to make it random for future tests
char* Block::GeneratePOW(int difficulty) {
	std::string diff_string(difficulty, '0');
	nonce = 0;
	std::string outputHash = GenerateHash();
	nonce++;
	while(std::strncmp(diff_string.c_str(), outputHash.c_str(), difficulty) != 0) {
		outputHash = GenerateHash();
		nonce++;
	}
	memset(hash, 0, HASH_LENGTH);
	memcpy(hash, (char*) outputHash.c_str(), HASH_LENGTH);
	return hash;
}

void Block::Print() {
	std::cout << "prev_hash " << prev_hash << " ";
	std::cout << "nonce " << nonce << " ";
	std::cout << "transactions " << std::endl;
	for (auto& txn : t_list) {
		txn.Print();
	}
}
