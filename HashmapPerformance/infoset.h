#pragma once

#include <vector>
#include <numeric>
#define max_macro(a, b) (((a) < (b)) ? (b) : (a))

class Infoset
{
public:
	Infoset();
	Infoset(int actions, bool trackAverageStrategy = false);
	~Infoset();

	std::vector<float> CalculateStrategy();
	std::vector<float> GetFinalStrategy();

	std::vector<float> regret = std::vector<float>();
	std::vector<float> actionCounter = std::vector<float>(); // only for round 0

	bool hasActionCounter = false;
	bool isFrozen = false;
};