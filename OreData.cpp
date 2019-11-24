#include "pch.h"
#include "OreData.h"

OreData::OreData(int count, int minHeight, int maxHeight, int size) {
	generationCounts = count;
	this->minHeight = minHeight;
	this->maxHeight = maxHeight;
	this->size = size;
}
