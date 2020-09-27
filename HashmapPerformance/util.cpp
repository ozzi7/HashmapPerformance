#include "util.h"

void Util::GetThreadWorkItems(std::vector<std::pair<uint64_t, uint64_t>>& loop_indices, const uint64_t start, const uint64_t end_excl)
{
	uint64_t minItems = (end_excl - start) / Params::nofThreads;
	uint64_t extraItems = (end_excl - start) % Params::nofThreads;

	for (uint64_t idx = 0; idx < Params::nofThreads; ++idx)
	{
		if (idx < extraItems) {
			loop_indices[idx] = std::make_pair(start + (minItems + 1) * idx, start + (minItems + 1) * (idx + 1));
		}
		else {
			loop_indices[idx] = std::make_pair(start + (minItems * idx) + extraItems, start + (minItems * idx) + extraItems + minItems);
		}
	}
}
void Util::GetThreadWorkItems(std::vector<std::pair<uint64_t, uint64_t>>& loop_indices, const uint64_t start, const uint64_t end_excl, int maxThreads)
{
	uint64_t minItems = (end_excl - start) / maxThreads;
	uint64_t extraItems = (end_excl - start) % maxThreads;

	for (uint64_t idx = 0; idx < maxThreads; ++idx)
	{
		if (idx < extraItems) {
			loop_indices[idx] = std::make_pair(start + (minItems + 1) * idx, start + (minItems + 1) * (idx + 1));
		}
		else {
			loop_indices[idx] = std::make_pair(start + (minItems * idx) + extraItems, start + (minItems * idx) + extraItems + minItems);
		}
	}
}
long long Util::RandomLL(const long long& min, const long long& max_excl)
{
	static thread_local std::mt19937* generator = nullptr;
	if (!generator) generator = new std::mt19937(clock() + std::hash<std::thread::id>()(std::this_thread::get_id()));
	std::uniform_int_distribution<long long> distribution(min, max_excl - 1); // should be lightweight but maybe we can test to keep this thread local too
	return distribution(*generator);
}