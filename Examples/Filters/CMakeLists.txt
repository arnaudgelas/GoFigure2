ADD_EXECUTABLE( itkvtkPolyDatatoBinaryMaskImageFilterTest
  itkvtkPolyDatatoBinaryMaskImageFilterTest.cxx )

TARGET_LINK_LIBRARIES( itkvtkPolyDatatoBinaryMaskImageFilterTest
  ITKIO
  ITKCommon
  vtkGraphics
  vtkHybrid
)

ADD_TEST( itkvtkPolyDatatoBinaryMaskImageFilterTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkvtkPolyDatatoBinaryMaskImageFilterTest
  ${TESTING_DATA_PATH}/Circle3D.mhd
  temp.mhd
)

ADD_EXECUTABLE( itkBinaryMaskImageToGoFigureMeshAttributesTest
 itkBinaryMaskImageToGoFigureMeshAttributesTest.cxx )
TARGET_LINK_LIBRARIES( itkBinaryMaskImageToGoFigureMeshAttributesTest
  QGoGui
)

ADD_TEST( itkBinaryMaskImageToGoFigureMeshAttributesTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkBinaryMaskImageToGoFigureMeshAttributesTest
  ${TESTING_DATA_PATH}/Circle3D.mhd
  temp.mhd
)

ADD_EXECUTABLE( itkvtkPolyDataToGoFigureMeshAttributesTest
 itkvtkPolyDataToGoFigureMeshAttributesTest.cxx )
TARGET_LINK_LIBRARIES( itkvtkPolyDataToGoFigureMeshAttributesTest
  QGoGui
  vtkGraphics
  vtkHybrid
)

ADD_TEST( itkvtkPolyDataToGoFigureMeshAttributesTest
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/itkvtkPolyDataToGoFigureMeshAttributesTest
  ${TESTING_DATA_PATH}/Circle3D.mhd
)

SET_TESTS_PROPERTIES( itkBinaryMaskImageToGoFigureMeshAttributesTest 
  PROPERTIES DEPENDS 
  itkvtkPolyDatatoBinaryMaskImageFilterTest
)

ADD_EXECUTABLE( ContourToMeshFilterTest
  ContourToMeshFilterTest.cxx
  )
TARGET_LINK_LIBRARIES( ContourToMeshFilterTest
  PoissonReconstruction
  vtkHybrid
  vtkRenderingAddOn2
  ITKCommon
  )

ADD_TEST( ContourToMeshFilterTest
 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ContourToMeshFilterTest
 1
 )

