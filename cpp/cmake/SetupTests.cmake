# Set UtilsTest executable
add_executable(UtilsTest source/test/graphTest.cpp)
target_include_directories(UtilsTest PRIVATE source external/catch2)
target_link_libraries(UtilsTest PRIVATE MainTest)

# Set IsingTest executable
add_executable(IsingTest source/test/isingTest.cpp)
target_include_directories(IsingTest PRIVATE source source/Ising external/catch2)
target_link_libraries(IsingTest PRIVATE MainTest IsingGraph)

# Set XYTest executable
add_executable(XYTest source/test/xyTest.cpp)
target_include_directories(XYTest PRIVATE source source/xy external/catch2)
target_link_libraries(XYTest PRIVATE MainTest XYGraph)

enable_testing()
# Add test to ctest
#             Name              HowToRun
add_test(NAME UtilsTest COMMAND UtilsTest)
add_test(NAME IsingTest COMMAND IsingTest)
add_test(NAME XYTest    COMMAND XYTest)
