#include <benchmark/benchmark.h>
#include "Graph.h"

static void BM_DivideGraph(benchmark::State& state) {
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);

	int length = int(state.range(0));
	const int dimension = 2;

	// Create a new graph
	int long seed = rand();
	Graph lattice(dimension, length, seed);
	std::unordered_map<int, std::vector<int>> blocks;

	for (auto _ : state) {
		benchmark::DoNotOptimize(blocks);
		lattice.DivideGraph(blocks);
		benchmark::ClobberMemory();
	}
}
// Register the function as a benchmark
// BENCHMARK(BM_DivideGraph)->RangeMultiplier(2)->Range(4, 4<<5);
BENCHMARK(BM_DivideGraph)->RangeMultiplier(2)->Range(4, 4<<2);

BENCHMARK_MAIN();
