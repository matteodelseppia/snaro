set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

set(Sources
  "src/segment.cpp"
)

add_executable(${PROJECT_NAME}_test ${Sources})

target_link_libraries(
  ${PROJECT_NAME}_test
  PRIVATE
    snaro
    gtest_main
)

gtest_discover_tests(${PROJECT_NAME}_test)
