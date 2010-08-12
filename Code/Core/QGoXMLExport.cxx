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


#include "QGoXMLExport.h"

#include "ConvertToStringHelper.h"
#include "SelectQueryDatabaseHelper.h"
#include "QueryDataBaseHelper.h"

#include "GoDBColorRow.h"
#include "GoDBCoordinateRow.h"

#include <QMessageBox>
#include <QFile>

QGoXMLExport::QGoXMLExport( std::string iServerName, std::string iLogin,
                            std::string iPassword, int iImagingSessionID )
{
  this->m_ServerName = iServerName;
  this->m_Login = iLogin;
  this->m_Password = iPassword;
  this->m_ImagingSessionID = iImagingSessionID;
}

QGoXMLExport::~QGoXMLExport()
{}

void
QGoXMLExport::Write( QString iFilename )
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

  this->xmlStream->setDevice( this->file );
  this->xmlStream->setAutoFormatting( true );
  this->xmlStream->setAutoFormattingIndent( 2 );
  this->xmlStream->writeStartDocument();

  this->xmlStream->writeStartElement( "GoFigure2Traces" );
  this->xmlStream->writeStartElement( "version", "1.0" );

  // -----------------------
  // WriteImagingSession
  WriteImagingSession();
  // -----------------------

  // -----------------------
  // WriteColorList
  WriteColorList();
  // -----------------------

  // -----------------------
  // WriteCoordinateList
  WriteCoordinateList();
  // -----------------------

  // -----------------------
  // WriteTraceList
  this->xmlStream->writeStartElement( "TraceList" ); //<TraceList>

  this->xmlStream->writeStartElement( "TrackList" ); //<TrackList>
  this->xmlStream->writeEndElement(); //</TrackList>

  this->xmlStream->writeStartElement( "MeshList" ); //<MeshList>
  this->xmlStream->writeEndElement(); //</MeshList>

  this->xmlStream->writeStartElement( "ContourList" ); //<ContourList>
  this->xmlStream->writeEndElement(); //</ContourList>

  this->xmlStream->writeEndElement( ); //</TraceList>
  // -----------------------

  this->xmlStream->writeEndElement(); // GoFigure2Traces
  this->xmlStream->writeEndDocument();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoXMLExport::WriteImagingSession()
{
  // <imagingsession>
  this->xmlStream->writeStartElement( "imagingsession" );

  QString temp =
      QString::fromStdString(
          this->GetOneInfoFromDBForImgSession( "Name" ) );

  this->xmlStream->writeTextElement( "Name", temp );

  temp =
      QString::fromStdString(
          this->GetOneInfoFromDBForImgSession( "CreationDate" ) );

  this->xmlStream->writeTextElement( "CreationDate", temp );

  temp =
      QString::fromStdString(
          this->GetOneInfoFromDBForImgSession( "MicroscopeName" ) );

  this->xmlStream->writeTextElement( "MicroscopeName", temp );
  this->xmlStream->writeEndElement();
  // </imagingsession>
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::string
QGoXMLExport::GetOneInfoFromDBForImgSession( std::string iNameInfo )
{
  return ListSpecificValuesForOneColumn(
    this->m_DatabaseConnector,
    "imagingsession",
    iNameInfo,
    "ImagingSessionID",
    ConvertToString<int>(this->m_ImagingSessionID) ).at(0);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoXMLExport::WriteColorList()
{
  // <colorList>
  this->xmlStream->writeStartElement( "colorList" );

  std::vector<std::string>               TablesNames;
  std::vector<std::string>               FieldNames;
  std::vector<std::vector<std::string> > VectorTracesIDs;

  this->GetVectorsTableNamesTracesIDsAndFields( TablesNames, VectorTracesIDs,
                                                FieldNames, true );
  std::vector<std::string> ColumnNames(1);
  ColumnNames[0] = "ColorID";

  std::vector<std::string> ListColorIDs =
      GetSameFieldsFromSeveralTables( this->m_DatabaseConnector, ColumnNames,
                                      TablesNames, FieldNames,
                                      VectorTracesIDs );
  this->WriteTableInfoFromDB< GoDBColorRow >(ListColorIDs);

  this->xmlStream->writeEndElement();
  // </colorList>
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoXMLExport::WriteCoordinateList()
{
  //<coordinateList>
  this->xmlStream->writeStartElement( "coordinateList" );

  std::vector<std::string>               TablesNames;
  std::vector<std::string>               FieldNames;
  std::vector<std::vector<std::string> > VectorTracesIDs;
  this->GetVectorsTableNamesTracesIDsAndFields( TablesNames,
                                                VectorTracesIDs,
                                                FieldNames );

  std::vector<std::string> ColumnNames(2);
  ColumnNames[0] = "CoordIDMax";
  ColumnNames[1] = "CoordIDMin";
  std::vector<std::string> ListCoordIDs =
      GetSameFieldsFromSeveralTables( this->m_DatabaseConnector, ColumnNames,
                                      TablesNames, FieldNames,
                                      VectorTracesIDs );

  this->WriteTableInfoFromDB<GoDBCoordinateRow>( ListCoordIDs );

  this->xmlStream->writeEndElement( );
  //</coordinateList>
}

