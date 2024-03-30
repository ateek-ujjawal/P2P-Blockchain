#include "BlockThread.h"
#include "Block.h"
#include "Common.h"

#include <thread>
#include <chrono>
#include <iostream>

#define DIFFICULTY 1

BlockThread::BlockThread() {

}

BlockThread::~BlockThread() {

}

void BlockThread::GenerateBlock() {
	Block block;
	char* blockHash;
	while(true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		{
			std::lock_guard<std::mutex> lock(mtx);
			if(!pending_txn.empty()){
				block.AddTransactions(pending_txn);
				pending_txn.clear();	
			}
		}
		
		if(block.GetTList().size() != 0) {
			blockHash = block.GeneratePOW(DIFFICULTY);
			std::cout << blockHash << std::endl;
			block.ResetTransactions();
		}
		
	}
}
