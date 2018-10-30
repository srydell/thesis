#include <benchmark/benchmark.h>
#include <vector>
#include <cmath>

struct test_site{
	test_site(int i, long long n1, long long n2)
		: i(i), n1(n1), n2(n2) {};
	int i;
	long long n1;
	long long n2;
};

// static void BM_singlevec(benchmark::State& state) {
// 	const int length = int(state.range(0));
// 	constexpr int dimension = 2;

// 	for (auto _ : state) {
// 		std::vector<test_site> lattice;
// 		benchmark::DoNotOptimize(lattice);
// 		for (int i = 0; i < std::pow(length, dimension); ++i) {
// 			lattice.push_back({i, i, i});
// 		}
// 		for (auto& site : lattice) {
// 			site = {1, 2, 3};
// 		}
// 		benchmark::ClobberMemory();
// 	}
// }
// // Register the function as a benchmark
// // BENCHMARK(BM_vecofvecs)->RangeMultiplier(2)->Range(4, 4<<5);
// BENCHMARK(BM_singlevec)->RangeMultiplier(2)->Range(4, 4<<5);

static void BM_vecofvecs(benchmark::State& state) {
	const int length = int(state.range(0));
	// const int dimension = 2;

	for (auto _ : state) {
		std::vector<std::vector<test_site>> lattice;
		benchmark::DoNotOptimize(lattice);
		for (int i = 0; i < length; ++i) {
			std::vector<test_site> v;
			lattice.push_back(v);
			for (int j = 0; j < length; ++j) {
				v.push_back({i, i, i});
			}
		}
		for (auto& row : lattice) {
			for (auto& site : row) {
				site = {1, 2, 3};
			}
		}
		benchmark::ClobberMemory();
	}
}
// Register the function as a benchmark
// BENCHMARK(BM_vecofvecs)->RangeMultiplier(2)->Range(4, 4<<5);
BENCHMARK(BM_vecofvecs)->RangeMultiplier(2)->Range(4, 4<<5);

BENCHMARK_MAIN();
