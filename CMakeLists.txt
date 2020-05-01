cmake_minimum_required(VERSION 3.7 FATAL_ERROR)


project(smv)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(COMPLYER_DISABLE_READLINE "Do not use GNU Readline library if available" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wdeprecated -Wno-attributes" )

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.4.0)
    message(FATAL_ERROR "GCC>=7.4.0 required. In case you are on Ubuntu upgrade via ppa:ubuntu-toolchain-r/test")
  endif()
else()
  message(FATAL_ERROR "GNU Compiler required.")
endif()


add_definitions(-DANTLR4CPP_SHARED)

set(ANTLR4_WITH_STATIC_CRT OFF)
set(ANTLR_EXECUTABLE ${PROJECT_SOURCE_DIR}/thirdparty/antlr/antlr-4.7.2-complete.jar)

include(ExternalAntlr4Cpp)
include(FindReadline)

include_directories(${ANTLR4_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src/)

find_package(ANTLR REQUIRED)


antlr_target(GrammarLexer ${PROJECT_SOURCE_DIR}/src/nusmv/NuSMVLexer.g4 LEXER)
antlr_target(GrammarParser ${PROJECT_SOURCE_DIR}/src/nusmv/NuSMVParser.g4
             PARSER
             LISTENER
             DEPENDS_ANTLR GrammarLexer
             COMPILE_FLAGS -lib ${ANTLR_GrammarLexer_OUTPUT_DIR})

include_directories(${ANTLR_GrammarLexer_OUTPUT_DIR})
include_directories(${ANTLR_GrammarParser_OUTPUT_DIR})

file(GLOB SOURCE ${PROJECT_SOURCE_DIR}/src/*.cpp
                ${PROJECT_SOURCE_DIR}/src/*.h
        ${PROJECT_SOURCE_DIR}/src/listener/*.h
        ${PROJECT_SOURCE_DIR}/src/listener/*.cpp
        ${PROJECT_SOURCE_DIR}/src/interpreter/*.h
        )

add_library(command_base SHARED ${PROJECT_SOURCE_DIR}/src/commands/CommandBase.h
        ${PROJECT_SOURCE_DIR}/src/commands/CommandBase.cpp
        ${PROJECT_SOURCE_DIR}/src/commands/CommandResponse.h
        ${PROJECT_SOURCE_DIR}/src/commands/ModelSpecification.h)

target_link_libraries(command_base ${PROJECT_SOURCE_DIR}/libs/libz3.so)
target_link_libraries(command_base ${PROJECT_SOURCE_DIR}/libs/libverification-algorithms.so)
target_link_libraries(command_base ${PROJECT_SOURCE_DIR}/libs/libformula-tree.so)

add_executable(complyer ${PROJECT_SOURCE_DIR}/src/driver.cpp
                    ${ANTLR_GrammarLexer_CXX_OUTPUTS}
                    ${ANTLR_GrammarParser_CXX_OUTPUTS}
                    ${SOURCE}
                  )

target_link_libraries(complyer antlr4_shared)
target_link_libraries(complyer command_base)

if(NOT COMPLYER_DISABLE_READLINE) 
  find_package(Readline)
  if(READLINE_FOUND) 
    add_definitions(-DCOMPLYER_USE_READLINE)
    include_directories(${READLINE_INCLUDE_DIR})
    target_link_libraries(complyer ${READLINE_LIBRARY})
    target_link_libraries(complyer -ltinfo)
    if(WITH_TEST)
      add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
    endif(WITH_TEST)
  endif()
endif()

