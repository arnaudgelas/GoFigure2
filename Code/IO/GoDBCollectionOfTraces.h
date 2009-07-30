/*=========================================================================
  Author: $Author$  // Author of last commit
  Version: $Revision$  // Revision of last commit
  Date: $Date$  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009

 Copyright (c) 2009, President and Fellows of Harvard College.
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

#include <QString>
#include <QObject>
#include "MegaVTK2Configure.h"
#include "GoDBRecordSetHelper.h"


class GoDBCollectionOfTraces
{

public:

  explicit QMEGAVTKADDON2_EXPORT GoDBCollectionOfTraces();
  explicit QMEGAVTKADDON2_EXPORT GoDBCollectionOfTraces (QString CollectionName,
    QString CollectionIDName, QString Traces,QString TracesIDName);
  virtual QMEGAVTKADDON2_EXPORT ~GoDBCollectionOfTraces();
  void QMEGAVTKADDON2_EXPORT SetDatabaseVariables(
    QString Server,QString User,QString Password, QString NameDB);

  /** \brief Delete in the Database all the traces listed in the QStringList */
  void QMEGAVTKADDON2_EXPORT DeleteTraces(QStringList TracesToDelete);

  template< class myT >
  void QMEGAVTKADDON2_EXPORT CreateNewCollectionFromSelection(QStringList ListSelectedTraces)
  {
  int NewCollectionID = this->CreateNewCollection<myT>();
  AddSelectedTracesToCollection(ListSelectedTraces,NewCollectionID);
  }

  QString CollectionName()
    { return m_CollectionName;}
  QString TracesName()
    { return m_TracesName;}

protected:

  QString m_Server;
  QString m_User;
  QString m_Password;
  QString m_NameDB;
  QString m_CollectionName;
  QString m_CollectionIDName;
  QString m_TracesName;
  QString m_TracesIDName;

  /** \bried Update the collectionID of the selected traces in the DB traces table
  with the new collectionID: */
  void AddSelectedTracesToCollection(QStringList ListSelectedTraces,int newCollectionID);

  /** \brief Create a new collection Row in the collection table and return the 
  collectionID from the created row: */
  int  CreateNewCollection();
 

  template< class myT >
  int CreateNewCollection()
  {
  myT myNewObject;  
  AddNewObjectInTable< myT >(
      m_Server.toStdString(),
      m_User.toStdString(),
      m_Password.toStdString(),
      m_NameDB.toStdString(), 
      m_CollectionName.toStdString(), myNewObject );  
  
  int ID = MaxValueForOneColumnInTable(
    m_Server.toStdString(), m_User.toStdString(),
    m_Password.toStdString(),m_NameDB.toStdString(),
    m_CollectionIDName.toStdString(),m_CollectionName.toStdString()); 

  return ID; 
  }
};
#endif
