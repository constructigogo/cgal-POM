# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.

cmake_minimum_required(VERSION 3.1...3.23)
project(linear_cell_complex_experimentations)

find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt5)
find_package( TBB REQUIRED )
include(CGAL_TBB_support)

# For Gprof.
# ADD_DEFINITIONS("-pg")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

# To use valgrind, we must disable rounding math ckeck.
# add_definitions(-DCGAL_DISABLE_ROUNDING_MATH_CHECK)

create_single_source_cgal_program("iterate_over_vertex.cpp")
create_single_source_cgal_program("sew_2_quadrangles.cpp")
create_single_source_cgal_program("sew_6_quadrangles.cpp")
create_single_source_cgal_program("multithread_crash.cpp")
create_single_source_cgal_program("concurrent_compact_container.cpp")

if(CGAL_Qt5_FOUND)
  target_link_libraries(iterate_over_vertex PUBLIC CGAL::CGAL_Basic_viewer)
  target_link_libraries(sew_2_quadrangles PUBLIC CGAL::CGAL_Basic_viewer)
  target_link_libraries(sew_6_quadrangles PUBLIC CGAL::CGAL_Basic_viewer)
endif()

target_link_libraries(concurrent_compact_container PUBLIC CGAL::TBB_support)
