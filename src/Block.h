#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <vector>

#include "Transaction.h"

#define HASH_LENGTH 256

// prev_hash(256) + hash(256) + nonce(4) + transaction_amount(4)
// #define BLOCK_MAX_SIZE 4096

// #define MAX_TXN_SIZE ((BLOCK_MAX_SIZE - 2 * HASH_LENGTH - 2 * sizeof(int)) / TXN_SIZE)

class Block {
private:
	/* data */
	char prev_hash[HASH_LENGTH];
	char hash[HASH_LENGTH];
	int nonce;
	int transaction_amount;
	std::vector<Transaction> t_list;

public:
	Block();
	Block(char* prev_hash, char* hash, int nonce, int transaction_amount, std::vector<Transaction> t_list);

	~Block();

	char* GetPrevHash();
	char* GetHash();
	int GetNonce();
	int GetTransactionAmount();
	std::vector<Transaction> GetTList();

	int GetSize();

	void Marshal(char* buffer);
	void Unmarshal(char* buffer);

	void Print();

	bool IsValid();
};

#endif
