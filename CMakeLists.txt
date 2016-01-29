cmake_minimum_required(VERSION 2.8)
project(FiberTaskingLibExamples)


# Add FiberTaskingLib dependency
set( FTL_BUILD_TESTS OFF CACHE BOOL  "Only build FiberTaskingLib core" )
add_subdirectory(third_party/fiber_tasking_lib)
include_directories(third_party/fiber_tasking_lib/include)
include_directories(third_party/fiber_tasking_lib/third_party/)

# Add Remotery
include_directories(third_party/remotery/lib)
add_library(remotery third_party/remotery/lib/remotery.c)


add_executable(ParallelSum source/parallel_sum.cpp)
target_link_libraries(ParallelSum FiberTaskingLib remotery)
