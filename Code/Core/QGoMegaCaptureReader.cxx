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


#include "QGoMegaCaptureReader.h"

#include <QXmlStreamReader>
#include <QMessageBox>
#include <QString>
#include <QFile>

// -----------------------------------------------------------------------------
QGoMegaCaptureReader::
QGoMegaCaptureReader()
{
  this->xmlStream = new QXmlStreamReader;
  this->file = new QFile( this );

  m_PCoord = 0;
  m_RCoord = 0;
  m_CCoord = 0;
  m_ZTileCoord = 0;
  m_YTileCoord = 0;
  m_XTileCoord = 0;
  m_TCoord = 0;
  m_ZCoord = 0;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
QGoMegaCaptureReader::
~QGoMegaCaptureReader()
{
  delete this->xmlStream;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void
QGoMegaCaptureReader::
Read( const QString& iFilename )
{
  if( ( iFilename.isEmpty() ) || ( iFilename.isNull() ) )
    {
    QMessageBox::critical(NULL,
                          "QGoMegaCaptureReader",
                          "empty filename",
                          QMessageBox::Ok);
    return;
    }

  this->file->setFileName( iFilename );

  if (!this->file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
    QMessageBox::critical(NULL,
                          "QGoMegaCaptureReader",
                          "Couldn't open this file",
                          QMessageBox::Ok);
    return;
    }

  this->xmlStream->setDevice( this->file );

  while( !this->xmlStream->atEnd() && !this->xmlStream->hasError() )
    {
    /* Read next element.*/
    QXmlStreamReader::TokenType token = this->xmlStream->readNext();
    /* If token is just StartDocument, we'll go to next.*/
    if(token == QXmlStreamReader::StartDocument)
      {
      continue;
      }

    /* If token is StartElement, we'll see if we can read it.*/
    if(token == QXmlStreamReader::StartElement)
      {
      if( ( this->xmlStream->name() == "MegaCapture" )  )
        {
        if( this->xmlStream->attributes().value("version") == "4.0" )
          {
          continue;
          }
        else
          {
          QMessageBox::critical( NULL,
                                 "QGoMegaCaptureReader",
                                 "Error wrong version number",
                                 QMessageBox::Ok );
          return;
          }
        }
      if( this->xmlStream->name() == "imagingsession" )
        {
        ReadImagingSession();
        continue;
        }
      if( this->xmlStream->name() == "ImageData" )
        {
        ReadImageData();
        continue;
        }
      }
    }
  if(this->xmlStream->hasError())
    {
    QMessageBox::critical(NULL,
                          "QGoMegaCaptureReader",
                          this->xmlStream->errorString(),
                          QMessageBox::Ok);
    }
  this->file->close();
  this->xmlStream->clear();
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadImagingSession()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
           ( this->xmlStream->name() != "imagingsession" ) ) )
    {
    token = this->xmlStream->readNext();

    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "ProjectName" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "MicroscopeName" )
        {
        // get the name from the imaging session
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "Name" )
        {
        // get the name from the imaging session
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "Description" )
        {
        // get the name from the imaging session
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "CreationDate" )
        {
        // get the name from the imaging session
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "ImagesInformation" )
        {
        ReadImagesInformation();
        continue;
        }
      if( this->xmlStream->name() == "ChannelList" )
        {
        ReadChannelList();
        continue;
        }
      if( this->xmlStream->name() == "MicroscopeSettings" )
        {
        ReadMicroscopeSettings();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadImagesInformation()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
           ( this->xmlStream->name() != "ImagesInformation" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "FileType" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "XImageSize" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "YImageSize" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "XTileOverlap" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "XTileOverlap" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "YTileOverlap" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "Range" )
        {
        ReadRange();
        continue;
        }
      if( this->xmlStream->name() == "Sampling" )
        {
        ReadSampling();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadRange()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
           ( this->xmlStream->name() != "Range" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "CoordMin" )
        {
        ReadCoordinate();
        continue;
        }
      if( this->xmlStream->name() == "CoordMax" )
        {
        ReadCoordinate();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadCoordinate()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
           ( this->xmlStream->name() != "coordinate" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "PCoord" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "CCoord" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "RCoord" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "ZTileCoord" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "YTileCoord" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "XTileCoord" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "TCoord" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      if( this->xmlStream->name() == "ZCoord" )
        {
        temp = this->xmlStream->readElementText();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadSampling()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
           ( this->xmlStream->name() != "Sampling" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "Space" )
        {
        temp = this->xmlStream->attributes().value("unit").toString();

        double factor = 1.;
        if( !temp.isEmpty() && !temp.isNull() )
          {
          if( ( temp == "um" ) || ( temp == "micrometer" ) )
            {
            factor = 1.;
            }
          else
            {
            if( ( temp == "nm" ) || ( temp == "nanometer" ) )
              {
              factor = 0.001;
              }
            else
              {
              if( ( temp == "mm" ) || ( temp == "millimeter" ) )
                {
                factor = 1000;
                }
              else
                {
                if( ( temp == "m" ) || ( temp == "meter" ) )
                  {
                  factor = 1000000;
                  }
                }
              }
            }
          }
        ReadSpatialSampling( factor );
        continue;
        }
      if( this->xmlStream->name() == "Time" )
        {
        temp = this->xmlStream->attributes().value("unit").toString();

        double factor = 1.;

        if( !temp.isEmpty() && !temp.isNull() )
          {
          if( ( temp == "s" ) || ( temp == "sec" ) ||
              ( temp == "seconds" ) || ( temp == "second" ) )
            {
            factor = 1.;
            }
          else
            {
            if( ( temp == "ms" ) ||
                ( temp == "milliseconds" ) || ( temp == "millisecond" ) )
              {
              factor = 0.001;
              }
            else
              {
              if( ( temp == "min" ) ||
                  ( temp == "minutes" ) || ( temp =="minute" ) )
                {
                factor = 60.;
                }
              else
                {
                if( ( temp == "h" ) || ( temp == "hr" ) ||
                    ( temp == "hours" ) || ( temp == "hour" ) )
                  {
                  factor = 3600.;
                  }
                else
                  {
                  if( ( temp == "days" ) || ( temp == "day" ) )
                    {
                    factor = 86400.;
                    }
                  }
                }
              }
            }
          }
        ReadTemporalSampling( factor );
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadSpatialSampling( double iFactor )
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
           ( this->xmlStream->name() != "Space" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "RealPixelWidth" )
        {
        continue;
        }
      if( this->xmlStream->name() == "RealPixelHeight" )
        {
        continue;
        }
      if( this->xmlStream->name() == "RealPixelDepth" )
        {
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadTemporalSampling( double iFactor )
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
           ( this->xmlStream->name() != "Time" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "ImagesTimeInterval" )
        {
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadChannelList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
           ( this->xmlStream->name() != "ChannelList" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "channel" )
        {
        ReadChannel();
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadChannel()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
           ( this->xmlStream->name() != "channel" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "ChannelNumber" )
        {
        continue;
        }
      if( this->xmlStream->name() == "Name" )
        {
        continue;
        }
      if( this->xmlStream->name() == "NumberOfBits" )
        {
        continue;
        }
      if( this->xmlStream->name() == "color" )
        {
        ReadColor();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadColor()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
           ( this->xmlStream->name() != "color" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "Name" )
        {
        continue;
        }
      if( this->xmlStream->name() == "Description" )
        {
        continue;
        }
      if( this->xmlStream->name() == "Red" )
        {
        continue;
        }
      if( this->xmlStream->name() == "Green" )
        {
        continue;
        }
      if( this->xmlStream->name() == "Blue" )
        {
        continue;
        }
      if( this->xmlStream->name() == "Alpha" )
        {
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadMicroscopeSettings()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "MicroscopeSettings" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "Objective" )
        {
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadImageData()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "ImageData" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "PlateList" )
        {
        ReadPlateList();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadPlateList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "PlateList" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "Plate" )
        {
        m_PCoord = this->xmlStream->attributes().value("PCoord").toString().toInt();
        continue;
        }
      if( this->xmlStream->name() == "RowList" )
        {
        ReadRowList();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadRowList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
          ( this->xmlStream->name() != "RowList" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "Row" )
        {
        m_PCoord = this->xmlStream->attributes().value("RCoord").toString().toInt();
        continue;
        }
      if( this->xmlStream->name() == "ColumnList" )
        {
        ReadColumnList();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadColumnList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "ColumnList" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "Column" )
        {
        m_PCoord = this->xmlStream->attributes().value("CCoord").toString().toInt();
        continue;
        }
      if( this->xmlStream->name() == "ZTileList" )
        {
        ReadZTileList();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadZTileList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "ZTileList" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "ZTile" )
        {
        m_PCoord = this->xmlStream->attributes().value("ZTileCoord").toString().toInt();
        continue;
        }
      if( this->xmlStream->name() == "YTileList" )
        {
        ReadYTileList();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadYTileList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "YTileList" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "YTile" )
        {
        m_PCoord = this->xmlStream->attributes().value("YTileCoord").toString().toInt();
        continue;
        }
      if( this->xmlStream->name() == "XTileList" )
        {
        ReadXTileList();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadXTileList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "XTileList" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "XTile" )
        {
        m_PCoord = this->xmlStream->attributes().value("XTileCoord").toString().toInt();
        continue;
        }
      if( this->xmlStream->name() == "TimePointList" )
        {
        ReadTimePointList();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadTimePointList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "TimePointList" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "TimePoint" )
        {
        m_PCoord = this->xmlStream->attributes().value("TCoord").toString().toInt();
        continue;
        }
      if( this->xmlStream->name() == "ZSliceList" )
        {
        ReadZSliceList();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadZSliceList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "ZSliceList" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "image" )
        {
        ReadImage();
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoMegaCaptureReader::ReadImage()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( !this->xmlStream->hasError() ) &&
         ( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "image" ) ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "Filename" )
        {
        continue;
        }
      if( this->xmlStream->name() == "ChannelNumber" )
        {
        continue;
        }
      }
    }
}
// -----------------------------------------------------------------------------
