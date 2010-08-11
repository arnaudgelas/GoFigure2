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

#ifndef QGOXMLIMPORT_H
#define QGOXMLIMPORT_H

#include <QObject>

#include <map>
#include <QXmlStreamReader>
#include "vtkMySQLDatabase.h"

class QString;
class QFile;

class QGoXMLImport : public QObject
{
  Q_OBJECT

public:
  QGoXMLImport( std::string iServerName, std::string iLogin,
                std::string iPassword, int iImagingSessionID );
  ~QGoXMLImport();

  void Read( QString iFilename );

protected:
  void ReadImagingSession();
  void ReadTraceList();
  void ReadTrackList();
  void ReadTrack();
  void ReadMeshList();
  void ReadMesh();
  void ReadIntensityList();
  void ReadIntensity();
  void ReadChannelList();

  vtkMySQLDatabase* m_DatabaseConnector;
  std::string       m_ServerName;
  std::string       m_Password;
  std::string       m_Login;
  int               m_ImagingSessionID;

  QXmlStreamReader* xmlStream;
  QFile* file;

  std::map< int, int > m_ColorMap;
  std::map< int, int > m_CoordinateMap;
  std::map< int, int > m_ChannelMap;
  std::map< int, int > m_CellTypeMap;
  std::map< int, int > m_SubCellularTypeMap;

  void OpenDBConnection();
  void CloseDBConnection();

  // -----------------------------------------------------------------------------
  template<class TGoDBRow>
  void ReadDBEntityList( std::map< int, int >& ioMap )
  {
    QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
    QString temp;

    TGoDBRow temp_row;
    QString Entity = QString::fromStdString( temp_row.GetTableName() );
    QString EntityList = Entity;
    EntityList += "List";

    int id;

    while( ( token != QXmlStreamReader::EndElement ) ||
           ( this->xmlStream->name() != EntityList ) )
      {
      token = this->xmlStream->readNext();

      if( token == QXmlStreamReader::StartElement )
        {
        if( this->xmlStream->name() == Entity )
          {
          id = this->xmlStream->attributes().value("id").toString().toInt();
          ioMap[id] = this->ReadDBEntity<TGoDBRow>();
          continue;
          }
        }
      }
  }
  // -----------------------------------------------------------------------------

  // -----------------------------------------------------------------------------
  template<class TGoDBRow>
  int ReadDBEntity()
  {
    QXmlStreamReader::TokenType token = this->xmlStream->tokenType();
    QString name;
    QString value;
    TGoDBRow row;

    while( ( token != QXmlStreamReader::EndElement ) ||
           ( this->xmlStream->name() != row.GetTableName().c_str() ) )
      {
      token = this->xmlStream->readNext();

      if( token == QXmlStreamReader::StartElement )
        {
        name = this->xmlStream->name().toString();
        value = this->xmlStream->readElementText();

        row.SetField( name.toStdString(), value.toStdString() );
        }
      }

    // here catch the exception if the field is not an element of TGoDBRow

    return row.SaveInDB( this->m_DatabaseConnector );
  }
  // -----------------------------------------------------------------------------
private:
  Q_DISABLE_COPY( QGoXMLImport );
};

#endif // QGOXMLIMPORT_H
