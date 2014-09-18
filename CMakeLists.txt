cmake_minimum_required(VERSION 2.8)
project(mc-pricer CXX)

# Prefix
if (NOT PREFIX)
    set(PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif (NOT PREFIX)
set(CMAKE_INSTALL_PREFIX ${PREFIX} CACHE STRING "Installation prefix." FORCE)

if (CXX)
    set(CMAKE_CXX_COMPILER ${CXX})
endif (CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Release or Debug
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif ()


# Detect Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Detect PNL
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})

# Add math lib
set(LIBS ${LIBS} m)

# Testing the parser
add_executable(parser-test src/parser.cpp src/test_parser.cpp)
target_link_libraries(parser-test ${LIBS})
install(TARGETS parser-test DESTINATION bin)

add_executable(option-test src/optionAsian.cpp src/option.cpp src/test_option.cpp)
target_link_libraries(option-test ${LIBS})
install(TARGETS option-test DESTINATION bin)

# Add google test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link test executable against gtest & gtest_main
add_executable(runUnitTests src/allTests.cpp src/optionBarrierLow.cpp src/option.cpp test/option-test.cpp)
target_link_libraries(runUnitTests gtest gtest_main)
add_test(RunUnitTests runUnitTests)

add_test(
    NAME runUnitTests
    COMMAND runUnitTests
)


# add_executable(mc-pricer src/parser.cpp src/pricer.cpp)
# target_link_libraries(mc-pricer ${LIBS})
# install(TARGETS mc-pricer DESTINATION bin)


