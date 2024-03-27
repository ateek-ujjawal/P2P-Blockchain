#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "Transaction.h"

#define HASH_LENGTH 256

#define BLOCK_MAX_SIZE 4096

class Block {
private:
	/* data */
	char prev_hash[HASH_LENGTH];
	int nonce;
	int transaction_amount;
	std::vector<Transaction> t_list;

public:
	Block(/* args */);
	~Block();

	char* GetPrevHash();
	int GetNonch();
	int GetTransactionAmount();
	std::vector<Transaction> GetTList();

	int GetSize();

	void Marshal(char* buffer);
	void Unmarshal(char* buffer);

	void Print();
};

#endif