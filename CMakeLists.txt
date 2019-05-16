cmake_minimum_required (VERSION 3.9)

project (DEAD_CODE_ELIMINATION)

find_package(LLVM REQUIRED CONFIG)

execute_process (COMMAND llvm-config --cxxflags --ldflags OUTPUT_VARIABLE LLVM_CONFIG_N)

include_directories(${LLVM_INCLUDE_DIRS})

string (REPLACE "\n" " " LLVM_CONFIG "${LLVM_CONFIG_N}")

set (CMAKE_CXX_FLAGS "-O3 -g ${LLVM_CONFIG} -undefined dynamic_lookup")

include_directories (include)
add_library (DeadCodeAnalysis SHARED
lib/DeadCodeAnalysis/DeadCodeAnalysis.cpp
lib/DeadCodeAnalysis/RangeAnalysis.cpp
lib/DeadCodeAnalysis/vSSA.cpp
lib/DeadCodeAnalysis/ClientRA.cpp
)
