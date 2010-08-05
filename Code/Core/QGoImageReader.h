/*=========================================================================
  Author: $Author:$  // Author of last commit
  Version: $Rev:$  // Revision of last commit
  Date: $Date:$  // Date of last commit
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

#ifndef __QGoImageReader_h
#define __QGoImageReader_h

#include <QThread>

#include <vector>
#include <string>

#include "itkImageIOBase.h"
#include "itkImage.h"
#include "itkImageFileReader.h"

//#include "itkMegaCaptureReader.h"

#include "QGoImage.h"
#include "QGoImageToITK.h"

class vtkLSMReader;
class QString;

/**
\class QGoImageReader
\brief Image Reader for GoFigure2
*/
class QGoImageReader : public QThread
{
public:
  typedef QGoImage::Pointer QGoImagePointer;

  explicit QGoImageReader( QObject* iParent = NULL );
  ~QGoImageReader();

  enum ReaderType
    {
//    MEGACAPTURE = 22,
    VTKLSM,
    ITKIO,
    UNSUPPORTED
    };

  void SetFileName( const char* iName );
  void SetFileName( const std::string& iName );
  void SetFileName( const QString& iName );

  void SetTimePoint( const unsigned int& iTimePoint );

  void Update();
  QGoImagePointer GetOutput();


protected:
  void run();

  std::string ExtractFileExtension();

  /** \brief Check if the input file is a MegaCapture. */
//  bool IsReadableByMegaCaptureReader();

  /** \brief Check if the input file can be read by one
  vtkLSMReader, i.e. if the extension is .lsm and check that
  there is no LZW Compression. */
  bool IsReadableByVTKLSMReader();
  void FillOutputFromLSMReaders();


  bool IsReadableByITKIO();

  template< unsigned int VDimension >
  void FillOutputFromITKReader( itk::ImageIOBase::IOComponentType iPixelType )
  {
    switch( iPixelType )
    {
    default:
    case itk::ImageIOBase::UNKNOWNCOMPONENTTYPE :
      {
      std::cerr <<"Unknown pixel type" <<std::endl;
      break;
      }
    case itk::ImageIOBase::UCHAR :
      {
      UpdateITKImage< unsigned char, VDimension >( );
      break;
      }
    case itk::ImageIOBase::CHAR :
      {
      UpdateITKImage< char, VDimension >( );
      break;
      }
    case itk::ImageIOBase::USHORT :
      {
      UpdateITKImage< unsigned short, VDimension >( );
      break;
      }
    case itk::ImageIOBase::SHORT :
      {
      UpdateITKImage< short, VDimension >( );
      break;
      }
    case itk::ImageIOBase::UINT :
      {
      UpdateITKImage< unsigned int, VDimension >( );
      break;
      }
    case itk::ImageIOBase::INT :
      {
      UpdateITKImage< int, VDimension >( );
      break;
      }
    case itk::ImageIOBase::ULONG :
      {
      UpdateITKImage< unsigned long, VDimension >( );
      break;
      }
    case itk::ImageIOBase::LONG :
      {
      UpdateITKImage< long, VDimension >( );
      break;
      }
    case itk::ImageIOBase::FLOAT :
      {
      UpdateITKImage< float, VDimension >( );
      break;
      }
    case itk::ImageIOBase::DOUBLE :
      {
      UpdateITKImage< double, VDimension >( );
      break;
      }
    }
  }

  template< typename TPixel, unsigned int VDimension >
  void UpdateITKImage()
  {
    typedef itk::Image< TPixel, VDimension > ImageType;
    typedef itk::ImageFileReader< ImageType > ReaderType;

    typename ReaderType::Pointer itk_reader = ReaderType::New();
    itk_reader->SetFileName( this->m_FileName.c_str() );
    itk_reader->Update();

    // convert itk_image to QGoImage
    QGoImageFromITKImage< ImageType >( this->m_Output, itk_reader->GetOutput() );
  }

  bool IsReadableByIOPlugin();

  void Init();

  std::vector< vtkLSMReader* >              m_LSMReaders;
//  itk::MegaCaptureReader::Pointer           m_MegaCaptureReader;
  itk::ImageIOBase::Pointer                 m_ITKReader;
  QGoImagePointer                           m_Output;
  ReaderType                                m_ReaderType;

  std::string m_FileName;

  unsigned int m_MinTimePoint;
  unsigned int m_MaxTimePoint;
  unsigned int m_UpdateTimePoint;

  unsigned int m_MinChannel;
  unsigned int m_MaxChannel;

  bool m_Initialized;
  bool m_Modified;

private:
  Q_DISABLE_COPY( QGoImageReader );
};
#endif

