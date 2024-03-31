#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include <string>

#define NAME_SIZE 20

// Transaction fields: id(4 bytes) sender(NAME_SIZE bytes, currently 20) receiver(NAME_SIZE bytes) amount(4 bytes)
#define TXN_SIZE (2 * NAME_SIZE + 2 * sizeof(int))

class Transaction {
private:
	int id;
	char sender[NAME_SIZE];
	char receiver[NAME_SIZE];
	int amount;

public:
	Transaction();
	int GetID();
	char *GetSender();
	char *GetReceiver();
	int GetAmount();
	int GetSize();

	void SetTransaction(int txn_id, char *txn_sender, int sender_len, char *txn_receiver, int receiver_len, int txn_amount);

	void Marshal(char *buffer);
	void Unmarshal(char *buffer);

	void Print();

	bool IsValid();

	std::string ToString();
};

#endif
