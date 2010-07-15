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
#ifndef __QGoSynchronizedView_h
#define __QGoSynchronizedView_h
/*
#include "itkImageToVTKImageFilter.h"
#include "itkSmartPointer.h"
#include "itkImage.h"
*/
#include "SnapshotHelper.h"
#include "vtkSmartPointer.h"
#include "ui_QGoSynchronizedView.h"

#include "itkImage.h"
class vtkCamera;
class vtkImageData;
class vtkEventQtSlotConnect;
class QGoImageView;
class QGoSynchronizedViewManager;

/**
\class QGoSynchronizedView
\brief Abstract class for the (synchronized) view of one vtkImageData*.
*/
class QGoSynchronizedView : public QWidget,
  protected Ui::QGoSynchronizedView
  {
  Q_OBJECT

public:
  explicit QGoSynchronizedView(QString iViewName, QWidget *iParent = 0);

  /** \brief Destructor.
   */
  ~QGoSynchronizedView();

  /** \brief Set image displayed by the SynchronizedView
   */
  virtual void SetImage(vtkImageData* iImage) = 0;

  /** \brief returns the type of SynchronizedView (2 for 2D, 3 for 3D)
   */
  virtual int GetSynchronizedViewType(void) = 0;

  /** \brief get SynchronizedView's name
   */
  QString* GetName(void);

  /** \brief Update the viewer contained in the widget
   */
  virtual void Update(void);

  /** \brief render the viewer contained in the widget if any
   */
  void Render(void);

  /** \brief get the camera of the current viewer
   */
  vtkCamera* GetCamera(void);

  /** \brief print the SynchronizedView information :
   *  it consists in the image information if any.
   */
  virtual void PrintOs(ostream& os) = 0;

  /** \brief true if the widget has a viewer
   */
  bool HasViewer(void);

  /** \brief Set the address of the current QGoSynchronizedViewManager
   */
  void SetCurrentViewManager(QGoSynchronizedViewManager* iCurrentViewManager);

public slots:
  /** \brief Save a snapshot of the displaid view, in a iType file
   */
  virtual QString SnapshotViewXY(const GoFigure::FileType& iType,
                                 const QString& iBaseName = tr("Snapshot")) = 0;

protected:
  /** \brief Qt change event function
   */
  void changeEvent(QEvent *e);

  QString                     m_currentViewName;
  vtkImageData*               m_currentImage;
  QString                     m_currentImageName;
  QGoImageView*               m_currentView;
  QGoSynchronizedViewManager* m_currentViewManager;
  vtkEventQtSlotConnect*      m_VTKEventQtConnector;

private slots:
  // the QMetaObject::connectSlotsByName(SynchronizedView)
  // is provided in the Setupui generated by moc

private:
  /** delete the viewer contained in the widget
   */
  void deleteViewer(void);

  /** create the viewer contained in the widget
   */
  virtual void createViewer(void) = 0;
  };

#endif //__QGoSynchronizedView_h
