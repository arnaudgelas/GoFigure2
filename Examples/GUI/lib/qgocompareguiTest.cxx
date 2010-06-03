#include <QApplication>
#include <QTimer>

#include "vtkSmartPointer.h"
#include "vtkPNGReader.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageGradient.h"
#include "vtkMetaImageReader.h"
/*
#include "QGoComparer.h"
#include  "QGoCompareOrchestra.h"
#include "vtkMetaImageReader.h"
#include "QGoComparer3D.h"
*/
#include "qgocomparegui.h"

#include <QStringList>
#include <QString>


int main( int argc, char** argv )
{

  if( argc != 4 )
    {
    std::cout <<"Usage : qgocomparertest(.exe) " <<std::endl;
    std::cout << "1-file.png" <<std::endl;
    std::cout << "2-file.mhd" <<std::endl;
    std::cout << "3-test (boolean)" <<std::endl;
    return EXIT_FAILURE;
    }

  QApplication app( argc, argv );
  QCoreApplication::setOrganizationName("MegasonLab");
  QCoreApplication::setOrganizationDomain( "http://gofigure2.sourceforge.net" );


  // create 3 2D images from 1

  vtkSmartPointer< vtkPNGReader > reader = vtkSmartPointer< vtkPNGReader >::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  vtkSmartPointer< vtkImageGaussianSmooth > filter1 =
                            vtkSmartPointer< vtkImageGaussianSmooth >::New();
  filter1->SetInputConnection(reader->GetOutputPort());
  filter1->Update();

  vtkSmartPointer< vtkImageGradient > filter2 =
                            vtkSmartPointer< vtkImageGradient >::New();
  filter2->SetInputConnection(reader->GetOutputPort());
  filter2->Update();



  // create 3 3D images from 1

  vtkSmartPointer< vtkMetaImageReader > reader3D = vtkSmartPointer< vtkMetaImageReader >::New();
  reader3D->SetFileName( argv[2] );
  reader3D->Update();

  vtkSmartPointer< vtkImageGaussianSmooth > filter13D =
                            vtkSmartPointer< vtkImageGaussianSmooth >::New();
  filter13D->SetInputConnection(reader3D->GetOutputPort());
  filter13D->Update();

  vtkSmartPointer< vtkImageGradient > filter23D =
                            vtkSmartPointer< vtkImageGradient >::New();
  filter23D->SetInputConnection(reader3D->GetOutputPort());
  filter23D->Update();





  QString cp0 = "comp0";
  QString cp1 = "comp1";
  QString cp2 = "comp3";

  QString cp03D = "comp03D";
  QString cp13D = "comp13D";
  QString cp23D = "comp33D";


  QTimer* timer = new QTimer;
  timer->setSingleShot( true );

  /*
  QObject::connect( timer, SIGNAL( timeout() ), Comparer0, SLOT( close() ) );
  QObject::connect( timer, SIGNAL( timeout() ), Comparer1, SLOT( close() ) );
  QObject::connect( timer, SIGNAL( timeout() ), Comparer2, SLOT( close() ) );
*/



  QGoCompareGUI* comparegui = new QGoCompareGUI() ;

  comparegui->newComparer2D(cp0,reader->GetOutput());
  comparegui->newComparer2D(cp1,filter1->GetOutput());

  comparegui->newComparer3D(cp03D,reader3D->GetOutput());
  comparegui->newComparer3D(cp13D,filter13D->GetOutput());



  comparegui->Update();


  comparegui->show();



  return app.exec();


}