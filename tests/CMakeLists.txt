add_executable(test-io test-io.cpp)
target_link_libraries(test-io ${READLINE_LIBRARY})
target_link_libraries(test-io -ltinfo)
