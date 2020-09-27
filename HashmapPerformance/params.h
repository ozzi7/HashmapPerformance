#pragma once
#define HASHMAP_N 5
#include <vector>
#include <unordered_map>
#include <string>

class Infoset;

class Params {
public:
	/* hardware params */
	static const int nofThreads = 1;
	static const long long nofIters = 50000000ll;
};