#ifndef __COMMON_H__
#define __COMMON_H__

#include <vector>
#include <mutex>
#include "Transaction.h"

extern std::vector<Transaction> pending_txn;
extern std::mutex mtx;
	
#endif
