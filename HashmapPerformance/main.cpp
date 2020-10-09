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
#include "parallel_hashmap/robin_hood.h"
#include "libcuckoo/cuckoohash_map.hh"
#include "parallel_hashmap/phmap.h"
#include "params.h"
#include "infoset.h"
#include "util.h"
#include "progress_bar.hpp"

void test_performance();
void create_index();

int main(int argc, char* argv[])
{
	//test_performance();
	create_index();
	//_CrtDumpMemoryLeaks();
}
void test_performance()
{
	// hashmaps
	std::unordered_map<long long, long long> stdmap; // 3mill, 80 mill
	//libcuckoo::cuckoohash_map<long long, Infoset> libcuckooMap;
	libcuckoo::cuckoohash_map<long long, long long> libcuckooMap; // 2.7mill, 37mill 
	//phmap::parallel_node_hash_map<long long, Infoset,
	//	phmap::container_internal::hash_default_hash<long long>,
	//	phmap::container_internal::hash_default_eq<long long>,
	//	phmap::container_internal::Allocator<phmap::container_internal::Pair<long long, Infoset>>,
	//	HASHMAP_N, std::mutex>* node_hash_map = new phmap::parallel_node_hash_map<long long, Infoset,
	//	phmap::container_internal::hash_default_hash<long long>,
	//	phmap::container_internal::hash_default_eq<long long>,
	//	phmap::container_internal::Allocator<phmap::container_internal::Pair<long long, Infoset>>,
	//	HASHMAP_N, std::mutex>();

	//phmap::parallel_flat_hash_map<long long, Infoset,
	//	phmap::container_internal::hash_default_hash<long long>,
	//	phmap::container_internal::hash_default_eq<long long>,
	//	phmap::container_internal::Allocator<phmap::container_internal::Pair<long long, Infoset>>,
	//	HASHMAP_N, std::mutex>* node_hash_map = new phmap::parallel_flat_hash_map<long long, Infoset,
	//	phmap::container_internal::hash_default_hash<long long>,
	//	phmap::container_internal::hash_default_eq<long long>,
	//	phmap::container_internal::Allocator<phmap::container_internal::Pair<long long, Infoset>>,
	//	HASHMAP_N, std::mutex>();

	//phmap::parallel_flat_hash_map<long long, long long,
	//	phmap::container_internal::hash_default_hash<long long>,
	//	phmap::container_internal::hash_default_eq<long long>,
	//	phmap::container_internal::Allocator<phmap::container_internal::Pair<long long, long long>>,
	//	HASHMAP_N, std::mutex>* node_hash_map = new phmap::parallel_flat_hash_map<long long, long long,
	//	phmap::container_internal::hash_default_hash<long long>,
	//	phmap::container_internal::hash_default_eq<long long>,
	//	phmap::container_internal::Allocator<phmap::container_internal::Pair<long long, long long>>,
	//	HASHMAP_N, std::mutex>(); // 3.3mill 24 mill

	//phmap::flat_hash_map<long long, long long,
	//	phmap::container_internal::hash_default_hash<long long>,
	//	phmap::container_internal::hash_default_eq<long long>,
	//	phmap::container_internal::Allocator<phmap::container_internal::Pair<long long, long long>>>* node_hash_map 
	//	= new phmap::flat_hash_map<long long, long long,
	//	phmap::container_internal::hash_default_hash<long long>,
	//	phmap::container_internal::hash_default_eq<long long>,
	//	phmap::container_internal::Allocator<phmap::container_internal::Pair<long long, long long>>>(); // 8.8mill, 131 mill

	robin_hood::unordered_flat_map<long long, long long>* node_hash_map
		= new robin_hood::unordered_flat_map<long long, long long>(); // 12.8 mill, 143mill


	/// <summary>
	/// Insert nodes
	/// </summary>
	Params::nofThreads = 1;

	// required for c++ parallel loops
	std::vector<std::pair<uint64_t, uint64_t>> loop_indices(Params::nofThreads); // (0,10000) (10000, 20000), 
	Util::GetThreadWorkItems(loop_indices, 0, Params::nofIters);

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
				//long long key = Util::RandomLL(0, 1000000000LL);
				//Infoset& infoset = (*node_hash_map)[key];
				long long testy = (*node_hash_map)[t% 100000000];

				//Infoset& infoset = umap[key];
				//libcuckooMap.insert(key, Infoset());
				//libcuckooMap.insert(t % 100000000, t % 100000000);

				//Infoset infoset = Infoset();
				//stdmap[t % 100000000] = t%100000000;

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
	Params::nofThreads = 24;
	totalIters = 0;
	ProgressBar progressBar2 = ProgressBar(2000000000, 70);
	progressBar2.display();

	long long sum = 0;
	loop_indices = std::vector<std::pair<uint64_t, uint64_t>>(Params::nofThreads); // (0,10000) (10000, 20000), 
	Util::GetThreadWorkItems(loop_indices, 0, 2000000000, Params::nofThreads);

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
				//long long key = Util::RandomLL(0, 1000000000LL);
				//Infoset& infoset = (*node_hash_map)[key];
				long long testy = (*node_hash_map)[t % 100000000];

				//Infoset& infoset = map[key];

				//Infoset infoset;
				//long long res;
				//if (libcuckooMap.find(t % 100000000)) {
				//	//tsum++;
				//}
				//long long res;
				//if (libcuckooMap.find(key, res)) {
				//	tsum++;
				//}
				//std::unordered_map<long long, long long>::const_iterator got = stdmap.find(t % 100000000);
				//if (!(got == stdmap.end()))
				//{
				//	//tsum++;
				//}

				if ((t - item.first) % 10000000 == 0 && (t - item.first) != 0)
				{
					progressBar2.add(10000000);
					progressBar2.display();
					sum += tsum;
					tsum = 0;
				}
			}
			sum += tsum;
			tsum = 0;
			progressBar2.add((t - item.first) % 10000000);
			progressBar2.display();
		});
	progressBar2.done();
	std::cout << "Found keys " << sum << "/" << Params::nofIters << std::endl;

	/// <summary>
	/// Delete nodes
	/// </summary>
}
void create_index()
{
	robin_hood::unordered_flat_map<long long, long long>* node_hash_map
		= new robin_hood::unordered_flat_map<long long, long long>();

	long long dataSize = 10000000;
	std::vector<long long> keys = std::vector<long long>(10000000);
	std::vector<float> data = std::vector<float>(100000000);
	long long idx = 0;

	Params::nofThreads = 1;
	std::vector<std::pair<uint64_t, uint64_t>> loop_indices(Params::nofThreads);
	Util::GetThreadWorkItems(loop_indices, 0, Params::nofIters);

	for (long long i = 0; i < dataSize; ++i)
	{
		// store index in hashmap
		long long key = Util::RandomLL(0, 1000000000LL);
		keys[i] = key;
		node_hash_map->emplace(key,idx);
		long long nofItems = Util::RandomLL(1, 9LL);
		
		data[idx] = nofItems; idx++;
		for (int j = 0; j < nofItems; ++j, ++idx) {
			data[idx] = j;
		}
	}

	Params::nofThreads = 48;
	long long totalIters = 0;
	ProgressBar progressBar2 = ProgressBar(2000000000, 70);
	progressBar2.display();

	long long sum = 0;
	loop_indices = std::vector<std::pair<uint64_t, uint64_t>>(Params::nofThreads);
	Util::GetThreadWorkItems(loop_indices, 0, 2000000000, Params::nofThreads);

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
				long long key = keys[t % 10000000];
				long long idx = node_hash_map->at(key);

				if (data[idx + 1] == 0)
					tsum++;

				if ((t - item.first) % 10000000 == 0 && (t - item.first) != 0)
				{
					progressBar2.add(10000000);
					sum += tsum;
					tsum = 0;
				}
			}
			sum += tsum;
			tsum = 0;
			progressBar2.add((t - item.first - 1) % 10000000);
		});
	progressBar2.done();
	std::cout << "Found keys " << sum << "/" << 2000000000 << std::endl;
}