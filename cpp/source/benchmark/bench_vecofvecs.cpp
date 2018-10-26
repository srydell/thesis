#include <benchmark/benchmark.h>
#include <vector>
#include <cmath>

static void BM_singlevec(benchmark::State& state) {
	const int length = int(state.range(0));
	constexpr int dimension = 2;

	for (auto _ : state) {
		std::vector<int> lattice;
		benchmark::DoNotOptimize(lattice);
		for (int i = 0; i < std::pow(length, dimension); ++i) {
			lattice.push_back(i);
		}
		for (auto& site : lattice) {
			site = 5;
		}
		benchmark::ClobberMemory();
	}
}
// Register the function as a benchmark
// BENCHMARK(BM_vecofvecs)->RangeMultiplier(2)->Range(4, 4<<5);
BENCHMARK(BM_singlevec)->RangeMultiplier(2)->Range(4, 4<<5);

static void BM_vecofvecs(benchmark::State& state) {
	const int length = int(state.range(0));
	// const int dimension = 2;

	for (auto _ : state) {
		std::vector<std::vector<int>> lattice;
		benchmark::DoNotOptimize(lattice);
		for (int i = 0; i < length; ++i) {
			std::vector<int> v;
			lattice.push_back(v);
			for (int j = 0; j < length; ++j) {
				v.push_back(i + j);
			}
		}
		for (auto& row : lattice) {
			for (auto& site : row) {
				site = 5;
			}
		}
		benchmark::ClobberMemory();
	}
}
// Register the function as a benchmark
// BENCHMARK(BM_vecofvecs)->RangeMultiplier(2)->Range(4, 4<<5);
BENCHMARK(BM_vecofvecs)->RangeMultiplier(2)->Range(4, 4<<5);

BENCHMARK_MAIN();
