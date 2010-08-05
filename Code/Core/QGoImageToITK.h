/*=========================================================================
  Author: $Author$  // Author of last commit
  Version: $Rev$  // Revision of last commit
  Date: $Date$  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009-10

 Copyright (c) 2009-10, President and Fellows of Harvard College.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 Neither the name of the  President and Fellows of Harvard College
 nor the names of its contributors may be used to endorse or promote
 products derived from this software without specific prior written
 permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#ifndef __QGoImageToITK_h
#define __QGoImageToITK_h

#include "QGoImage.h"
#include "itkImage.h"
#include "itkImageToVTKImageFilter.h"

template< class TImage >
QGoImage::Pointer
QGoImageFromITKImage( typename TImage::Pointer iImage )
{
  QGoImage::Pointer oImage = QGoImage::New();

  typedef TImage ImageType;
  typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
  typename ConnectorType::Pointer connector = ConnectorType::New();
  connector->SetInput( iImage );

  try
    {
    connector->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "Error while setting QGoImage" << " " << err << std::endl;
    return oImage;
    }
  vtkImageData *image = vtkImageData::New();
  image->ShallowCopy(connector->GetOutput());
  oImage->SetNumberOfChannels( 1 );
  oImage->SetNthChannelVTKImage( 0, image );

  return oImage;
}

template< class TImage >
void
QGoImageFromITKImage( QGoImage::Pointer ioQGoImage,
                      typename TImage::Pointer iImage )
{
  typedef TImage ImageType;
  typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
  typename ConnectorType::Pointer connector = ConnectorType::New();
  connector->SetInput( iImage );

  try
    {
    connector->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "Error while setting QGoImage" << " " << err << std::endl;
    return;
    }
  vtkImageData *image = vtkImageData::New();
  image->ShallowCopy(connector->GetOutput());
  ioQGoImage->SetNumberOfChannels( 1 );
  ioQGoImage->SetNthChannelVTKImage( 0, image );
}

#endif // __QGoImageToITK_h
