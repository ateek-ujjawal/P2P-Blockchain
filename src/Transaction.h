#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include <string>

typedef struct Transaction {
	int id;
	std::string sender;
	std::string receiver;
	int amount;
} transaction_t;

#endif
