cmake_minimum_required(VERSION 3.10)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 11)

include_directories(header)

add_executable(searchengine src/searchengine.cpp
src/document_store.cpp
src/Map.cpp)