cmake_minimum_required(VERSION 3.7)

include(ExternalProject)

ExternalProject_Add(
        git_verificationalgorithms
        PREFIX ${CMAKE_BINARY_DIR}/verificationalgorithms
        URL https://github.com/AnandSaminathan/verification-algorithms/releases/download/v1.0/verificationalgo-source.zip
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        UPDATE_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
)
ExternalProject_Get_Property(git_verificationalgorithms source_dir)
set(VERIFICATIONALGORITHMS_INCLUDE ${source_dir}/include)
set(VERIFICATIONALGORITHMS_LIBRARY ${source_dir}/bin)

add_library(verification-algorithms STATIC ${PROJECT_SOURCE_DIR}/cmake/verificationalgorithms-main.cpp)

add_dependencies(verification-algorithms git_verificationalgorithms)
target_include_directories(verification-algorithms PUBLIC ${VERIFICATIONALGORITHMS_INCLUDE})
target_link_libraries(verification-algorithms ${VERIFICATIONALGORITHMS_LIBRARY}/libverification-algorithms.so)

target_link_libraries(verification-algorithms formula-tree)