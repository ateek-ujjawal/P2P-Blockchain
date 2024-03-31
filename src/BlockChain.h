#ifndef __BLOCKCHAIN_H__
#define __BLOCKCHAIN_H__

#include <string>
#include <unordered_map>
#include <vector>

#include "Block.h"

struct BlockChain_Ele {
	int level;
	Block *blk;
};

class BlockChain {
private:
	// key: hash string
	std::unordered_map<std::string, BlockChain_Ele *> blk_mp;
	// last_ele is the last blockchain element of the main chain
	BlockChain_Ele *last_ele;

public:
	BlockChain(/* args */);
	~BlockChain();
	std::vector<Block *> GetMainChain();
	bool AddBlock(Block *blk);
	char *GetLastHash();
};

#endif