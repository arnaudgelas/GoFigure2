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
#ifndef __GoDBCollectionOfTraces_h
#define __GoDBCollectionOfTraces_h

#include "MegaVTK2Configure.h"
#include "GoDBRecordSetHelper.h"
#include "vtkMySQLDatabase.h"
#include "GoDBRow.h"
#include "GoDBCoordinateRow.h"
#include "GoDBTraceInfoForTableWidget.h"
#include "GoDBTableWidgetContainer.h"
#include <map>
#include <QColor>

/**
\class GoDBCollectionOfTraces
\brief this class handles the interaction between the database and the children
of QGoDBTraceManager
\ingroup DB
*/
class QGOIO_EXPORT GoDBCollectionOfTraces
{
public:

  GoDBCollectionOfTraces();
  //refactoring
  GoDBCollectionOfTraces(
    std::string CollectionName, std::string Traces,
    std::string iCollectionOfName, unsigned int iImgSessionID);
  virtual ~GoDBCollectionOfTraces();

  typedef GoDBTableWidgetContainer::TWContainerType TWContainerType;
  typedef std::pair< std::string, QColor >          NameWithColorData;

  /** 
  \brief fill the global values for the collection of traces.
  \param[in] iCollectionName name of the collection exp: track
  \param[in] iTracesName name of the trace exp:mesh
  \param[in] iCollectionOfName name of the collectionOf exp: contour
  */
  void SetCollectionInfo(std::string iCollectionName,
                         std::string iTracesName,
                         std::string iCollectionOfName);

  /**
  \brief set m_ImgSessionID to iImgSessionID
  */
  void SetImgSessionID(unsigned int iImgSessionID);

  //Modif into Database
  /** 
  \brief Delete the corresponding trace in the database
  \param[in] DatabaseConnector connection to the database
  */
  void DeleteTraceInDB(int TraceToDelete, vtkMySQLDatabase *DatabaseConnector);

  //Modif into Database
  /** 
  \brief Delete in the Database all the traces listed in the list of int 
  \param[in] TracesToDelete delete from the database the traces with the
  ID listed in it
  \param[in] DatabaseConnector connection to the database
  */
  void DeleteTracesInDB(std::list< unsigned int > TracesToDelete,
                        vtkMySQLDatabase *DatabaseConnector);

  /** 
  \brief Update the collectionID of the selected traces in the DB trace table,
  update the bounding box of the collection, update the bounding boxes of the
  previous collections the traces belonged to and return the list of the
  previous collection with an updated bounding box
  */
  //std::list< unsigned int > UpdateDBDataForAddedTracesToExistingCollection(
  //  std::list< unsigned int > ListSelectedTraces, int iNewCollectionID,
  //  vtkMySQLDatabase *DatabaseConnector);

  //Modif into Database
  /** 
  \brief Update the collectionID of the selected traces in the DB traces table
  with the new collectionID
  \param[in] iListSelectedTraces IDs of the traces the collectionID need to 
  be updated
  \param[in] iCollectionID new collectionID
  \param[in] DatabaseConnector connection to the database
  */
  void UpdateCollectionIDOfSelectedTraces(
    std::list< unsigned int > iListSelectedTraces, unsigned int iCollectionID,
    vtkMySQLDatabase *iDatabaseConnector);

  /** 
  \brief 
  \return the name of the trace of the current collection which
  is also a collection of
  */
  std::string GetCollectionOf();

  /** 
  \brief get the IDs of the traces part of the collection with iCollectionID
  \param[in] DatabaseConnector connection to the database
  \param[in] iCollectionID ID of the collection the traces needed are part of
  \return the list of IDs for the traces part of the collectionID
  */
  std::list< int > GetTracesIDPartOfTheCollection(
    vtkMySQLDatabase *DatabaseConnector, int iCollectionID);

  //******************************Modif-Refactoring************************************************
  //public:
  //Modif into Database
  /** 
  \brief Calculate the bounding box of the corresponding collection and update
  it in the database
  \param[in] DatabaseConnector connection to the database
  \param[in] iCollectionID ID of the collection the bounding box is calculated
  */
  void RecalculateDBBoundingBox(
    vtkMySQLDatabase *iDatabaseConnector, int iCollectionID);

  /** 
  \brief Get the list of all the collectionIDs, distinct and different from zero for the
  corresponding traces IDs and recalculate the bounding boxes for them.
  \param[in] DatabaseConnector connection to the database
  \param[in] iListTracesIDs list of the tracesIDs the collection need to be recalculated
  */
  void RecalculateDBBoundingBox(
    vtkMySQLDatabase *iDatabaseConnector, std::list< unsigned int > iListTracesIDs);

