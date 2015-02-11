file(GLOB SOURCE_FILES *.cc)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

add_executable(wagner2 ${SOURCE_FILES})

