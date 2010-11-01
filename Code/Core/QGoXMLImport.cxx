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

#include "QGoXMLImport.h"

#include "SelectQueryDatabaseHelper.h"
#include "QueryDataBaseHelper.h"

#include <QXmlStreamReader>
#include <QMessageBox>
#include <QString>
#include <QFile>

#include "GoDBTrackRow.h"
#include "GoDBMeshRow.h"
#include "GoDBCellTypeRow.h"
#include "GoDBIntensityRow.h"
#include "GoDBChannelRow.h"
#include "GoDBSubCellTypeRow.h"
#include "GoDBColorRow.h"
#include "GoDBChannelRow.h"

QGoXMLImport::QGoXMLImport( std::string iServerName, std::string iLogin,
                            std::string iPassword, int iImagingSessionID )
{
  this->xmlStream = new QXmlStreamReader;
  this->file = new QFile( this );

  this->m_ServerName = iServerName;
  this->m_Login = iLogin;
  this->m_Password = iPassword;
  this->m_ImagingSessionID = iImagingSessionID;

  this->OpenDBConnection();
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
QGoXMLImport::~QGoXMLImport()
{
  this->CloseDBConnection();
  delete this->xmlStream;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoXMLImport::OpenDBConnection()
{
  this->m_DatabaseConnector =
      OpenDatabaseConnection( m_ServerName, m_Login, m_Password,
                              "gofiguredatabase");
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void QGoXMLImport::CloseDBConnection()
{
  CloseDatabaseConnection(m_DatabaseConnector);
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void
QGoXMLImport::ReadImagingSession()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  QString temp;

  while( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "imagingsession" ) )
    {
    token = this->xmlStream->readNext();

    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "Name" )
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
      if( this->xmlStream->name() == "MicroscopeName" )
        {
        // get the name from the imaging session
        temp = this->xmlStream->readElementText();
        }
      }
    }
}
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
void
QGoXMLImport::ReadTraceList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();

  while( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "TraceList" ) )
    {
    token = this->xmlStream->readNext();

    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "TrackList" )
        {
        ReadTrackList();
        continue;
        }
      if( this->xmlStream->name() == "MeshList" )
        {
        ReadMeshList();
        continue;
        }
      if( this->xmlStream->name() == "ContourList" )
        {
        ReadContourList();
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void
QGoXMLImport::ReadContourList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();

  int id;

  while( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "ContourList" ) )
    {
    token = this->xmlStream->readNext();

    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "contour" )
        {
        id = this->xmlStream->attributes().value("id").toString().toInt();
        this->m_ContourMap[id] = ReadContour();
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void
QGoXMLImport::ReadTrackList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  int id;

  while( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "TrackList" ) )
    {
    token = this->xmlStream->readNext();

    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "track" )
        {
        id = this->xmlStream->attributes().value("id").toString().toInt();
        this->m_TrackMap[id] = ReadTrack();
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void
QGoXMLImport::ReadMeshList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();

  int id;

  while( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "MeshList" ) )
    {
    token = this->xmlStream->readNext();

    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "mesh" )
        {
        id = this->xmlStream->attributes().value("id").toString().toInt();
        this->m_MeshMap[id] = ReadMesh();
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
int
QGoXMLImport::ReadContour()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();

  QString temp;
  GoDBContourRow contour_row;
  int temp_id;

  while( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "contour" ) )
    {
    token = this->xmlStream->readNext();

    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "Points" )
        {
        temp = this->xmlStream->readElementText();
        contour_row.SetField( "Points", temp.toStdString() );
        continue;
        }
     if( this->xmlStream->name() == "ColorID" )
        {
        temp = this->xmlStream->attributes().value( "color" ).toString();
        temp_id = temp.toInt();
        contour_row.SetField( "ColorID", this->m_ColorMap[temp_id] );
        continue;
        }
      if( this->xmlStream->name() == "CoordIDMax" )
        {
        temp = this->xmlStream->attributes().value( "coordinate" ).toString();
        temp_id = temp.toInt();
        contour_row.SetField( "CoordIDMax", this->m_CoordinateMap[temp_id] );
        continue;
        }
      if( this->xmlStream->name() == "CoordIDMin" )
        {
        temp = this->xmlStream->attributes().value( "coordinate" ).toString();
        temp_id = temp.toInt();
        contour_row.SetField( "CoordIDMax", this->m_CoordinateMap[temp_id] );
        continue;
        }
      }
    }
  return contour_row.SaveInDB( this->m_DatabaseConnector );
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
int
QGoXMLImport::ReadMesh()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();

  QString temp;
  GoDBMeshRow mesh_row;
  int temp_id;

  while( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "mesh" ) )
    {
    token = this->xmlStream->readNext();

    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "Points" )
        {
        temp = this->xmlStream->readElementText();
        mesh_row.SetField( "Points", temp.toStdString() );
        continue;
        }
     if( this->xmlStream->name() == "CellTypeID" )
        {
        temp = this->xmlStream->attributes().value( "celltype" ).toString();
        temp_id = temp.toInt();
        mesh_row.SetField( "CellTypeID", this->m_CellTypeMap[temp_id] );
        continue;
        }
      if( this->xmlStream->name() == "ColorID" )
        {
        temp = this->xmlStream->attributes().value( "color" ).toString();
        temp_id = temp.toInt();
        mesh_row.SetField( "ColorID", this->m_ColorMap[temp_id] );
        continue;
        }
      if( this->xmlStream->name() == "CoordIDMax" )
        {
        temp = this->xmlStream->attributes().value( "coordinate" ).toString();
        temp_id = temp.toInt();
        mesh_row.SetField( "CoordIDMax", this->m_CoordinateMap[temp_id] );
        continue;
        }
      if( this->xmlStream->name() == "CoordIDMin" )
        {
        temp = this->xmlStream->attributes().value( "coordinate" ).toString();
        temp_id = temp.toInt();
        mesh_row.SetField( "CoordIDMax", this->m_CoordinateMap[temp_id] );
        continue;
        }
      if( this->xmlStream->name() == "SubCellularID" )
        {
        temp = this->xmlStream->attributes().value( "subcellular" ).toString();
        temp_id = temp.toInt();
        mesh_row.SetField( "CoordIDMax", this->m_SubCellularTypeMap[temp_id] );
        continue;
        }
      if( this->xmlStream->name() == "TrackID" )
        {
        temp = this->xmlStream->attributes().value( "track" ).toString();
        temp_id = temp.toInt();
        mesh_row.SetField( "CoordIDMax", this->m_TrackMap[temp_id] );
        continue;
        }
      }
    }
  return mesh_row.SaveInDB( this->m_DatabaseConnector );
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void
QGoXMLImport::ReadIntensityList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();

  while( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "IntensityList" ) )
    {
    token = this->xmlStream->readNext();

    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "intensity" )
        {
        ReadIntensity();
        }
      }
    }
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
int
QGoXMLImport::ReadIntensity()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  GoDBIntensityRow  row;

  QString temp;
  int temp_id;

  while( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "intensity" ) )
    {
    token = this->xmlStream->readNext();

    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "ChannelID" )
        {
        temp = this->xmlStream->readElementText();
        temp_id = temp.toInt();
        row.SetField( "ChannelID", this->m_ChannelMap[temp_id] );
        continue;
        }
      if( this->xmlStream->name() == "MeshID" )
        {
        temp = this->xmlStream->readElementText();
        temp_id = temp.toInt();
        row.SetField( "MeshID", this->m_MeshMap[temp_id] );
        continue;
        }
      if( this->xmlStream->name() == "Value" )
        {
        temp = this->xmlStream->readElementText();
        temp_id = temp.toInt();
        row.SetField( "Value", temp_id );
        }
      }

    }

  return row.SaveInDB( this->m_DatabaseConnector );
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
int
QGoXMLImport::ReadChannel()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  GoDBChannelRow channel_row;
  QString field;
  QString value;

  while( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "channel" ) )
    {
    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "ColorID" )
        {
        int temp_id = this->xmlStream->attributes().value("color").toString().toInt();
        channel_row.SetField( "ColorID", m_ColorMap[temp_id] );
        }
      else
        {
        field = this->xmlStream->name().toString();
        value = this->xmlStream->readElementText();
        channel_row.SetField( field.toStdString(), value.toStdString() );
        }
      }
    }
  return channel_row.SaveInDB( this->m_DatabaseConnector );
}

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void
QGoXMLImport::ReadChannelList()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();


  while( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "ChannelList" ) )
    {
    token = this->xmlStream->readNext();

    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "channel" )
        {
        int temp_id = this->xmlStream->attributes().value("id").toString().toInt();
        this->m_ChannelMap[temp_id] = ReadChannel();
        }
      }
    }
}

