SET( CORE_SRC
  QGoImage.cxx
  QGoImageReader.cxx
  QGoXMLImport.cxx
  )

SET( CORE_HDRS
  QGoImage.h
  QGoImageReader.h
  QGoXMLImport.h
  )

QT4_WRAP_CPP( CORE_MOC ${CORE_HDRS} )

ADD_LIBRARY( QGoCore ${CORE_SRC} ${CORE_MOC} )
TARGET_LINK_LIBRARIES( QGoCore
  ${QT_LIBRARIES}
  QGoIO
  ITKBasicFilters
  ITKCommon
  ITKIO
  vtkLSMReader
  vtkImaging
)
