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
	source/xy/xyGraphUtils.cpp
	source/xy/Site.cpp
)
target_include_directories(XYGraphUtils PRIVATE source)

add_library(XY
	source/xy/xyHelpers.cpp
	source/xy/xyHelpers.h
	source/xy/xyWorm.cpp
	source/xy/xyWorm.h
)
target_include_directories(XY PRIVATE source source/xy)

# XYWorm will link to this
add_library(XYGraph INTERFACE)
target_link_libraries(XYGraph INTERFACE 
	Graph
	XYGraphUtils
	XY
)

# Set testing of source files
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE external/catch2)

add_library(MainTest source/test/mainTest.cpp)
target_include_directories(MainTest PRIVATE external/catch2)
target_link_libraries(MainTest PRIVATE Catch)
