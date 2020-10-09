#pragma once
#define HASHMAP_N 5
#include <vector>
#include <unordered_map>
#include <string>

class Infoset;

class Params {
public:
	/* hardware params */
	static int nofThreads;
	static const long long nofIters = 300000000ll;
};