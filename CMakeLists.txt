project(snaro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Headers
  "include/snaro/point"
  "include/snaro/segment"
)

set(Sources
  "src/segment.cpp"
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