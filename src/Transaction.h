#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include <string>

/* Did not use string for sender and receiver as it was giving segfault on server side */
struct Transaction {
	int id;
	int sender;
	int receiver;
	int amount;
};

#endif
