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

#include "QGoImageReader.h"

#include "vtkLSMReader.h"
#include "QGoImage.h"

#include <QString>
#include <QFileInfo>
#include <QApplication>

#include "itkImageIOFactory.h"

//-------------------------------------------------------------------------
QGoImageReader::
QGoImageReader( QObject* iParent ) : 
  QThread( iParent ), 
  m_LSMReaders( 1, NULL ),
//  ITKReader( NULL ),
  m_ReaderType( UNSUPPORTED ),
  m_MinTimePoint( 0 ),
  m_MaxTimePoint( 0 ),
  m_UpdateTimePoint( 0 ),
  m_MinChannel( 0 ),
  m_MaxChannel( 0 ),
  m_Initialized( false ),
  m_Modified( false )
{
  m_LSMReaders[0] = vtkLSMReader::New();
//  m_MegaCaptureReader = itk::MegaCaptureReader::New();
  m_Output = QGoImage::New();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QGoImageReader::
~QGoImageReader()
{
  while( !m_LSMReaders.empty() )
    {
    m_LSMReaders.back()->Delete();
    m_LSMReaders.pop_back();
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void 
QGoImageReader::
SetFileName( const char* iName )
{
  m_FileName = iName;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void 
QGoImageReader::
SetFileName( const std::string& iName )
{
  m_FileName = iName;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void 
QGoImageReader::
SetFileName( const QString& iName )
{
  m_FileName = iName.toStdString();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QGoImageReader::QGoImagePointer
QGoImageReader::
GetOutput()
{
  return m_Output;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::string
QGoImageReader::
ExtractFileExtension()
{
  QString q_file = QString::fromStdString( m_FileName );
  QString suffix = QFileInfo( q_file ).completeSuffix();

  return suffix.toLower().toStdString();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void 
QGoImageReader::
SetTimePoint( const unsigned int& iTimePoint )
{
  if( iTimePoint != m_UpdateTimePoint )
    {
    m_Modified = true;
    m_UpdateTimePoint = iTimePoint;
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//bool 
//QGoImageReader::
//IsReadableByMegaCaptureReader()
//{
//  return false;
//}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool 
QGoImageReader::
IsReadableByVTKLSMReader()
{
  m_LSMReaders[0]->SetFileName( m_FileName.c_str() );
  
//   try
//     {
    m_LSMReaders[0]->Update();
//     }
  // here catch exception!
  //catch( )
    
  return( m_LSMReaders[0]->IsValidLSMFile() &&
        !m_LSMReaders[0]->IsCompressed() );
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool
QGoImageReader::
IsReadableByITKIO()
{
  m_ITKReader = itk::ImageIOFactory::CreateImageIO( m_FileName.c_str(), 
    itk::ImageIOFactory::ReadMode);
  if( m_ITKReader.IsNull() )
    {
    return false;
    }
  else
    {
    m_ITKReader->SetFileName( m_FileName );
//    m_ITKReader->ReadInformation();
    return true;
    }                                       
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoImageReader::
Update()
{
  this->start();
  while( this->isRunning() )
    {
    qApp->processEvents();
    this->wait( 50 );
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void 
QGoImageReader::
run()
{
  if( !m_Initialized )
    {
    Init();
    m_Initialized = true;
    }
  else
    {
    if( m_Modified )
      {
      // if megacapture
//       if( m_ReaderType == MEGACAPTURE )
//         {
//         m_MegaCaptureReader->SetTimePoint(m_UpdateTimePoint);
//         m_MegaCaptureReader->Update();
//           
//         for (unsigned int i = m_MinChannel; i <= m_MaxChannel; i++)
//           {
//           m_Output->SetNthChannelVTKImage( i, 
//             m_MegaCaptureReader->GetOutput(i) );
//           }
//         }
      // if vtkLSM
      if( m_ReaderType == VTKLSM )
        {
        vtkLSMReader* t_reader;
        for( unsigned int i = m_MinChannel; i <= m_MaxChannel; i++ )
          {
          t_reader = m_LSMReaders[i];
          t_reader->SetUpdateTimePoint(m_UpdateTimePoint);
          m_Output->SetNthChannelVTKImage( i, t_reader->GetOutput() );
          }
        }
      m_Modified = false;
      }
    }
  this->exec();
}
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
void 
QGoImageReader::
Init()
{
  bool readable = false;
  std::string extension = ExtractFileExtension();
  
  // -----------------------------------
  // Is it a megacapture file?
  if( extension.compare( ".meg" ) == 0 )
    {
//     if( IsReadableByMegaCaptureReader() )
//       {
//       // empty LSM Readers, release memory when possible!
//       m_LSMReaders[0]->Delete();
//       m_LSMReaders.pop_back();
//       
//       m_ReaderType = MEGACAPTURE;
//       readable = true;
//       }
    }
    
  // -----------------------------------
  // Is it a lsm file?
  if( extension.compare( ".lsm" ) == 0 )
    {
    if( IsReadableByVTKLSMReader() )
      {	
      m_MinChannel = 0;
      m_MaxChannel = m_LSMReaders[0]->GetNumberOfChannels();
    
      m_MinTimePoint = 0;
      m_MaxTimePoint = m_LSMReaders[0]->GetNumberOfTimePoints();
      
      for( unsigned int i = m_MinChannel; 
        i < m_MaxChannel; 
        i++ )
        {
        m_LSMReaders.push_back( vtkLSMReader::New() );
        m_LSMReaders.back()->SetFileName( m_FileName.c_str() );
        m_LSMReaders.back()->SetUpdateChannel( i );
        }
        
      m_ReaderType = VTKLSM;
      readable = true;
      }
    }
  
  // -----------------------------------
  // Is it a file that can be read by ITK?
  if( !readable )
    {  
    if( IsReadableByITKIO() )
      {
      // Read Image!
      readable = true;
      }
    }
   
  // -----------------------------------
  if( !readable )
    {
    // try with plugins if any
    }

  // -----------------------------------    
  if( !readable )
    {
    // throw an exception here!
    }
}
//-------------------------------------------------------------------------
