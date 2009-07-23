#include <iostream>

#include "itkImageFileReader.h"
#include "itkChanAndVeseSegmentationFilter.h"
#include "vtkImageData.h"
#include "vtkMarchingCubes.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkImageActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"

int main( int argc, char** argv )
{
  const unsigned int Dimension = 3;
  typedef itk::Image< float, Dimension > LevelSetImageType;
  typedef itk::Image< unsigned char, Dimension > FeatureImageType;

  typedef itk::ChanAndVeseSegmentationFilter< LevelSetImageType, FeatureImageType >
    SegmentationFilterType;

  typedef itk::ImageFileReader< FeatureImageType > ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  LevelSetImageType::PointType pt;

  for( unsigned int dim = 0; dim < Dimension; dim++ )
  {
    pt[dim] = atof( argv[dim+2] );
  }

  double cellRadius = atof( argv[Dimension+3] );

  SegmentationFilterType::Pointer filter = SegmentationFilterType::New();
  filter->SetFeatureImage( reader->GetOutput() );
  filter->SetRadius( cellRadius );
  filter->SetCenter( pt );
  filter->Update();

  vtkImageData* image = filter->GetOutput();

  // create iso-contours
  vtkMarchingCubes *contours = vtkMarchingCubes::New();
  contours->SetInput( image );
  contours->GenerateValues ( 1, 0, 0 );

  // map to graphics library
  vtkPolyDataMapper *map = vtkPolyDataMapper::New();
  map->SetInput( contours->GetOutput() );

  // actor coordinates geometry, properties, transformation
  vtkActor *contActor = vtkActor::New();
  contActor->SetMapper( map );

  vtkRenderer *ren = vtkRenderer::New();
  ren->AddActor ( contActor );

  vtkRenderWindow *renWin1 = vtkRenderWindow::New();
  renWin1->AddRenderer ( ren );

  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow ( renWin1 );

  bool test = atoi( argv[Dimension+4] );
  renWin1->Render();

  if( !test )
    {
    iren->Start();
    }

  iren->Delete();
  renWin1->Delete();
  ren->Delete();
  contActor->Delete();
  map->Delete();
  contours->Delete();
  image->Delete();

  return EXIT_SUCCESS;
}
