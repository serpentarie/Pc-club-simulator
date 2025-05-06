cmake_minimum_required(VERSION 3.10)
project(pc-club-simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/Main.cpp
    src/club.cpp
    src/event.cpp
    src/table.cpp
    src/client.cpp
    src/utils.cpp
    src/parser.cpp
)

add_executable(task ${SOURCES})

target_include_directories(task PRIVATE src)

option(BUILD_TESTS "tests" OFF)
if(BUILD_TESTS)
    find_package(GTest REQUIRED)
    enable_testing()

    set(TEST_SOURCES
        tests/tests.cpp
        src/club.cpp
        src/event.cpp
        src/table.cpp
        src/client.cpp
        src/utils.cpp
        src/parser.cpp
    )

    add_executable(tests ${TEST_SOURCES})
    target_include_directories(tests PRIVATE src)
    target_link_libraries(tests GTest::GTest GTest::Main)
    add_test(NAME ClubTests COMMAND tests)
endif()