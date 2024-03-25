#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include <string>

#define NAME_SIZE 20

class Transaction {
private:
	int id;
	char sender[NAME_SIZE];
	char receiver[NAME_SIZE];
	int amount;
	
public:
	Transaction();
	int GetID();
	char* GetSender();
	char* GetReceiver();
	int GetAmount();
	int GetSize();
	
	void SetTransaction(int txn_id, char *txn_sender, int sender_len, char *txn_receiver, int receiver_len, int txn_amount);
	
	void Marshal(char *buffer);
	void Unmarshal(char *buffer);
};

#endif
