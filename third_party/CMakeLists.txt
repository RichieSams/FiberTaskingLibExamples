# Add necessary dependencies
find_package(OpenGL REQUIRED)

# Add third party libs
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_TESTS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
add_subdirectory(glfw)

set( FTL_BUILD_TESTS OFF CACHE BOOL  "FTL lib only" )
add_subdirectory(fiber_tasking_lib)

add_subdirectory(glad)

add_subdirectory(stb)

add_subdirectory(glm)
