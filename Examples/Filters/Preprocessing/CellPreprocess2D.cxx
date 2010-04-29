/*=========================================================================
  Author: $Author$  // Author of last commit
  Version: $Revision$  // Revision of last commit
  Date: $Date$  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009

 Copyright (c) 2009, President and Fellows of Harvard College.
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

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCellPreprocess.h"

int main ( int argc, char* argv[] )
  {
  if ( argc < 2 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " InputImage OutputImage" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;
  typedef itk::Image< unsigned char, Dimension > InputImage;
  typedef itk::ImageFileReader< InputImage > ReaderType;
  typedef itk::ImageFileWriter< InputImage > WriterType;

  typedef itk::CellPreprocess< InputImage, InputImage >
  FilterType;

  InputImage::Pointer input;

    {
    ReaderType::Pointer cellReader = ReaderType::New();
    cellReader->SetFileName ( argv[1] );
    cellReader->Update();
    input = cellReader->GetOutput();
    input->DisconnectPipeline();
    }

  InputImage::SpacingType spacing = input->GetSpacing();

//   std::cout << "Spacing: " << spacing << std::endl;
//   spacing[0] = spacing[1] = 0.2; spacing[2] = 1.0;
//   input->SetSpacing( spacing );
//   membraneImg->SetSpacing( spacing );

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput ( input );
  filter->SetLargestCellRadius ( 4.0 ); // in real coordinates
  filter->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput ( filter->GetOutput() );
  writer->SetFileName ( argv[2] );

  try
    {
    writer->Update();
    }
  catch ( itk::ExceptionObject e )
    {
    std::cerr << "Error: " << e << std::endl;
    }

  return EXIT_SUCCESS;
  }
