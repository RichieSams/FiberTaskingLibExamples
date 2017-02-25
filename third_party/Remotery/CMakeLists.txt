cmake_minimum_required(VERSION 3.2)
project(Remotery)

add_library(Remotery include/remotery/remotery.h
                      source/remotery.c)
target_include_directories(Remotery PUBLIC include)
