
cmake_minimum_required(VERSION 3.16)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(monstrum2d VERSION ${PROJECT_VESRION})
set (CMAKE_CXX_STANDARD 17)

set(THIRD_PARTY_INCLUDE_DIR)
set(THIRD_PARTY_LIBS)

# Python
find_package(Python 3 COMPONENTS Interpreter Development)
set(THIRD_PARTY_INCLUDE_DIR ${THIRD_PARTY_INCLUDE_DIR} ${Python_INCLUDE_DIRS})
set(THIRD_PARTY_LIBS ${THIRD_PARTY_LIBS} ${Python_LIBRARIES})
set(PYTHON3 python${Python_VERSION_MAJOR}${Python_VERSION_MINOR})
message(PYTHON3 = ${PYTHON3})

#yaml-cpp
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml tests")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "disable yaml tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml contrib")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp)
set(THIRD_PARTY_INCLUDE_DIR ${THIRD_PARTY_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/yaml-cpp/include)
set (THIRD_PARTY_LIBS ${THIRD_PARTY_LIBS} yaml-cpp)

set (Boost_USE_STATIC_LIBS ON)
set (BOOST_ALL_DYN_LINK OFF)
find_package(
  Boost
  REQUIRED
  COMPONENTS
  program_options
  ${PYTHON3})

file (
  GLOB_RECURSE
  PROJECT_SOURCE_FILES
  ${PROJECT_SOURCE_DIR}/src/*.h
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

include_directories(${THIRD_PARTY_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

if(Boost_FOUND)
  set (
    THIRD_PARTY_LIBS
    ${THIRD_PARTY_LIBS}
    Boost::boost
    Boost::program_options
    Boost::${PYTHON3})
endif()

target_link_libraries(${PROJECT_NAME} ${THIRD_PARTY_LIBS})

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT unlinker@mail.ru)

include(CPack)