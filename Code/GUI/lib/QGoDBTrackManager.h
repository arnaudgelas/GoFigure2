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

#ifndef __QGoDBTrackManager_h
#define __QGoDBTrackManager_h

#include "QGoTableWidget.h"
#include "GoDBCollectionOfTraces.h"
#include "GoDBTWContainerForTrackLineage.h"
#include "QGoDBTraceManager.h"
#include "QGoGUILibConfigure.h"

//#include <QWidget>

class QGOGUILIB_EXPORT QGoDBTrackManager:public QGoDBTraceManager
{
  Q_OBJECT
public:
  QGoDBTrackManager(int iImgSessionID, QWidget *iparent);
  ~QGoDBTrackManager();

  virtual void DisplayInfoForLastCreatedTrace(vtkMySQLDatabase *iDatabaseConnector);

  virtual void DisplayInfoForExistingTrace(vtkMySQLDatabase *iDatabaseConnector,
                                           int iTraceID);

  void DisplayInfoAndLoadVisuContainerForAllTracks(
    vtkMySQLDatabase *iDatabaseConnector);

  /**
  \brief create a new track with no mesh and no points in the database, add it in the
  TW and in the visu container
  */
  unsigned int CreateNewTrackWithNoMesh(
    vtkMySQLDatabase *iDatabaseConnector, NameWithColorData iColor);

  /**
  \brief virtual pure method in QGoDBTraceManager
  */
  std::list< unsigned int > UpdateTheTracesColor(vtkMySQLDatabase *iDatabaseConnector,
                                                 NameWithColorData iNewColor);

  /**
  \brief virtual pure method in QGoDBTraceManager
  */
  virtual void UpdateTWAndContainerForImportedTraces(std::vector< int > iVectorImportedTraces,
                                                     vtkMySQLDatabase *iDatabaseConnector);

protected:
  GoDBTWContainerForTrackLineage *m_TWContainer;

  virtual void SetCollectionsTraceNames();

  virtual void DisplayInfoForAllTraces(vtkMySQLDatabase *iDatabaseConnector);
};
#endif
