cmake_minimum_required(VERSION 3.2)
project(stb)


add_library(stb INTERFACE)

# Add include directory
target_include_directories(stb INTERFACE include)