  /**
  \brief get all the distinct traces IDs with their QColor
  \param[in] iDatabaseConnector connection to the database
  \return std::list<NameWithColorData> list of the TracesIDs with their QColor
  */
  std::list< NameWithColorData > GetAllTracesIDsWithColor(
    vtkMySQLDatabase *iDatabaseConnector);

  /**
  \brief get all the distinct traces IDs with their QColor for a given timepoint
  \param[in] iDatabaseConnector connection to the database
  \param[in] iTimePoint timepoint for which the tracesIDs are expected
  \return std::list<NameWithColorData> list of the TracesIDs with their QColor
  */
  std::list< NameWithColorData > GetTracesIDsWithColorForATimePoint(
    vtkMySQLDatabase *iDatabaseConnector, unsigned int iTimePoint);

  /**
  \brief save the collection in the database after getting an empty bounding box
  and return the corresponding ID
  \param[in] iDatabaseConnector connection to the database
  \param[in] iColor color for the new collection
  \param[in] iNewCollection collection with all the fields set except bounding box
  \param[in] iTimePoint timepoint for the collection (only for mesh)
  \tparam T child of GoDBTraceRow
  */
  template< typename T >
  int CreateCollectionWithNoTracesNoPoints(vtkMySQLDatabase *iDatabaseConnector,
                                           NameWithColorData iColor,
                                           T iNewCollection, int iTimePoint = -1)
  {
    iNewCollection.SetField("ImagingSessionID", this->m_ImgSessionID);

    int CoordIDMax = GetCoordIDMaxForBoundingBoxWithNoTraces(iDatabaseConnector);
    int CoordIDMin = GetCoordIDMinForBoundingBoxWithNoTraces(iDatabaseConnector);

    if ( iTimePoint != -1 )
      {
      this->SetTheTimePointCoordinatesForMesh(
        iTimePoint, CoordIDMax, CoordIDMin, iDatabaseConnector);
      }
    std::string CollectionID = iNewCollection.GetMapValue(this->m_CollectionIDName);
    if ( CollectionID != "0" )
      {
      return this->CreateNewTraceInDB< T >( iNewCollection, iDatabaseConnector,
                                            CoordIDMin, CoordIDMax, iColor, ss_atoi< unsigned int >(CollectionID) );
      }
    else
      {
      return this->CreateNewTraceInDB< T >(iNewCollection, iDatabaseConnector,
                                           CoordIDMin, CoordIDMax, iColor, 0);
      }
  }

  /**
  \brief set the bounding box, the color and the collectionID of the trace,save it in
  the database and return the new TraceID created
  \param[in] iTrace
  \param[in] iDatabaseConnector connection to the database
  \param[in] iColor QColor and Name of the color for the trace
  \param[in] iCollectionID ID of the collection for the trace
  \tparam T children of GoDBTraceRow
  \return unsigned int New created TraceID
  */
  template< typename T >
  unsigned int CreateNewTraceInDB(T iTrace, vtkMySQLDatabase *iDatabaseConnector,
                                  NameWithColorData iColor, unsigned int iCollectionID)
  {
    iTrace.SetColor(iColor.second.red(), iColor.second.green(),
                    iColor.second.blue(), iColor.second.alpha(), iColor.first,
                    iDatabaseConnector);

    iTrace.SetCollectionID(iCollectionID);
    return iTrace.SaveInDB(iDatabaseConnector);
  }

  /**
  \overload CreateNewTraceInDB(T iTrace, vtkMySQLDatabase *iDatabaseConnector,
                                  NameWithColorData iColor, unsigned int iCollectionID)
  \param[in] iCoordIDMin coordinateID of the minimum of the bounding box
  \param[in] iCoordIDMax coordinateID of the maximum of the bounding box
  */
  template< typename T >
  unsigned int CreateNewTraceInDB(T iTrace, vtkMySQLDatabase *iDatabaseConnector,
                                  unsigned int iCoordIDMin, unsigned int iCoordIDMax, NameWithColorData iColor,
                                  unsigned int iCollectionID)
  {
    iTrace.SetField( "CoordIDMin", ConvertToString< unsigned int >(iCoordIDMin) );
    iTrace.SetField( "CoordIDMax", ConvertToString< unsigned int >(iCoordIDMax) );
    iTrace.SetColor(iColor.second.red(), iColor.second.green(),
                    iColor.second.blue(), iColor.second.alpha(), iColor.first,
                    iDatabaseConnector);

    iTrace.SetCollectionID(iCollectionID);
    return iTrace.SaveInDB(iDatabaseConnector);
  }

