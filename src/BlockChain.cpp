#include "BlockChain.h"

#include <algorithm>

BlockChain::BlockChain(/* args */) {
	last_ele = nullptr;
}

BlockChain::~BlockChain() {
}

std::vector<Block *> BlockChain::GetMainChain() {
	std::vector<Block *> chain;
	BlockChain_Ele *last = last_ele;

	while (last) {
		chain.emplace_back(last->blk);
		std::string prev_hash = last->blk->GetPrevHash();
		if (blk_mp.count(prev_hash)) {
			last = blk_mp[prev_hash];
		} else {
			break;
		}
	}

	std::reverse(chain.begin(), chain.end());

	return chain;
}

bool BlockChain::AddBlock(Block *blk) {
	std::string prev_hash = blk->GetPrevHash();
	BlockChain_Ele *prev_ele = nullptr;

	if (!blk_mp.count(prev_hash)) {
		return false;
	}

	prev_ele = blk_mp[prev_hash];

	// may toss the block when level is smaller than max_level - 1 here

	BlockChain_Ele *bc_ele = new BlockChain_Ele;
	bc_ele->level = prev_ele->level + 1;
	bc_ele->blk = blk;

	blk_mp[blk->GetHash()] = bc_ele;

	// if current level is greater than the current main chain length, update the last element of main chain
	if (bc_ele->level > last_ele->level) {
		last_ele = bc_ele;
	}

	return true;
}