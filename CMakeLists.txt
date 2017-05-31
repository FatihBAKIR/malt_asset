cmake_minimum_required(VERSION 3.2)
project(malt)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../malt_cmake/")
set(CMAKE_CXX_STANDARD 14)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
endif()

include(Malt)

set(HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/malt_asset")
set(HEADER_FILES
        ${HEADER_PATH}/module.hpp
        ${HEADER_PATH}/assets.hpp
        ${HEADER_PATH}/asset_mgr.hpp
        ${HEADER_PATH}/text_asset.hpp
        ${HEADER_PATH}/asset_loader.hpp
        )

set(SOURCE_FILES
        src/module.cpp
        src/assets.cpp
        src/text_asset.cpp)

add_library(malt_asset SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(malt_asset PUBLIC "C:\\MinGW\\include")

malt_def_module(malt_asset)

malt_add_module(malt_asset malt_core)

malt_install(malt_asset ${HEADER_PATH})
