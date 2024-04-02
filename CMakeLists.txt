cmake_minimum_required(VERSION 3.10)

# set the project name
project(ipk24chat-client VERSION 1.0)

file(GLOB SRC_DIR "${PROJECT_SOURCE_DIR}/src/*.cc")
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB TEST_DIR "${PROJECT_SOURCE_DIR}/tests/*.cc")


# add_executable(${PROJECT_NAME} main.cc ${SRC_DIR} ${TEST_SRC_DIR} ${TEST_DIR})
add_executable(${PROJECT_NAME} main.cc ${SRC_DIR} ${TEST_SRC_DIR})



set(PROJECT_VERSION 1.0)
message(STATUS "Build chat_client network client: ${PROJECT_VERSION}")

# google test lib section 
# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)
# enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie")

# enabling gdb 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

# enabling gdb ended


# google tes lib section ended


# setting the src file of the project
add_subdirectory(lib/spdlog)
add_subdirectory(lib/argparse)
# add_subdirectory(lib/boost)
add_subdirectory(tests)




target_include_directories(${PROJECT_NAME}
	PUBLIC lib/argparse/include PUBLIC lib/spdlog/include
)

target_link_directories(${PROJECT_NAME}
	PRIVATE lib/spdlog/src
)



target_link_libraries(${PROJECT_NAME} 

spdlog  argparse ncurses)

