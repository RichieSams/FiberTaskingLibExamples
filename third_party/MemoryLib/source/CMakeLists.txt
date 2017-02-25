include(SetSourceGroup)

SetSourceGroup(NAME Allocators
	PREFIX MEMORY_LIB
	SOURCE_FILES ../include/memory_lib/tagged_heap.h
	             allocators/tagged_heap.cpp
	             ../include/memory_lib/tagged_heap_backed_linear_allocator.h
	             allocators/tagged_heap_backed_linear_allocator.cpp
)

SetSourceGroup(NAME Util
	PREFIX MEMORY_LIB
	SOURCE_FILES ../include/memory_lib/config.h
	             ../include/memory_lib/memory.h
	             ../include/memory_lib/typedefs.h
)

set(MEMORY_LIB_SRC 
	${MEMORY_LIB_ALLOCATORS}
	${MEMORY_LIB_UTIL}
)

add_library(MemoryLib ${MEMORY_LIB_SRC})
target_include_directories(MemoryLib PUBLIC ../include)
