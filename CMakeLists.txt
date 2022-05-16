cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES
    ./src/*.cpp
    ./src/*.hpp
    ./src/*.h
)

project(crawler-cpp LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_MULTITHREADED ON)
add_definitions( -DBOOST_ALL_NO_LIB )
add_definitions( -DBOOST_ALL_DYN_LINK )

find_package(Boost REQUIRED COMPONENTS
    system
)

find_library(ASAN_PATH asan)

if(ASAN_PATH)
  message("asan found at " ${ASAN_PATH})
else()
  message("asan not found")
endif()

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    include_directories(${BOOST_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})

    add_definitions("-DHAS_BOOST")
    add_definitions("-DBOOST_THREAD_USE_LIB")

    include_directories(${BOOST_ROOT})
endif()

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES} ${ASAN_PATH})

# asan
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -fsanitize=address
    #-static-libasan
    -fsanitize=leak
    -fsanitize=undefined
    -fno-sanitize=vptr
    -fno-omit-frame-pointer
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -fsanitize=address
    #-static-libasan
    -fsanitize=leak
    -fsanitize=undefined
    -fno-sanitize=vptr
    -fno-omit-frame-pointer
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
