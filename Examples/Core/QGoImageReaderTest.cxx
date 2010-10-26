#include <QApplication>
#include "QGoImageReader.h"
#include "QGoImage.h"

int main( int argc, char** argv )
{
  if (argc != 3)
    {
    std::cerr << "QGoImageReaderTest requires 2 arguments:" << std::endl;
    std::cerr << "1-filename" << std::endl;
    std::cerr << "2-test (boolean)" << std::endl;
    return EXIT_FAILURE;
    }

  QApplication app( argc, argv );
  QCoreApplication::setOrganizationName("MegasonLab");
  QCoreApplication::setOrganizationDomain("http://gofigure2.sourceforge.net");

  QGoImageReader* reader = new QGoImageReader;
  reader->SetFileName( argv[1] );
  reader->Update();

  QGoImage::Pointer qgo_image = reader->GetOutput();

  return EXIT_SUCCESS;
}
