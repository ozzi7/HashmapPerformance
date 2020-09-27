#pragma once

#include <windows.h>
#include <iostream>
#include <random>
#include <execution>
#include <algorithm>
#include <sstream>

#include "params.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

class Util
{
public:
	static void GetThreadWorkItems(std::vector<std::pair<uint64_t, uint64_t>>& loop_indices, uint64_t start, uint64_t end);
	static void GetThreadWorkItems(std::vector<std::pair<uint64_t, uint64_t>>& loop_indices, const uint64_t start, const uint64_t end_excl, int maxThreads);
	static long long RandomLL(const long long& min, const long long& max);
};