set(wagner_src
  point.cc
  species.cc
  speciestree.cc
  tbranch.cc
  simulation.cc
)

# Compile the library
add_library(wagner STATIC ${wagner_src})

# Compile the executable
add_executable(wagner_exe main.cc)

target_link_libraries(wagner_exe wagner ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBS})

install(TARGETS wagner LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(TARGETS wagner_exe RUNTIME DESTINATION bin)
install(DIRECTORY ../include/wagner DESTINATION include)
