set( TESTS
     cst-test0
     )

foreach( test ${TESTS} )
  add_executable( ${test} ${test}.cpp )
  target_link_libraries( ${test} ${LIBS} )
endforeach( test )
