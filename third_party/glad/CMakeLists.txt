cmake_minimum_required(VERSION 3.2)
project(glad)


# Create the glad library
add_library(glad source/glad.c)

# Add include directory
target_include_directories(glad PUBLIC include)
