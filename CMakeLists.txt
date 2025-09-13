cmake_minimum_required(VERSION 3.5)
project(snaro)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Headers
  "include/snaro/point.hpp"
  "include/snaro/segment.hpp"
  "include/snaro/details/pixel.hpp"
  "include/snaro/details/event.hpp"
  "include/snaro/details/event_queue.hpp"
  "include/snaro/details/active_segment_queue.hpp"
)

set(Sources
  "src/segment.cpp"
  "src/event.cpp"
  "src/event_queue.cpp"
  "src/active_segment_queue.cpp"
)

add_library(snaro ${Headers} ${Sources})

target_include_directories(
  snaro PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if (SNARO_BUILD_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()