  /**
  \brief update the color of the specified trace with iNewColor in the database
  \param[in] iTraceID ID of the trace with the color to modify
  \param[in] iNewColor name and rgba values of the new color
  \param[in] iDatabaseConnector connection to the database
  \tparam T children of GoDBTraceRow
  */
  template< typename T >
  void ChangeColorForTrace(unsigned int iTraceID,
                           NameWithColorData iNewColor, vtkMySQLDatabase *iDatabaseConnector)
  {
    T tempTrace;

    tempTrace.SetValuesForSpecificID(iTraceID, iDatabaseConnector);
    tempTrace.SetColor(iNewColor.second.red(), iNewColor.second.green(),
                       iNewColor.second.blue(), iNewColor.second.alpha(),
                       iNewColor.first, iDatabaseConnector);
    tempTrace.SaveInDB(iDatabaseConnector);
  }

  /**
  \brief get the list of IDs belonging to the iListTraces as collectionof
  \param[in] iDatabaseConnector connection to the database
  \param[in] iListTraces list of traces IDs for which the collectionof are needed
  \return std::list<unsigned int> list of the tracesIDs
  */
  std::list< unsigned int > GetListTracesIDsFromThisCollectionOf(
    vtkMySQLDatabase *iDatabaseConnector, std::list< unsigned int > iListTraces);

  /**
 \brief get the list of IDs that are collection of iListTraces
 \param[in] iDatabaseConnector connection to the database
 \param[in] iListTracesIDs list of traces IDs for which we need the collectionIDs
 \return std::list<unsigned int> list of the tracesIDs
 */
  std::list< unsigned int > GetListCollectionIDs(
    vtkMySQLDatabase *iDatabaseConnector, std::list< unsigned int > iListTracesIDs);

  /**
  \brief get the list of tracesIDs that have no points
  \param[in] iListTracesIDs list of the tracesIDs to be checked
  \param[in] iDatabaseConnector connection to the database
  \return list of IDs for the traces that have no point and that are in iListTracesIDs
  */
  std::list< unsigned int > GetListTracesIDWithNoPoints(
    std::list< unsigned int > iListTracesIDs, vtkMySQLDatabase *iDatabaseConnector);

  /**
  \brief get the IDs of the last saved traces in the database
  \param[in] iDatabaseConnector connection to the database
  \param[in] iNumberOfTraces number of IDs to get
  \return list of last created IDs
  */
  std::list< unsigned int > GetLastCreatedTracesIDs(
    vtkMySQLDatabase *iDatabaseConnector, int iNumberOfTraces);

protected:

  std::string  m_CollectionName;
  std::string  m_CollectionIDName;
  std::string  m_TracesName;
  std::string  m_TracesIDName;
  std::string  m_CollectionOfName;
  std::string  m_CollectionOfIDName;
  unsigned int m_ImgSessionID;

  /** \brief Create a new collection Row in the collection table and
  return the collectionID from the created row: */
  int CreateNewCollection();

  /**
  \brief create a new collection in the database and return the corresponding ID
  */
  int CreateNewCollection(vtkMySQLDatabase *DatabaseConnector, GoDBTraceRow & myNewObject);

  //get from Database and/or Modif into Database
  /** \brief get the max of all the coordinates in the ListSelectedTraces, compare it
  to the max of the existing collection and update the max coordinate of the bounding
  box of the collection if necessary*/
  int GetCoordMaxID(vtkMySQLDatabase *DatabaseConnector,
                    int CollectionID, std::list< unsigned int > ListSelectedTraces);

  //get from Database and/or Modif into Database
  /** \brief get the min of all the coordinates in the ListSelectedTraces, compare it
  to the min of the existing collection and update the min coordinate of the bounding
  box of the collection if necessary*/
  int GetCoordMinID(vtkMySQLDatabase *DatabaseConnector,
                    int CollectionID, std::list< unsigned int > ListSelectedTraces);

  /** \brief Update in the database the coordid max and min of the trace*/
  void UpdateBoundingBoxInDB(int iCoordIDMin, int iCoordIDMax,
                             int iTraceID, vtkMySQLDatabase *iDatabaseConnector);

