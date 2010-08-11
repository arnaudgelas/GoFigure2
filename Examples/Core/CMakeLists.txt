SET( QGoCoreExampleList
  QGoImageTest
  QGoImageReaderTest
  QGoXMLImportTest
  )

FOREACH( var ${QGoCoreExampleList} )
  ADD_EXECUTABLE( ${var} ${var}.cxx )
  TARGET_LINK_LIBRARIES( ${var} QGoCore )

ENDFOREACH( var ${QGoCoreExampleList} )

ADD_TEST( QGoImageTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoImageTest
)

ADD_TEST( QGoImageReaderTest1
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QGoImageReaderTest
  ${TESTING_DATA_PATH}/earpax2isl11.lsm
  1
)
