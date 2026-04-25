cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)
project(bismuth)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem)
find_package(tsl-ordered-map REQUIRED)

find_library(linenoise "linenoise" REQUIRED)
find_library(tcmalloc_minimal "tcmalloc_minimal" REQUIRED)

add_executable(bismuth)

target_sources(bismuth PRIVATE main.cpp)
target_link_libraries(bismuth PRIVATE tsl::ordered_map linenoise Boost::filesystem tcmalloc_minimal)

add_compile_options(-Wall -O3 -fomit-frame-pointer -fnew-alignment)

install(TARGETS bismuth)