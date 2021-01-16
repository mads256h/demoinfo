cmake_minimum_required(VERSION 3.10)

# Project name
project(demoinfo VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(project_warnings INTERFACE)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

add_executable(demoinfo demoinfo.c)