// -----------------------------------------------------------------------------
int
QGoXMLImport::ReadTrack()
{
  QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
  GoDBTrackRow track_row;
  QString temp;
  int temp_id;

  while( ( token != QXmlStreamReader::EndElement ) ||
         ( this->xmlStream->name() != "track" ) )
    {
    token = this->xmlStream->readNext();
    temp = this->xmlStream->name().toString();

    if( token == QXmlStreamReader::StartElement )
      {
      if( this->xmlStream->name() == "ColorID" )
        {
        temp_id = this->xmlStream->attributes().value("coordinate").toString().toInt();
        track_row.SetField( "ColorID", m_ColorMap[temp_id] );
        continue;
        }
      if( this->xmlStream->name() == "CoordIDMax" )
        {
        temp_id = this->xmlStream->attributes().value("coordinate").toString().toInt();
        track_row.SetField( "CoordIDMax", m_CoordinateMap[temp_id] );
        continue;
        }
      if( this->xmlStream->name() == "CoordIDMin" )
        {
        temp_id = this->xmlStream->attributes().value("coordinate").toString().toInt();
        track_row.SetField( "CoordIDMin", m_CoordinateMap[temp_id] );
        continue;
        }
      if( this->xmlStream->name() == "LineageID" )
        {
        temp = this->xmlStream->readElementText();
        //track_row.SetField( "LineageID", )
        continue;
        }
      if( this->xmlStream->name() == "TrackFamilyID" )
        {
        temp = this->xmlStream->readElementText();
        }
      }
    }

  return track_row.SaveInDB( this->m_DatabaseConnector );
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void
QGoXMLImport::Read( QString iFilename )
{
  if( ( iFilename.isEmpty() ) || ( iFilename.isNull() ) )
    {
    QMessageBox::critical(NULL,
                          "QGoXMLImport",
                          "empty filename",
                          QMessageBox::Ok);
    return;
    }

  this->file->setFileName( iFilename );

  if (!this->file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
    QMessageBox::critical(NULL,
                          "QGoXMLImport",
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
      if( ( this->xmlStream->name() == "GoFigure2Traces" )  )
        {
        if( this->xmlStream->attributes().value("version") == "1.0" )
          {
          continue;
          }
        else
          {
          std::cout <<"Error wrong version number" <<std::endl;
          return;
          }
      }
        if( this->xmlStream->name() == "imagingsession" )
          {
          ReadImagingSession();
          continue;
          }
        if( this->xmlStream->name() == "colorList")
          {
          ReadDBEntityList<GoDBColorRow>( m_ColorMap );
          continue;
          }
        if( this->xmlStream->name() == "channelList")
          {
          ReadDBEntityList<GoDBChannelRow>( m_ChannelMap );
          continue;
          }
        if( this->xmlStream->name() == "celltypeList")
          {
          ReadDBEntityList<GoDBCellTypeRow>( m_CellTypeMap );
          continue;
          }
        if( this->xmlStream->name() == "subcellulartypeList")
          {
          ReadDBEntityList<GoDBSubCellTypeRow>( m_SubCellularTypeMap );
          continue;
          }
        if( this->xmlStream->name() == "coordinateList")
          {
          ReadDBEntityList<GoDBCoordinateRow>( m_CoordinateMap );
          continue;
          }
        if( this->xmlStream->name() == "TraceList" )
          {
          ReadTraceList();
          continue;
          }
        if( this->xmlStream->name() == "IntensityList" )
          {
          ReadIntensityList();
          }
        }

    }
  if(this->xmlStream->hasError())
    {
    QMessageBox::critical(NULL,
                          "xmltest",
                          this->xmlStream->errorString(),
                          QMessageBox::Ok);
    }
  this->file->close();
  this->xmlStream->clear();
}
// -----------------------------------------------------------------------------