add_library(Graph
	source/Graph.cpp
	source/Graph.h
	source/IndexCluster.cpp
	source/HKIndex.cpp
	source/Site.h
	source/Percolation.cpp
	source/ClusterDimension.cpp
	source/utils.h
)
target_include_directories(Graph PRIVATE source)

add_library(IsingGraphUtils
	source/ising/IsingGraphUtils.cpp
	source/ising/Site.cpp
)
target_include_directories(IsingGraphUtils PRIVATE source)

add_library(Ising
	source/ising/IsingWorm.cpp
	source/ising/IsingWorm.h
)
target_include_directories(Ising PRIVATE source source/ising)

# IsingWorm will link to this
add_library(IsingGraph INTERFACE)
target_link_libraries(IsingGraph INTERFACE 
	Graph
	IsingGraphUtils
	Ising
)

add_library(XYGraphUtils
	source/xy/IsingGraphUtils.cpp
	source/xy/Site.cpp
)
target_include_directories(IsingGraphUtils PRIVATE source)

# Set testing of source files
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE external/catch2)
