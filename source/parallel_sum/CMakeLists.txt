include(SetSourceGroup)

SetSourceGroup(NAME Root
	PREFIX PARALLEL_SUM
	SOURCE_FILES parallel_sum.cpp
)

add_executable(ParallelSum ${PARALLEL_SUM_ROOT})
target_link_libraries(ParallelSum FiberTaskingLib)
