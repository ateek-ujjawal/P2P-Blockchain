#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "Transaction.h"
#include <vector>
#include <string>

#define HASH_LENGTH 256

#define BLOCK_MAX_SIZE 4096

class Block {
private:
	/* data */
	char prev_hash[HASH_LENGTH];
	char hash[HASH_LENGTH];
	int nonce;
	int transaction_amount;
	std::vector<Transaction> t_list;
	
	//Generates a sha256 hash of this block
	std::string GenerateHash();
public:
	Block(/* args */);
	~Block();

	char* GetPrevHash();
	int GetNonce();
	int GetTransactionAmount();
	std::vector<Transaction> GetTList();

	int GetSize();

	void Marshal(char* buffer);
	void Unmarshal(char* buffer);
	
	void AddTransactions(std::vector<Transaction> txn_list);
	void ResetTransactions();
	
	//Generate Proof-of-Work(Returns the sha256 hash that matches the difficulty set)
	char* GeneratePOW(int difficulty);

	void Print();
};

#endif
