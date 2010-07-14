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
#ifndef __GoSynchronizedView3DCallbacks_h
#define __GoSynchronizedView3DCallbacks_h



#include "vtkCommand.h"
#include "QGoSynchronizedView3D.h"

class vtkObject;
class vtkCallbackCommand;

/**
\class QGoSynchronizedView3DCallbacks
\brief This object takes a list of QGoSynchronizedView and synchronize
 *  their cameras by setting up callbacks. it is recommended to let the Q
 *  GoSynchronizedViewManager deal
 *  with SynchronizedView synchronization.
 */

class QGoSynchronizedView3DCallbacks : public QObject
{
  // QT macro for signals and slots
  Q_OBJECT

public:
  /** \brief the constructor do most of the work :
   *  add observers & callbacks to QGoSynchronizedViews of the vector
   */
  QGoSynchronizedView3DCallbacks( std::vector<QGoSynchronizedView3D*>
                                    ioOpenSynchronizedViews,
                                  QObject*  iParent = 0);

  ~QGoSynchronizedView3DCallbacks();

  /** \brief callback function to synchornize cameras (has to be public)
   */
  static void synchronizeCameras0 ( vtkObject* caller, long unsigned int eventId,
                                   void* clientData, void* callData );

  static void synchronizeCameras1 ( vtkObject* caller, long unsigned int eventId,
                                   void* clientData, void* callData );

  static void synchronizeCameras2 ( vtkObject* caller, long unsigned int eventId,
                                   void* clientData, void* callData );

  static void synchronizeCameras3 ( vtkObject* caller, long unsigned int eventId,
                                   void* clientData, void* callData );

  /** \brief remove a Manager from the vector of synchronized Managers
   *  (this method takes care of removing the callback)
   */
  void removeSynchronizedView( QGoSynchronizedView3D* ioSynchronizedView );

  /** \brief add a Manager to the vector of synchronized Managers
   *  (this method takes care of adding the callback)
   */
  void addSynchronizedView( QGoSynchronizedView3D* ioSynchronizedView );

signals:
  /** \brief signals for synchronizing sliders (gives current XYslice
   *  of any changing Manager)
   */
  void SliceViewXYChanged( int oSlice);

  /** \brief signals for synchronizing sliders (gives current XZ slice
   *  of any changing Manager)
   */
  void SliceViewXZChanged( int oSlice);

  /** \brief signals for synchronizing sliders (gives current YZ slice
   *  of any changing Manager)
   */
  void SliceViewYZChanged( int oSlice);

private:
  /** \brief setup the callback command object
   * (client data, called function...etc)
   */
  void SetupCallBack();

  /** callback object to link callback function to master Manager's event
   */
  vtkCallbackCommand*            m_vtkCallBackCamSync0;
  vtkCallbackCommand*            m_vtkCallBackCamSync1;
  vtkCallbackCommand*            m_vtkCallBackCamSync2;
  vtkCallbackCommand*            m_vtkCallBackCamSync3;

  /** all open Managers are stored in this array,
   *  to transmit it to the callback function
   */
  std::vector<QGoSynchronizedView3D*>      m_openSynchronizedView;

};

#endif // QGoSynchronizedView3D3DSYNC_H
