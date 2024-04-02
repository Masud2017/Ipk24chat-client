include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SRC "${PROJECT_SOURCE_DIR}/tests/*.cc")

enable_testing()

file(GLOB SRC_DIR "${PROJECT_SOURCE_DIR}/src/*.cc")
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(
  chat_test
  ${TEST_SRC}
  ${SRC_DIR}
)

set(SPDLOG_BINARY_DIR ${CMAKE_BINARY_DIR}/spdlog_build)


target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/lib/argparse/include PUBLIC ${PROJECT_SOURCE_DIR}/lib/spdlog/include
)

target_link_directories(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCE_DIR}/lib/spdlog/src
)


target_link_libraries(
  chat_test
  GTest::gtest_main
  argparse
  spdlog
  ncurses
)


include(GoogleTest)
gtest_discover_tests(chat_test)