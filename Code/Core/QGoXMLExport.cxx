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
#include "GoDBChannelRow.h"
#include "GoDBCellTypeRow.h"
#include "GoDBSubCellTypeRow.h"

#include <QMessageBox>
#include <QFile>

QGoXMLExport::QGoXMLExport( std::string iServerName, std::string iLogin,
                            std::string iPassword, int iImagingSessionID )
{
  this->xmlStream = new QXmlStreamWriter;
  this->xmlStream->setAutoFormatting( true );
  this->xmlStream->setAutoFormattingIndent( 2 );

  this->file = new QFile( this );

  this->m_ServerName = iServerName;
  this->m_Login = iLogin;
  this->m_Password = iPassword;
  this->m_ImagingSessionID = iImagingSessionID;

}

QGoXMLExport::~QGoXMLExport()
{
  delete this->xmlStream;
}

//--------------------------------------------------------------------------
void QGoXMLExport::OpenDBConnection()
{
  this->m_DatabaseConnector =
      OpenDatabaseConnection( m_ServerName, m_Login, m_Password,
                              "gofiguredatabase" );
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoXMLExport::CloseDBConnection()
{
  CloseDatabaseConnection(m_DatabaseConnector);
}
//--------------------------------------------------------------------------

void
QGoXMLExport::Write( QString iFilename )
{
  if( ( iFilename.isEmpty() ) || ( iFilename.isNull() ) )
    {
    QMessageBox::critical(NULL,
                          "QGoXMLExport",
                          "empty filename",
                          QMessageBox::Ok);
    return;
    }

  this->file->setFileName( iFilename );

  if ( !this->file->open( QIODevice::WriteOnly ) )
    {
    QMessageBox::critical(NULL,
                          "QGoXMLExport",
                          QString( "Failed to write %d" ).arg( iFilename ),
                          QMessageBox::Ok);
    return;
    }

  this->xmlStream->setDevice( this->file );

  this->xmlStream->writeStartDocument();

  this->xmlStream->writeStartElement( "GoFigure2Traces" );
  this->xmlStream->writeAttribute( "version", "1.0" );

  this->OpenDBConnection();

  // -----------------------
  // WriteImagingSession
  WriteImagingSession();


  // -----------------------
  this->UpdateAllVectorTracesIDsToExportContours();

  // -----------------------
  // WriteColorList
  WriteColorList();
  // -----------------------

  // -----------------------
  // WriteChannelList
  //WriteChannelList();
  // -----------------------

  // -----------------------
  // WriteCellTypeList
  WriteCellTypeList();
  // -----------------------

  // -----------------------
  // WriteSubCellularTypeList
  WriteSubCellularTypeList();
  // -----------------------

  // -----------------------
  // WriteCoordinateList
  WriteCoordinateList();
  // -----------------------

  // -----------------------
  // WriteTraceList
  this->WriteTraceList();
  // -----------------------

  this->CloseDBConnection();
  // </GoFigure2Traces>
  this->xmlStream->writeEndElement();
  this->xmlStream->writeEndDocument();

  this->file->close();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoXMLExport::UpdateAllVectorTracesIDsToExportContours()
{
  this->UpdateVectorContourIDsForExportContours();
  this->UpdateVectorMeshIDsForExportContours();
  this->UpdateVectorTrackIDsToExportInfo();
  this->UpdateVectorLineageIDsToExportInfo();

  //no need for channel info when exporting contours at this time:
  this->m_VectorChannelIDs.clear();
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
  this->WriteTableInfoFromDB< GoDBColorRow >( ListColorIDs, m_ColorMap );

  this->xmlStream->writeEndElement();
  // </colorList>
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoXMLExport::WriteChannelList()
{
  //<channelList>
  this->xmlStream->writeStartElement( "channelList" );

  this->WriteTableInfoFromDB<GoDBChannelRow>( this->m_VectorChannelIDs,
                                              this->m_ChannelMap );

  this->xmlStream->writeEndElement();
  //</channelList>
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoXMLExport::WriteChannel( unsigned int iId )
{
  // <channel id="0">
  this->xmlStream->writeStartElement( "channel" );
  this->xmlStream->writeAttribute( "id", QString("%d").arg(iId) );

  // <ColorID color="0"/>
  this->xmlStream->writeEmptyElement( "ColorID" );
  this->xmlStream->writeAttribute( "color", "0" );//QString("%d").arg( ) );

  // <Name>name</Name>
  this->xmlStream->writeTextElement( "Name", "name" );

  // <NumberOfBits>8</NumberOfBits>
  this->xmlStream->writeTextElement( "NumberOfBits", "8" );
  this->xmlStream->writeEndElement();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoXMLExport::WriteCellTypeList()
{
  //<celltypeList>
  this->xmlStream->writeStartElement( "celltypeList" );

  std::vector<std::string> ListCellTypeIDs =
      ListSpecificValuesForOneColumn( this->m_DatabaseConnector, "mesh",
                                      "CellTypeID", "MeshID",
                                      this->m_VectorMeshIDs, true, true );
  this->WriteTableInfoFromDB<GoDBCellTypeRow>( ListCellTypeIDs,
                                               this->m_CellTypeMap );


  this->xmlStream->writeEndElement( );
  //</celltypeList>
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoXMLExport::WriteSubCellularTypeList()
{
  //<celltypeList>
  this->xmlStream->writeStartElement( "subcellulartypeList" );

  std::vector<std::string> ListSubCellTypeIDs =
      ListSpecificValuesForOneColumn( this->m_DatabaseConnector, "mesh",
                                      "SubCellularID", "MeshID",
                                      this->m_VectorMeshIDs, true, true );

  this->WriteTableInfoFromDB<GoDBSubCellTypeRow>( ListSubCellTypeIDs,
                                                  m_SubCellularTypeMap );


  this->xmlStream->writeEndElement( );
  //</celltypeList>
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

  this->WriteTableInfoFromDB<GoDBCoordinateRow>( ListCoordIDs,
                                                 m_CoordinateMap );

  this->xmlStream->writeEndElement( );
  //</coordinateList>
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoXMLExport::WriteTraceList()
{
  //<TraceList>
  this->xmlStream->writeStartElement( "TraceList" );

  WriteTrackList();
  WriteMeshList();
  WriteContourList();

  this->xmlStream->writeEndElement( );
  //</TraceList>
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoXMLExport::WriteTrackList()
{
  //<TrackList>
  this->xmlStream->writeStartElement( "TrackList" );

  this->xmlStream->writeEndElement();
  //</TrackList>
}

//--------------------------------------------------------------------------
void
QGoXMLExport::WriteMeshList()
{
  //<MeshList>
  this->xmlStream->writeStartElement( "MeshList" );
  this->xmlStream->writeEndElement();
  //</MeshList>
}

//--------------------------------------------------------------------------
void
QGoXMLExport::WriteContourList()
{
  //<ContourList>
  this->xmlStream->writeStartElement( "ContourList" );
  this->xmlStream->writeEndElement();
  //</ContourList>
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoXMLExport::
UpdateVectorContourIDsForExportContours()
{
  std::string imagingsession_id =
      ConvertToString<int>(this->m_ImagingSessionID);

  this->m_VectorContourIDs =
    ListSpecificValuesForOneColumn( this->m_DatabaseConnector, "contour",
                                    "ContourID", "imagingsessionID",
                                    imagingsession_id );
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoXMLExport::
UpdateVectorContourIDsForExportMeshes()
{
  this->m_VectorContourIDs.clear();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoXMLExport::UpdateVectorMeshIDsForExportContours()
{
  this->m_VectorMeshIDs = ListSpecificValuesForOneColumn(
    this->m_DatabaseConnector, "contour", "meshID", "ContourID",
    this->m_VectorContourIDs, true, true);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoXMLExport::UpdateVectorMeshIDsForExportMeshes()
{
  std::string imagingsession_id =
      ConvertToString<int>(this->m_ImagingSessionID);

  this->m_VectorMeshIDs =
      FindSeveralIDs( this->m_DatabaseConnector,"mesh", "MeshID",
                      "ImagingSessionID", imagingsession_id, "Points", "0" );
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoXMLExport::UpdateVectorChannelIDsForExportMeshes()
{
  std::string imagingsession_id =
      ConvertToString<int>(this->m_ImagingSessionID);

  this->m_VectorChannelIDs =
      ListSpecificValuesForOneColumn( this->m_DatabaseConnector, "channel",
                                      "ChannelID", "ImagingSessionID",
                                      imagingsession_id );
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoXMLExport::UpdateVectorTrackIDsToExportInfo()
{
  if (!this->m_VectorMeshIDs.empty())
    {
    this->m_VectorTrackIDs =
        ListSpecificValuesForOneColumn( this->m_DatabaseConnector, "mesh",
                                        "TrackID", "MeshID",
                                        this->m_VectorMeshIDs, true, true );
    }
  else
    {
    this->m_VectorTrackIDs.clear();
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoXMLExport::UpdateVectorLineageIDsToExportInfo()
{
  if (!this->m_VectorTrackIDs.empty())
    {
    this->m_VectorLineageIDs =
        ListSpecificValuesForOneColumn( this->m_DatabaseConnector, "track",
                                        "LineageID", "TrackID",
                                        this->m_VectorTrackIDs, true, true );
    }
  else
    {
    this->m_VectorLineageIDs.clear();
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void QGoXMLExport::GetVectorsTableNamesTracesIDsAndFields(
  std::vector<std::string>& ioVectorTableNames,
  std::vector<std::vector<std::string> >& ioVectorTracesIDs,
  std::vector<std::string>& ioVectorFields,
  bool IncludeChannelIDs )
{
  if (!this->m_VectorContourIDs.empty())
    {
    ioVectorTableNames.push_back("contour");
    ioVectorFields.push_back("ContourID");
    ioVectorTracesIDs.push_back(this->m_VectorContourIDs);
    }
  if (!this->m_VectorMeshIDs.empty())
    {
    ioVectorTableNames.push_back("mesh");
    ioVectorFields.push_back("MeshID");
    ioVectorTracesIDs.push_back(this->m_VectorMeshIDs);
    }
  if (!this->m_VectorTrackIDs.empty())
    {
    ioVectorTableNames.push_back("track");
    ioVectorFields.push_back("TrackID");
    ioVectorTracesIDs.push_back(this->m_VectorTrackIDs);
    }
  if (!this->m_VectorLineageIDs.empty())
    {
    ioVectorTableNames.push_back("lineage");
    ioVectorFields.push_back("LineageID");
    ioVectorTracesIDs.push_back(this->m_VectorLineageIDs);
    }
  if (IncludeChannelIDs)
    {
    if (!this->m_VectorChannelIDs.empty())
      {
      ioVectorTableNames.push_back("channel");
      ioVectorFields.push_back("ChannelID");
      ioVectorTracesIDs.push_back(this->m_VectorChannelIDs);
      }
    }
}
//--------------------------------------------------------------------------
