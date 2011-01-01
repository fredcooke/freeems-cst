project( freeems-cst CXX )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

# set Debug builds as default (if not already changed in the cache)
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF()

find_package( Boost COMPONENTS
              unit_test_framework
              system
              program_options
              )

link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS}
                     ./inc
                     )

add_definitions(
    -Wextra
    -Werror
    -Wall
    -Wunused
    -Wcast-qual
    -ansi
    -pedantic-errors
    )

if( NOT APPLE )
    add_definitions( -Wold-style-cast )
endif( NOT APPLE )

set( DIRS
     src
     test
     )

set( LIBS
     ${Boost_LIBRARIES}
     )

set( EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" )
set( LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" )

foreach( dir ${DIRS} )
    add_subdirectory( ${dir} )
endforeach( dir )
