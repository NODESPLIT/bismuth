cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)
project(example)

find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem)
find_package(tsl-ordered-map REQUIRED)

set(OLD_CMAKE_FIND_LIBRARY_SUFFIXES "${CMAKE_FIND_LIBRARY_SUFFIXES}")
list(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ".so.4")

find_library(tcmalloc NAMES tcmalloc tcmalloc_minimal)

add_library(example SHARED main.cpp)
add_compile_options(-fomit-frame-pointer -fnew-alignment 8 -O3 -shared -undefined dynamic_lookup)

target_sources(example PRIVATE main.cpp)
target_link_libraries(example PRIVATE tsl::ordered_map Boost::filesystem)

set_target_properties(
	example
	PROPERTIES
		PREFIX ""
		SUFFIX .crystal
)