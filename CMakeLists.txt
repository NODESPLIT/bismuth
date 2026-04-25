cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)
project(bismuth)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost REQUIRED COMPONENTS filesystem)
find_library(linenoise "linenoise" REQUIRED)
# find_library(tcmalloc "tcmalloc" REQUIRED)

add_executable(bismuth)

target_sources(bismuth PRIVATE main.cpp)
target_link_libraries(bismuth PRIVATE linenoise Boost::filesystem)

add_compile_options(-Wall -O3 -fomit-frame-pointer -fnew-alignment)

install(TARGETS bismuth)