#define _HAS_STD_BYTE 0
//#define DEBUG
#define _CRTDBG_MAP_ALLOC
#define HASHMAP_N 5

#include <algorithm>
#include <iostream>
#include <filesystem>
#include <iostream>
#include <chrono>
#include <vector>
#include <crtdbg.h>
#include <algorithm>
#include <thread>
#include <math.h>
#include <execution>

#include "libcuckoo/cuckoohash_map.hh"
#include "parallel_hashmap/phmap.h"
#include "params.h"
#include "infoset.h"
#include "util.h"
#include "progress_bar.hpp"

void test_performance();

int main(int argc, char* argv[])
{
	test_performance();
	//_CrtDumpMemoryLeaks();
}
void test_performance()
{
	// required for c++ parallel loops
	std::vector<std::pair<uint64_t, uint64_t>> loop_indices(Params::nofThreads); // (0,10000) (10000, 20000), 
	Util::GetThreadWorkItems(loop_indices, 0, Params::nofIters);

	// hashmaps
	std::unordered_map<long long, Infoset> stdmap;
	//libcuckoo::cuckoohash_map<long long, Infoset> libcuckooMap;
	/*phmap::node_hash_map<long long, Infoset,
		phmap::container_internal::hash_default_hash<long long>,
		phmap::container_internal::hash_default_eq<long long>,
		phmap::container_internal::Allocator<phmap::container_internal::Pair<long long, Infoset>>>* map = new phmap::node_hash_map<long long, Infoset,
		phmap::container_internal::hash_default_hash<long long>,
		phmap::container_internal::hash_default_eq<long long>,
		phmap::container_internal::Allocator<phmap::container_internal::Pair<long long, Infoset>>>();*/

	/// <summary>
	/// Insert nodes
	/// </summary>
 	long long totalIters = 0;
	ProgressBar progressBar = ProgressBar(Params::nofIters, 70);
	progressBar.display();

	std::for_each(
		std::execution::par_unseq,
		loop_indices.begin(),
		loop_indices.end(),
		[&](auto&& item)
		{
			long long t = -1;
			long long tsum = 0;

			for (t = item.first; t < item.second; ++t) 
			{
				long long key = Util::RandomLL(0, 1000000000LL);
				//Infoset& infoset = (*bp_hashmap)[key];
				//Infoset& infoset = umap[key];
				//libcuckooMap.insert(key, Infoset());

				Infoset infoset = Infoset();
				stdmap[key] = infoset;

				if ((t - item.first) % 1000000 == 0 && (t - item.first) != 0) 
				{
					progressBar.add(1000000);
					progressBar.display();
				}
			}
			progressBar.add((t - item.first) % 1000000);
			progressBar.display();
		});
	progressBar.done();
	std::cout << "Elements inserted: " << stdmap.size() << std::endl;


	/// <summary>
	/// Find nodes
	/// </summary>
	totalIters = 0;
	ProgressBar progressBar2 = ProgressBar(Params::nofIters, 70);
	progressBar2.display();

	long long sum = 0;
	loop_indices = std::vector<std::pair<uint64_t, uint64_t>>(Params::nofThreads); // (0,10000) (10000, 20000), 
	Util::GetThreadWorkItems(loop_indices, 0, Params::nofIters);

	std::for_each(
		std::execution::par_unseq,
		loop_indices.begin(),
		loop_indices.end(),
		[&](auto&& item)
		{
			long long t = -1;
			long long tsum = 0;

			for (t = item.first; t < item.second; ++t)
			{
				long long key = Util::RandomLL(0, 1000000000LL);
				//Infoset& infoset = (*map)[key];
				//Infoset& infoset = map[key];

				//Infoset infoset;
				//if (libcuckooMap.find(key, infoset)) {
				//	tsum++;
				//}

				std::unordered_map<long long, Infoset>::const_iterator got = stdmap.find(key);
				if (!(got == stdmap.end()))
				{
					tsum++;
				}

				if ((t - item.first) % 1000000 == 0 && (t - item.first) != 0)
				{
					progressBar2.add(1000000);
					progressBar2.display();
					sum += tsum;
					tsum = 0;
				}
			}
			sum += tsum;
			tsum = 0;
			progressBar2.add((t - item.first) % 1000000);
			progressBar2.display();
		});
	progressBar2.done();
	std::cout << "Found keys " << sum << "/" << Params::nofIters << std::endl;

	/// <summary>
	/// Delete nodes
	/// </summary>
}