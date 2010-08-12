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

#ifndef __QGoXMLExport_h
#define __QGoXMLExport_h

#include <QObject>
#include "vtkMySQLDatabase.h"
#include <QXmlStreamWriter>

#include <vector>

class QFile;

class QGoXMLExport : public QObject
{
  Q_OBJECT
public:
  QGoXMLExport( std::string iServerName, std::string iLogin,
                std::string iPassword, int iImagingSessionID );
  ~QGoXMLExport();

  void Write( QString iFilename );

protected:
  vtkMySQLDatabase* m_DatabaseConnector;
  std::string       m_ServerName;
  std::string       m_Password;
  std::string       m_Login;
  int               m_ImagingSessionID;

  QXmlStreamWriter* xmlStream;
  QFile* file;

  void WriteImagingSession();
  void WriteColorList();
  void WriteCoordinateList();

  std::string GetOneInfoFromDBForImgSession( std::string iNameInfo );

  /** \brief fill the different vectors needed for the queries
  depending if the vectors of IDs are empty or not: get the
  tables names, the key for the table and the tracesIDs. */
  void GetVectorsTableNamesTracesIDsAndFields(
    std::vector<std::string>& ioVectorTableNames,
    std::vector<std::vector<std::string> >& ioVectorTracesIDs,
    std::vector<std::string>& ioVectorFields,
    bool IncludeChannelIDs = false );

  //---------------------------------------------------------------------------
  /** \brief get the info with their names for an entity from the database
  and put them in a vector of pair of string (name of the info + value of
  the info)*/
  template<typename T>
  std::vector<std::pair<std::string, std::string> >
  GetOneEntityInfoFromDB( std::string iEntityID, T iTableRow )
  {
    typedef std::pair< std::string, std::string > StringPairType;
    typedef std::vector< StringPairType > VectorOfStringPairType;
    typedef typename VectorOfStringPairType::iterator
        VectorOfStringPairIterator;

    VectorOfStringPairType oEntityInfo;
    iTableRow.SetValuesForSpecificID( atoi(iEntityID.c_str() ),
                                      this->m_DatabaseConnector );

    std::vector<std::string> FieldNames = iTableRow.GetVectorColumnNames();

    std::vector<std::string>::iterator iter = FieldNames.begin();

    std::string temp;
    while (iter != FieldNames.end())
      {
      temp = iTableRow.GetMapValue(*iter);
      oEntityInfo.push_back(StringPairType( *iter, temp ) );
      ++iter;
      }
    return oEntityInfo;
  }
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------
  template< class TGoDBRow >
  void WriteTableInfoFromDB( std::vector<std::string> iListIDs )
  {
    typedef std::vector<std::pair<std::string, std::string> >
        VectorOfStringPairType;
    typedef typename VectorOfStringPairType::iterator
        VectorOfStringPairIterator;

    if( !iListIDs.empty() )
      {
      TGoDBRow TableRow;
      std::string table_name = TableRow.GetTableName();

      VectorOfStringPairType EntityInfo;
      VectorOfStringPairIterator e_it;

      unsigned int k = 0;
      std::vector<std::string>::iterator iter = iListIDs.begin();

      while (iter != iListIDs.end())
        {
        EntityInfo = this->GetOneEntityInfoFromDB(*iter, TableRow);

        this->xmlStream->writeStartElement(
            QString::fromStdString( table_name ) );

        this->xmlStream->writeAttribute( "id",
                                         QString("%d").arg(k) );

        // for loop on all field from one table
        e_it = EntityInfo.begin();

        while (e_it != EntityInfo.end())
          {
          this->xmlStream->writeTextElement(
              QString::fromStdString( e_it->first ),
              QString::fromStdString( e_it->second ) );
          ++e_it;
          }
        ++iter;
        ++k;
        }
      }
  }
  //---------------------------------------------------------------------------

private:
  Q_DISABLE_COPY( QGoXMLExport );
};

#endif
