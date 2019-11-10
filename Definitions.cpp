#include "pch.h"

#include "Definitions.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::array<BlockData, NUM_BLOCKS> Definitions::BLOCK_DATA;

void Definitions::loadData() {
	std::ifstream file("Resources/Misc/BlockData.json");

	// not found by intellisense...actually correct
	json root = json::parse(file);

	std::string ind;
	BlockData blockData;
	int i = 0;
	for (auto it = root.begin(); it != root.end(); it++) {
		ind = std::to_string(i);
		blockData.tid_up = (*it)["tid_up"].get<int>();
		blockData.tid_px = (*it)["tid_px"].get<int>();
		blockData.tid_nx = (*it)["tid_nx"].get<int>();
		blockData.tid_pz = (*it)["tid_pz"].get<int>();
		blockData.tid_nz = (*it)["tid_nz"].get<int>();
		blockData.tid_bot = (*it)["tid_bt"].get<int>();
		Definitions::BLOCK_DATA[i] = blockData;
		i++;
	}
}