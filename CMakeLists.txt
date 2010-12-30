project( cst CXX )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

# set Debug builds as default (if not already changed in the cache)
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
ENDIF()

find_package( Boost )
link_directories( ${Boost_LIBRARY_DIRS} )
include_directories( ${Boost_INCLUDE_DIRS} )

add_definitions(
    -Wextra
    -Werror
    -Wall
    -Wunused
    -Wcast-qual
    -Wold-style-cast
    -ansi
    -pedantic-errors
    )

add_subdirectory( src )
