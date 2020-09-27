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

#include "parallel_hashmap/phmap.h"
#include "params.h"
#include "infoset.h"
#include "util.h"
#include "progress_bar.hpp"

using namespace std;

void create_nodes();

int main(int argc, char* argv[])
{
	/* train the bp_hashmap */
	create_nodes();
	//_CrtDumpMemoryLeaks();
}
void create_nodes()
{
	// required for c++ parallel loops
	std::vector<std::pair<uint64_t, uint64_t>> loop_indices(Params::nofThreads); // (0,10000) (10000, 20000), 
	Util::GetThreadWorkItems(loop_indices, 0, Params::nofIters);

	long long totalIters = 0;
	ProgressBar progressBar(Params::nofIters, 70);
	progressBar.display();

	long long sum = 0;
	std::unordered_map<long long, Infoset> umap;

	// main loop
	for_each(
		execution::par_unseq,
		loop_indices.begin(),
		loop_indices.end(),
		[&](auto&& item)
		{
			long long t = -1;
			long long tsum = 0;

			phmap::node_hash_map<long long, Infoset,
				phmap::container_internal::hash_default_hash<long long>,
				phmap::container_internal::hash_default_eq<long long>,
				phmap::container_internal::Allocator<phmap::container_internal::Pair<long long, Infoset>>>* bp_hashmap = new phmap::node_hash_map<long long, Infoset,
					phmap::container_internal::hash_default_hash<long long>,
					phmap::container_internal::hash_default_eq<long long>,
					phmap::container_internal::Allocator<phmap::container_internal::Pair<long long, Infoset>>>();

			for (t = item.first; t < item.second; ++t) 
			{
				long long key = Util::RandomLL(0, 100000000000LL);
				tsum += key;
				Infoset& infoset = (*bp_hashmap)[key];
				//Infoset& infoset = umap[key];

				if ((t - item.first) % 1000000 == 0 && (t - item.first) != 0) 
				{
					progressBar.add(1000000);
					progressBar.display();
					sum += tsum;
					tsum = 0;
				}
			}
			sum += tsum;
			tsum = 0;
			progressBar.add((t - item.first) % 1000000);
			progressBar.display();
		});
	progressBar.done();
	std::cout << sum << std::endl;
}