  /** \brief return the coordinate min for the existing Collection*/
  GoDBCoordinateRow GetExistingCoordMin(
    vtkMySQLDatabase *DatabaseConnector, int CollectionCoordIDMin,
    int CollectionID);

  /** \brief return the coordinate max for the existing Collection*/
  GoDBCoordinateRow GetExistingCoordMax(
    vtkMySQLDatabase *DatabaseConnector, int CollectionCoordIDMax,
    int CollectionID);

  //Modif into Database
  /** \brief change the collection ID of the trace*/
  void UpdateCollectionIDOfSelectedTrace(int iSelectedTraceID, int inewCollectionID,
                                         vtkMySQLDatabase *DatabaseConnector);

  //******************************Modif-Refactoring************************************************
  //protected:

  // get from Database and/or Modif into Database
  /**
  \brief modify the timepoint to iTimePoint for the coordmax and coordmin and
  replace the ioCoordIDMax/Min with the new saved coordinates in the database
  \param[in] iTimePoint TCoord to be replaced with for the coordmin and max
  \param[in,out] ioCoordIDMax in: ID for the coordinate max with the timepoint
  to be modified, out: ID of the coordinate max with the timepoint set as itimepoint
  \param[in,out] ioCoordIDMin in: ID for the coordinate min with the timepoint
  to be modified, out: ID of the coordinate min with the timepoint set as itimepoint
  \param[in] iDatabaseConnector connection to the database
  */
  void SetTheTimePointCoordinatesForMesh(unsigned int iTimePoint,
                                         int & ioCoordIDMax,
                                         int & ioCoordIDMin,
                                         vtkMySQLDatabase *iDatabaseConnector);

  /**
  \brief return the CoordIDMax for a minimum bounding box
  \param[in] iDatabaseConnector connection to the database
  \return the ID for the coordinate Max of the bounding box
  */
  int GetCoordIDMaxForBoundingBoxWithNoTraces(
    vtkMySQLDatabase *iDatabaseConnector);

  /**
  \brief return the CoordIDMin for a minimum bounding box
  \param[in] iDatabaseConnector connection to the database
  \return the ID for the coordinate min of the bounding box
  */
  int GetCoordIDMinForBoundingBoxWithNoTraces(
    vtkMySQLDatabase *iDatabaseConnector);

  // get from Database and/or Modif into Database
  /** \brief get the min of all the coordinates of the collectionof traces
  belonging to the trace and record them in the database*/
  int GetCoordMinID(vtkMySQLDatabase *DatabaseConnector, int iTraceID);

  //get from Database and/or Modif into Database
  /** \brief get the max of all the coordinates of  the collectionof
  traces belonging to the trace and record them in the database*/
  int GetCoordMaxID(vtkMySQLDatabase *DatabaseConnector, int iTraceID);

  /** \brief return the coordinate max of all the coordinates of the
  collectionOf traces*/
  GoDBCoordinateRow GetCollectionOfTracesCoordMax(
    vtkMySQLDatabase *DatabaseConnector, std::vector< std::string > iListCollectionOfTracesID);

  /** \brief return the coordinate min of all the coordinates of the
  selected traces*/
  GoDBCoordinateRow GetCollectionOfTracesCoordMin(
    vtkMySQLDatabase *DatabaseConnector, std::vector< std::string > iListCollectionOfTracesID);

  /**
  \brief get all the different parts needed for the query to get the color of traces
  from the database
  \param[in,out] ioSelectedFields will be filed with the attributes of the color DBtable
  \param[in,out] ioJoinTablesOnTraceTable will be filled with the conditions to link the
  color table and the trace table
  */
  void GetFieldsNeededForQueryForColorData(
    std::vector< std::string > & ioSelectedFields,
    std::vector< std::string > & ioJoinTablesOnTraceTable);

  /**
  \brief get the data from the query results to fill a QColor and the corresponding ID
  \param[in] iResultsQuery vector containing results from a query with a name/ID
  and the corresponding rgba
  \return std::list<NameWithColorData> list of the IDs with their corresponding QColor
  */
  std::list< NameWithColorData >
  GetListNameWithColorDataFromResultsQuery(
    std::vector< std::vector< std::string > > iResultsQuery);

  std::vector< std::string > ListUnsgIntToVectorString(std::list< unsigned int > iList);

  std::list< unsigned int > VectorStringToUnsgInt(std::vector< std::string > iVector);
};
#endif
