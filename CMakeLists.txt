cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)
project(bismuth)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem)
find_package(tsl-ordered-map REQUIRED)

find_library(linenoise "linenoise" REQUIRED)

set(OLD_CMAKE_FIND_LIBRARY_SUFFIXES "${CMAKE_FIND_LIBRARY_SUFFIXES}")
list(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ".so.4")

find_library(tcmalloc NAMES tcmalloc tcmalloc_minimal)

add_executable(bismuth)

target_sources(bismuth PRIVATE main.cpp)
target_link_libraries(bismuth PRIVATE tsl::ordered_map linenoise Boost::filesystem)

add_compile_options(-Wall -O3 -fomit-frame-pointer -fnew-alignment)

set(CMAKE_FIND_LIBRARY_SUFFIXES "${OLD_CMAKE_FIND_LIBRARY_SUFFIXES}")
unset(OLD_CMAKE_FIND_LIBRARY_SUFFIXES)

install(TARGETS bismuth)