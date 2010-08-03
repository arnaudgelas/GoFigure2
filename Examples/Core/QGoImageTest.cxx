#include "vtkImageData.h"
#include "QGoImage.h"

int main( int argc, char** argv )
{
  unsigned int NbChannels = 5;
  
  QGoImage::Pointer qgo_image = QGoImage::New();
  qgo_image->SetNumberOfChannels( NbChannels );

  for( unsigned int ch = 0; ch < NbChannels; ++ch )
    {
    vtkImageData* image = vtkImageData::New();
    qgo_image->SetNthChannelVTKImage( ch, image );
    } 

  return EXIT_SUCCESS;
}
