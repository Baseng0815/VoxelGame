#pragma once

struct OreData
{
    int generationCounts = 0;
    int minHeight = 0;
    int maxHeight = 0;
    int size = 0;

    OreData(int count, int minHeight, int maxHeight, int size);
};
