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

#include "QGoImageView3D.h"

#include "vtkConstrainedPointHandleRepresentation.h"
#include "vtkSeedWidget.h"
#include "vtkImageActorPointPlacer.h"

#include "vtkImageData.h"
#include "vtkViewImage2D.h"
#include "vtkViewImage3D.h"
#include "vtkViewImage2DCollection.h"

#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"
#include "vtkRenderer.h"
#include "vtkTextProperty.h"
#include "vtkProperty.h"
#include "vtkImageClip.h"
#include "vtkImagePermute.h"
#include "vtkImageResample.h"
#include "vtkWindowToImageFilter.h"
#include "vtkBMPWriter.h"
#include "vtkPostScriptWriter.h"
#include "vtkJPEGWriter.h"
#include "vtkPNGWriter.h"
#include "vtkTIFFWriter.h"
#include "vtkCamera.h"

#include "vtkEventQtSlotConnect.h"
#include "QSplitterChild.h"
#include "QVTKWidget.h"

#include <QResizeEvent>
#include <QSettings>

#include "vtkViewImage2DCommand.h"
#include "vtkViewImage2DCollectionCommand.h"
#include "vtkViewImage3DCommand.h"
#include "vtkImageActorPointPlacer.h"

#include "vtkCellArray.h"
#include "vtkMath.h"
#include "vtkPolyData.h"

#include "vtkWidgetEvent.h"
#include "vtkWidgetEventTranslator.h"

#include <cstdlib>

//-------------------------------------------------------------------------
QGoImageView3D::
QGoImageView3D(QWidget* iParent) :
  QGoImageView(iParent),
  IsFullScreen(0),
  m_FirstRender(true),
  m_Initialized(false),
  m_ShowAnnotations(true),
  m_ShowSplinePlane(true),
  m_ShowCube(true)
  {
  VtkEventQtConnector = vtkEventQtSlotConnect::New();

  m_HighlightedContourProperty = vtkProperty::New();
  m_HighlightedContourProperty->SetColor(1., 0., 0.);
  m_HighlightedContourProperty->SetLineWidth(3.);

  setupUi(this);

  QObject::connect(this->SliderXY, SIGNAL(valueChanged(int)),
                   this, SLOT(SetSliceViewXY(int)));
  QObject::connect(this->SliderXZ, SIGNAL(valueChanged(int)),
                   this, SLOT(SetSliceViewXZ(int)));
  QObject::connect(this->SliderYZ, SIGNAL(valueChanged(int)),
                   this, SLOT(SetSliceViewYZ(int)));

  QObject::connect(this->HtSplitter, SIGNAL(splitterMoved(int, int)),
                   this->HbSplitter, SLOT(moveSplitter(int, int)));
  QObject::connect(this->HbSplitter, SIGNAL(splitterMoved(int, int)),
                   this->HtSplitter, SLOT(moveSplitter(int, int)));

  vtkViewImage2D* View1 = vtkViewImage2D::New();
  SetupViewGivenQVTKWidget(View1, this->QvtkWidget_XY);

  this->m_Pool->AddItem(View1);
  View1->Delete();

  vtkViewImage2D* View2 = vtkViewImage2D::New();
  SetupViewGivenQVTKWidget(View2, this->QvtkWidget_XZ);

  this->m_Pool->AddItem(View2);
  View2->Delete();

  vtkViewImage2D* View3 = vtkViewImage2D::New();
  SetupViewGivenQVTKWidget(View3, this->QvtkWidget_YZ);

  this->m_Pool->AddItem(View3);
  View3->Delete();

  vtkRenderWindow* renwin4 = this->QvtkWidget_XYZ->GetRenderWindow();
  this->m_View3D = vtkViewImage3D::New();
  this->m_View3D->SetRenderWindow(renwin4);
  this->m_View3D->SetupInteractor(this->QvtkWidget_XYZ->GetInteractor());

  this->m_Pool->SetExtraRenderWindow(renwin4);

  InitializeSeedWidgetInteraction();
  }
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QGoImageView3D::~QGoImageView3D()
  {
  delete HtSplitter;
  delete HbSplitter;

  std::vector<vtkSeedWidget*>::iterator seedWidgetIterator = SeedWidget.begin();
  while (seedWidgetIterator != SeedWidget.end())
    {
    (*seedWidgetIterator)->Delete();
    ++seedWidgetIterator;
    }

  std::vector<vtkConstrainedPointHandleRepresentation*>::iterator
  handleIterator = Handle.begin();
  while (handleIterator != Handle.end())
    {
    (*handleIterator)->Delete();
    ++handleIterator;
    }

  std::vector<vtkSeedRepresentation*>::iterator
  seedIterator = SeedRep.begin();
  while (seedIterator != SeedRep.end())
    {
    (*seedIterator)->Delete();
    ++seedIterator;
    }

  // note m_Pool is supposed to be deleted in QGoImageView, but due to a bug
  // it has to be deleted in this order...
  if (m_Pool)
    {
    m_Pool->Delete();
    m_Pool = NULL;
    }
  m_View3D->Delete();
  m_View3D = NULL;

  VtkEventQtConnector->Delete();
  m_HighlightedContourProperty->Delete();
  }
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::setupUi(QWidget* iParent)
{
  if (iParent->objectName().isEmpty())
    {
    iParent->resize(800, 800);
    }

  QList<int> list_size;
  list_size.push_back(400);
  list_size.push_back(400);

  this->VSplitter  = new QSplitter(Qt::Vertical, iParent);

  this->HtSplitter = new QSplitterChild(this->VSplitter);
  this->HbSplitter = new QSplitterChild(this->VSplitter);

  this->VSplitter->addWidget(this->HtSplitter);
  this->VSplitter->addWidget(this->HbSplitter);
  this->VSplitter->setSizes(list_size);
  this->VSplitter->resize(800, 800);

  this->QvtkWidget_XY = new QVTKWidget;
  this->SliderXY = new QSlider(Qt::Vertical);
  this->LayOut1 = new QHBoxLayout;
  this->LayOut1->addWidget(this->QvtkWidget_XY);
  this->LayOut1->addWidget(this->SliderXY);
  this->LayOutWidget1 = new QWidget;
  this->LayOutWidget1->setLayout(this->LayOut1);
  this->HtSplitter->addWidget(this->LayOutWidget1);

  this->QvtkWidget_XZ = new QVTKWidget;
  this->SliderXZ = new QSlider(Qt::Vertical);
  this->LayOut2 = new QHBoxLayout;
  this->LayOut2->addWidget(this->QvtkWidget_XZ);
  this->LayOut2->addWidget(this->SliderXZ);
  this->LayOutWidget2 = new QWidget;
  this->LayOutWidget2->setLayout(this->LayOut2);
  this->HbSplitter->addWidget(this->LayOutWidget2);

  this->QvtkWidget_YZ = new QVTKWidget;
  this->SliderYZ = new QSlider(Qt::Vertical);
  this->LayOut3 = new QHBoxLayout;
  this->LayOut3->addWidget(this->QvtkWidget_YZ);
  this->LayOut3->addWidget(this->SliderYZ);
  this->LayOutWidget3 = new QWidget;
  this->LayOutWidget3->setLayout(this->LayOut3);
  this->HtSplitter->addWidget(this->LayOutWidget3);

  this->QvtkWidget_XYZ = new QVTKWidget;
  this->LayOut4 = new QHBoxLayout;
  this->LayOut4->addWidget(this->QvtkWidget_XYZ);
  this->LayOut4->addSpacing(27);
  this->LayOutWidget4 = new QWidget;
  this->LayOutWidget4->setLayout(this->LayOut4);
  this->HbSplitter->addWidget(this->LayOutWidget4);

  this->HtSplitter->setSizes(list_size);
  this->HtSplitter->resize(800, 400);

  this->HbSplitter->setSizes(list_size);
  this->HbSplitter->resize(800, 400);

  retranslateUi(iParent);

  QMetaObject::connectSlotsByName(iParent);
} // setupUi
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::retranslateUi(QWidget *iParent)
{
  iParent->setWindowTitle(tr("QGoImageView3D"));
  Q_UNUSED(iParent);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::Update()
{
  vtkViewImage2D* View1 = this->m_Pool->GetItem(0);
  View1->SetInput(this->m_Image);
  View1->SetViewOrientation(vtkViewImage2D::VIEW_ORIENTATION_AXIAL);
  View1->SetViewConvention(vtkViewImage2D::VIEW_CONVENTION_NEUROLOGICAL);
  View1->UpdateWindowLevelObservers();

  this->m_View3D->Add2DPhantom(0,
                               View1->GetImageActor(), View1->GetSlicePlane());

  int *range = View1->GetSliceRange();

  this->SliderXY->setMinimum(range[0]);
  this->SliderXY->setMaximum(range[1]);

  vtkViewImage2D* View2 = this->m_Pool->GetItem(1);
  View2->SetInput(this->m_Image);
  View2->SetViewConvention(vtkViewImage2D::VIEW_CONVENTION_NEUROLOGICAL);
  View2->SetViewOrientation (vtkViewImage2D::VIEW_ORIENTATION_CORONAL);
  View2->UpdateWindowLevelObservers();

  this->m_View3D->Add2DPhantom(1,
                               View2->GetImageActor(), View2->GetSlicePlane());

  range = View2->GetSliceRange();

  this->SliderXZ->setMinimum(range[0]);
  this->SliderXZ->setMaximum(range[1]);

  vtkViewImage2D* View3 = this->m_Pool->GetItem(2);
  View3->SetInput(this->m_Image);
  View3->SetViewConvention(vtkViewImage2D::VIEW_CONVENTION_NEUROLOGICAL);
  View3->SetViewOrientation(vtkViewImage2D::VIEW_ORIENTATION_SAGITTAL);
  View3->UpdateWindowLevelObservers();

  this->m_View3D->Add2DPhantom(
    2, View3->GetImageActor(), View3->GetSlicePlane());

  range = View3->GetSliceRange();
  this->SliderYZ->setMinimum(range[0]);
  this->SliderYZ->setMaximum(range[1]);

  this->m_View3D->SetInput(this->m_Image);
  this->m_View3D->SetVolumeRenderingOff();
  this->m_View3D->SetTriPlanarRenderingOn();
  this->m_View3D->SetShowScalarBar(false);
  this->m_View3D->ResetCamera();

  this->m_Pool->SyncSetBackground(this->m_Pool->GetItem(0)->GetBackground());
  this->m_Pool->SyncSetShowAnnotations(m_ShowAnnotations);
  this->m_Pool->SetSplinePlaneActorsVisibility(m_ShowSplinePlane);
  this->m_View3D->SetBoundsActorsVisibility(m_ShowSplinePlane);
  this->m_View3D->SetCubeVisibility(m_ShowCube);

  for (int i = 0; i < 3; i++)
    {
    this->m_Pool->GetItem(i)->GetTextProperty()->SetFontFamilyToArial();
    this->m_Pool->GetItem(i)->GetTextProperty()->SetFontSize(14);
    }

  this->m_Pool->UpdateWindowLevelObservers();
  this->m_Pool->SyncSetShowScalarBar(false);
  this->m_Pool->SyncRender();

  if (m_FirstRender)
    {
    this->m_Pool->SyncReset();
    this->m_Pool->InitializeAllObservers();
    this->m_Pool->Initialize();

    this->SliderXY->setValue((this->SliderXY->minimum() + this->SliderXY->maximum()) / 2);
    this->SliderXZ->setValue((this->SliderXZ->minimum() + this->SliderXZ->maximum()) / 2);
    this->SliderYZ->setValue((this->SliderYZ->minimum() + this->SliderYZ->maximum()) / 2);

    // Rotate the camera to show that the view is 3d
    vtkCamera *camera = this->m_View3D->GetRenderer()->GetActiveCamera();
    camera->Roll(-135);
    camera->Azimuth(-45);

    this->m_View3D->GetRenderer()->SetActiveCamera(camera);
    this->m_View3D->ResetCamera();

    SetupVTKtoQtConnections();

    m_FirstRender = false;
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::SetupVTKtoQtConnections()
{
  vtkViewImage2D* View1  = this->m_Pool->GetItem(0);
  vtkViewImage2D* View2  = this->m_Pool->GetItem(1);
  vtkViewImage2D* View3  = this->m_Pool->GetItem(2);
  vtkViewImage3D* View3D = this->m_View3D;

  // Event connection between vtk and qt
  // when RequestedPositionEvent occurs in the XY View (double-click),
  // SliderXZ and SliderYZ move.
  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View1->GetInteractorStyle()),
    vtkViewImage2DCommand::RequestedPositionEvent,
    this, SLOT(MoveSliderXZ()));

  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View1->GetInteractorStyle()),
    vtkViewImage2DCommand::RequestedPositionEvent,
    this, SLOT(MoveSliderYZ()));

  // Event connection between vtk and qt
  // when RequestedPositionEvent occurs in the XY View (double-click),
  // SliderXZ and SliderYZ move.
  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View2->GetInteractorStyle()),
    vtkViewImage2DCommand::RequestedPositionEvent,
    this, SLOT(MoveSliderXY()));

  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View2->GetInteractorStyle()),
    vtkViewImage2DCommand::RequestedPositionEvent,
    this, SLOT(MoveSliderYZ()));

  // Event connection between vtk and qt
  // when SliceMoveEvent occurs in the XY View, SliderXY moves.
  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View3->GetInteractorStyle()),
    vtkViewImage2DCommand::EndSliceMoveEvent,
    this, SLOT(MoveSliderYZ()));

  // Event connection between vtk and qt
  // when RequestedPositionEvent occurs in the XY View (double-click),
  // SliderXZ and SliderYZ move.
  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View3->GetInteractorStyle()),
    vtkViewImage2DCommand::RequestedPositionEvent,
    this, SLOT(MoveSliderXY()));

  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View3->GetInteractorStyle()),
    vtkViewImage2DCommand::RequestedPositionEvent,
    this, SLOT(MoveSliderXZ()));

  // Event connection between vtk and qt
  // when SliceMoveEvent occurs in the XY View, SliderXY moves.
  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View2->GetInteractorStyle()),
    vtkViewImage2DCommand::EndSliceMoveEvent,
    this, SLOT(MoveSliderXZ()));

  // Event connection between vtk and qt
  // when SliceMoveEvent occurs in the XY View, SliderXY moves.
  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View1->GetInteractorStyle()),
    vtkViewImage2DCommand::EndSliceMoveEvent,
    this, SLOT(MoveSliderXY()));

  // Event connection between vtk and qt
  // when contours picked, send a signal
  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View1->GetInteractorStyle()),
    vtkViewImage2DCommand::ContourPickingEvent,
    this, SIGNAL(ContoursSelectionChanged()));

  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View2->GetInteractorStyle()),
    vtkViewImage2DCommand::ContourPickingEvent,
    this, SIGNAL(ContoursSelectionChanged()));

  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View3->GetInteractorStyle()),
    vtkViewImage2DCommand::ContourPickingEvent,
    this, SIGNAL(ContoursSelectionChanged()));

  // Event connection between vtk and qt
  // when contours picked, send a signal
  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View3D->GetInteractorStyle3D()),
    vtkViewImage3DCommand::ReadyEvent,
    this, SIGNAL(MeshesSelectionChanged()));

  // Event connection between vtk and qt
  // when contours picked, send a signal
  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View3D->GetCommand()->GetBoxWidget()),
    vtkViewImage3DCommand::BoxWidgetReadyEvent,
    this, SIGNAL(MeshesSelectionChanged()));

  // Event connection between vtk and qt
  // when contours picked, send a signal
  VtkEventQtConnector->Connect(
    reinterpret_cast<vtkObject*>(View1->GetInteractorStyle()),
    vtkViewImage2DCommand::WindowLevelEvent,
    this, SLOT(UpdateScalarBarIn3DWiew()));
}

//-------------------------------------------------------------------------
void
QGoImageView3D::
InitializeSeedWidgetInteraction()
{
  int N = this->m_Pool->GetNumberOfItems();

  // Enable seed interaction
  this->Handle.resize(N);
  this->SeedRep.resize(N);
  this->SeedWidget.resize(N);

  for (int i = 0; i < N; ++i)
    {
    this->Handle[i] = vtkConstrainedPointHandleRepresentation::New();
    this->Handle[i]->GetProperty()->SetColor(1, 0, 0);

    this->SeedRep[i] = vtkSeedRepresentation::New();
    this->SeedRep[i]->SetHandleRepresentation(this->Handle[i]);

    this->SeedWidget[i] = vtkSeedWidget::New();
    this->SeedWidget[i]->SetRepresentation(this->SeedRep[i]);

    this->SeedWidget[i]->SetInteractor(
      this->m_Pool->GetItem(i)->GetInteractor());

    // to remove right click interaction in the one click widget
    this->SeedWidget[i]->GetEventTranslator()->RemoveTranslation(
      vtkCommand::RightButtonPressEvent);
    }
}

//-------------------------------------------------------------------------
void QGoImageView3D::SetImage(vtkImageData* input)
{
  if (!input)
    {
    return;
    }
  else
    {
    int dim[3];
    input->GetDimensions(dim);

    if (dim[0] + dim[1] + dim[2] > 0)
      {
      m_Initialized = true;
      this->m_Image = input;
      }
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
vtkImageData* QGoImageView3D::GetImage()
{
  return m_Image;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
vtkImageActor* QGoImageView3D::GetImageActor(const int& iId)
{
  if ((iId < 0) || (iId > 2))
    {
    return NULL;
    }
  else
    {
    vtkViewImage2D* View = m_Pool->GetItem(iId);
    return View->GetImageActor();
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QVTKInteractor* QGoImageView3D::GetInteractor(const int& iId)
{

  if ((iId < 0) || (iId > 3))
    {
    return NULL;
    }
  else
    {
    switch (iId)
      {
      default:
      case 0:
        {
        return this->QvtkWidget_XY->GetInteractor();
        }
      case 1:
        {
        return this->QvtkWidget_XZ->GetInteractor();
        }
      case 2:
        {
        return this->QvtkWidget_YZ->GetInteractor();
        }
      case 3:
        {
        return this->QvtkWidget_XYZ->GetInteractor();
        }
      }
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QString QGoImageView3D::SnapshotViewXY(const GoFigure::FileType& iType,
                                       const QString& iBaseName)
{
  QString filename = SnapshotView(QvtkWidget_XY, iType,
                                  iBaseName, m_SnapshotId);

  m_SnapshotId++;
  return filename;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QString QGoImageView3D::SnapshotView2(const GoFigure::FileType& iType,
                                      const QString& iBaseName)
{
  QString filename = SnapshotView(QvtkWidget_XZ, iType,
                                  iBaseName, m_SnapshotId);

  m_SnapshotId++;
  return filename;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QString QGoImageView3D::SnapshotView3(
  const GoFigure::FileType& iType,
  const QString& iBaseName)
{
  QString filename = SnapshotView(QvtkWidget_YZ, iType,
                                  iBaseName, m_SnapshotId);

  m_SnapshotId++;
  return filename;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QString QGoImageView3D::SnapshotViewXYZ(
  const GoFigure::FileType& iType,
  const QString& iBaseName)
{
  QString filename = SnapshotView(QvtkWidget_XYZ, iType,
                                  iBaseName, m_SnapshotId);

  m_SnapshotId++;
  return filename;
}
//------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::SetFullScreenView(const int& iS)
{
  if (IsFullScreen == iS)
    {
    IsFullScreen = 0;
    }
  else
    {
    IsFullScreen = iS;
    }

  switch (IsFullScreen)
    {
    default:
    case 0:
      {
      Quadview();
      break;
      }
    case 1:
      {
      FullScreenViewXY();
      break;
      }
    case 2:
      {
      FullScreenViewXZ();
      break;
      }
    case 3:
      {
      FullScreenViewYZ();
      break;
      }
    case 4:
      {
      FullScreenViewXYZ();
      break;
      }
    }
  emit FullScreenViewChanged(IsFullScreen);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::Quadview()
{
  IsFullScreen = 0;
  LayOutWidget1->show();
  LayOutWidget2->show();
  LayOutWidget3->show();
  LayOutWidget4->show();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::FullScreenViewXY()
{
  IsFullScreen = 1;
  LayOutWidget1->show();
  LayOutWidget2->hide();
  LayOutWidget3->hide();
  LayOutWidget4->hide();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::FullScreenViewXZ()
{
  IsFullScreen = 2;
  LayOutWidget1->hide();
  LayOutWidget2->show();
  LayOutWidget3->hide();
  LayOutWidget4->hide();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::FullScreenViewYZ()
{
  IsFullScreen = 3;
  LayOutWidget1->hide();
  LayOutWidget2->hide();
  LayOutWidget3->show();
  LayOutWidget4->hide();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::FullScreenViewXYZ()
{
  IsFullScreen = 4;
  LayOutWidget1->hide();
  LayOutWidget2->hide();
  LayOutWidget3->hide();
  LayOutWidget4->show();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
int QGoImageView3D::GetFullScreenView() const
{
  return IsFullScreen;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::resizeEvent(QResizeEvent* iEvent)
{
  QWidget::resizeEvent(iEvent);
  VSplitter->resize(iEvent->size());
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::SetSliceViewXY(const int& iSlice)
{
  if (m_Initialized)
    {
    int s = GetSliceViewXY();

    if (iSlice != s)
      {
      this->m_Pool->GetItem(0)->SetSlice(iSlice);
      this->m_Pool->SyncRender();
      emit SliceViewXYChanged(iSlice);
      }
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
int QGoImageView3D::GetSliceViewXY() const
{
  return this->m_Pool->GetItem(0)->GetSlice();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::SetSliceViewXZ(const int& iSlice)
{
  if (m_Initialized)
    {
    int s = GetSliceViewXZ();

    if (s != iSlice)
      {
      this->m_Pool->GetItem(1)->SetSlice(iSlice);
      this->m_Pool->SyncRender();
      emit SliceViewXZChanged(iSlice);
      }
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
int QGoImageView3D::GetSliceViewXZ() const
{
  return this->m_Pool->GetItem(1)->GetSlice();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::SetSliceViewYZ(const int& iSlice)
{
  if (m_Initialized)
    {
    int s = GetSliceViewYZ();

    if (s != iSlice)
      {
      this->m_Pool->GetItem(2)->SetSlice(iSlice);
      this->m_Pool->SyncRender();
      emit SliceViewYZChanged(iSlice);
      }
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
int QGoImageView3D::GetSliceViewYZ() const
{
  return this->m_Pool->GetItem(2)->GetSlice();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::MoveSliderXY()
{
  if (m_Initialized)
    {
    int s = GetSliceViewXY();

    if (s != this->SliderXY->value())
      {
      this->SliderXY->setValue(s);
      emit SliceViewXYChanged(s);
      }
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::MoveSliderXZ()
{
  if (m_Initialized)
    {
    int s = GetSliceViewXZ();

    if (s != this->SliderXZ->value())
      {
      this->SliderXZ->setValue(s);
      emit SliceViewXZChanged(s);
      }
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::MoveSliderYZ()
{
  if (m_Initialized)
    {
    int s = GetSliceViewYZ();

    if (s != this->SliderYZ->value())
      {
      this->SliderYZ->setValue(s);
      emit SliceViewYZChanged(s);
      }
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoImageView3D::SaveStateSplitters()
{
  QSettings settings;
  settings.setValue("VSplitterSizes", VSplitter->saveState());
  settings.setValue("HtSplitterSizes", HtSplitter->saveState());
  settings.setValue("HbSplitterSizes", HbSplitter->saveState());
}
//-------------------------------------------------------------------------

//--------------------------------------------------------------------------
vtkViewImage3D*
QGoImageView3D::
GetImageViewer3D()
{
  return m_View3D;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// std::vector< vtkQuadricLODActor* >
std::vector<vtkActor*>
QGoImageView3D::
AddContour(const int& iId, vtkPolyData* dataset, vtkProperty* iProperty)
{
//   std::vector< vtkQuadricLODActor* > oList =
  std::vector<vtkActor*> oList =
    QGoImageView::AddContour(iId, dataset, iProperty);

//   vtkQuadricLODActor* temp =
  vtkActor* temp = m_View3D->AddDataSet((vtkDataSet*) dataset,
                                        iProperty, false, false);

  m_View3D->Render();
  oList.push_back(temp);

//   std::vector< vtkQuadricLODActor* >::iterator list_it = oList.begin();
//   while( list_it != oList.end() )
//     {
//     m_ActorsPropertyMap[*list_it] = iProperty;
//     ++list_it;
//     }

  return oList;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoImageView3D::
ChangeActorProperty(vtkProp3D* iActor, vtkProperty* iProperty)
{
  m_View3D->ChangeActorProperty(iActor, iProperty);
  QGoImageView::ChangeActorProperty(iActor, iProperty);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoImageView3D::
ChangeActorProperty(int iDir, vtkProp3D* iActor, vtkProperty* iProperty)
{
  vtkViewImage2D* viewer = NULL;

  if ((iDir >= 0) && (iDir < m_Pool->GetNumberOfItems()))
    {
    viewer = m_Pool->GetItem(iDir);
    viewer->ChangeActorProperty(iActor, iProperty);
    }
  else
    {
    if (iDir == 3)
      {
      m_View3D->ChangeActorProperty(iActor, iProperty);
      }
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoImageView3D::
RemoveActor(const int& iId, vtkActor* iActor)
{
  if (iId == 3)
    {
    m_View3D->GetRenderer()->RemoveActor(iActor);
    }
  else
    {
    QGoImageView::RemoveActor(iId, iActor);
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoImageView3D::
AddActor(const int& iId, vtkActor* iActor)
{
  if (iId == 3)
    {
    m_View3D->GetRenderer()->AddActor(iActor);
    }
  else
    {
    QGoImageView::AddActor(iId, iActor);
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoImageView3D::
SetLookupTable(vtkLookupTable* iLut)
{
  if (this->m_Image->GetNumberOfScalarComponents() == 1)
    {
    m_Pool->SyncSetLookupTable(iLut);
    m_Pool->SyncRender();
    m_View3D->SetLookupTable(iLut);
    m_View3D->Render();
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
void
QGoImageView3D::
ShowScalarBar(const bool& iShow)
{
  if (this->m_Image->GetNumberOfScalarComponents() == 1)
    {
    m_Pool->SyncSetShowScalarBar(iShow);
    m_Pool->SyncRender();
    m_View3D->SetShowScalarBar(iShow);
    m_View3D->Render();
    }
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------

/**
 * \todo use dynamic_cast or more appropriate cast operator
 */
std::vector<vtkActor*>
QGoImageView3D::
AddMesh(const int& iId, vtkPolyData* dataset, vtkProperty* iProperty)
{
  std::vector<vtkActor*> oList =
    QGoImageView::AddContour(iId, dataset, iProperty);

  vtkActor* temp = m_View3D->AddDataSet((vtkDataSet*) dataset,
                                        iProperty, false, false);

  m_View3D->Render();
  oList.push_back(temp);

  return oList;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::list<vtkProp3D*>
QGoImageView3D::
GetListOfPickedActors()
{
  vtkViewImage2DCollectionCommand* command = m_Pool->GetCommand();
  return command->GetListOfPickedActors();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
std::list<vtkProp3D*>
QGoImageView3D::
GetListOfUnPickedActors()
{
  vtkViewImage2DCollectionCommand* command = m_Pool->GetCommand();
  return command->GetListOfUnPickedActors();
}
//--------------------------------------------------------------------------

void
QGoImageView3D::
ShowAnnotations()
{
  if (m_ShowAnnotations)
    {
    // remove annotations in 2d views
    m_ShowAnnotations = false;
    this->m_Pool->SyncSetShowAnnotations(m_ShowAnnotations);
    }
  else
    {
    m_ShowAnnotations = true;
    this->m_Pool->SyncSetShowAnnotations(m_ShowAnnotations);
    }

  this->UpdateRenderWindows();
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------

void
QGoImageView3D::
ShowSplinePlane()
{
  if (m_ShowSplinePlane)
    {
    m_ShowSplinePlane = false;
    this->m_Pool->SetSplinePlaneActorsVisibility(m_ShowSplinePlane);
    this->m_View3D->SetBoundsActorsVisibility(m_ShowSplinePlane);
    }
  else
    {
    m_ShowSplinePlane = true;
    this->m_Pool->SetSplinePlaneActorsVisibility(m_ShowSplinePlane);
    this->m_View3D->SetBoundsActorsVisibility(m_ShowSplinePlane);
    }

  this->UpdateRenderWindows();
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------

void
QGoImageView3D::
ShowCube3D()
{
  if (m_ShowCube)
    {
    m_ShowCube = false;
    this->m_View3D->SetCubeVisibility(m_ShowCube);
    }
  else
    {
    m_ShowCube = true;
    this->m_View3D->SetCubeVisibility(m_ShowCube);
    }

  this->UpdateRenderWindows();
}

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------

void
QGoImageView3D::
UpdateRenderWindows()
{
  // Update Visualization
  // Update() not used because just want to update the renderWindow)
  vtkRenderWindow* ren;

  for (int i = 0; i < this->m_Pool->GetNumberOfItems(); ++i)
    {
    ren = this->m_Pool->GetItem(i)->GetRenderWindow();
    ren->Render();
    }

  ren = this->m_View3D->GetRenderWindow();
  ren->Render();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoImageView3D::
SetCamera(int iView)
{
  //Strange behaviour....

  vtkCamera *camera = vtkCamera::New();
  this->m_View3D->GetRenderer()->SetActiveCamera(camera);

  // Dorsal view
  camera->Roll(180);

  // Posterior view
  if (iView == 1)
    {
    camera->SetRoll(0);
    camera->Elevation(90);
    }

  // Left view
  if (iView == 3)
    {
    camera->Azimuth(-90);
    }

  this->m_View3D->ResetCamera();
  this->m_View3D->Render();
  camera->Delete();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoImageView3D::
DefaultMode()
{
  EnableOneClickMode(false);
  EnableContourPickingMode(false);
  EnableMeshPickingMode(false);

  //Change cursor
  QvtkWidget_XY->setCursor(Qt::ArrowCursor);
  QvtkWidget_XZ->setCursor(Qt::ArrowCursor);
  QvtkWidget_YZ->setCursor(Qt::ArrowCursor);
  QvtkWidget_XYZ->setCursor(Qt::ArrowCursor);

  for (int i = 0; i < this->m_Pool->GetNumberOfItems(); ++i)
    {
    vtkViewImage2D* View = this->m_Pool->GetItem(i);
    View->SetDefaultInteractionStyle();
    }

  vtkInteractorStyleImage3D* t = m_View3D->GetInteractorStyle3D();
  if (t)
    {
    t->EnableDefaultMode();
    }
}
//-------------------------------------------------------------------------

void
QGoImageView3D::
ZoomMode()
{
  DefaultMode();

  //Change cursors
  QCursor zoomCursor(QPixmap(QString::fromUtf8(":/fig/zoom.png")), -1, -1);
  this->QvtkWidget_XY->setCursor(zoomCursor);
  this->QvtkWidget_XZ->setCursor(zoomCursor);
  this->QvtkWidget_YZ->setCursor(zoomCursor);
  this->QvtkWidget_XYZ->setCursor(zoomCursor);

  for (int i = 0; i < this->m_Pool->GetNumberOfItems(); ++i)
    {
    vtkViewImage2D* View = this->m_Pool->GetItem(i);
    View->SetInteractionStyle(vtkInteractorStyleImage2D::InteractionTypeZoom);
    }

  vtkInteractorStyleImage3D* t = m_View3D->GetInteractorStyle3D();
  if (t)
    {
    t->EnableZoomMode();
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoImageView3D::
PanMode()
{
  DefaultMode();

  //Change cursor
  QvtkWidget_XY->setCursor(Qt::OpenHandCursor);
  QvtkWidget_XZ->setCursor(Qt::OpenHandCursor);
  QvtkWidget_YZ->setCursor(Qt::OpenHandCursor);
  QvtkWidget_XYZ->setCursor(Qt::OpenHandCursor);

  for (int i = 0; i < this->m_Pool->GetNumberOfItems(); ++i)
    {
    vtkViewImage2D* View = this->m_Pool->GetItem(i);
    View->SetInteractionStyle(vtkInteractorStyleImage2D::InteractionTypePan);
    }

  vtkInteractorStyleImage3D* t = m_View3D->GetInteractorStyle3D();
  if (t)
    {
    t->EnablePanMode();
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoImageView3D::
OneClickMode()
{
  //Reinitialize cursor interaction
  DefaultMode();
  // Enable widget in each slice
  EnableOneClickMode(true);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoImageView3D::
EnableOneClickMode(bool iEnable)
{
  for (int i = 0; i < this->m_Pool->GetNumberOfItems(); i++)
    {
    SeedWidget[i]->SetEnabled(iEnable);
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
vtkPoints*
QGoImageView3D::
GetAllSeeds()
{
  /// TODO Correct it earlier might solve the visualization problem
  /// regarding the seeds

  double worldPosition[3];

  /// TODO MEMORY LEAK HERE
  vtkPoints* oPoints = vtkPoints::New();

  for (unsigned int i = 0; i < this->SeedWidget.size(); i++)
    {
    int N = this->SeedRep[i]->GetNumberOfSeeds();
    for (int j = 0; j < N; j++)
      {
      // Get World position (may be not accurate if we are between 8 pixels (3D))
      this->SeedRep[i]->GetSeedWorldPosition(j, worldPosition);
      // Get indexes of the closest point
      int* index = this->m_Pool->GetItem(i)->GetImageCoordinatesFromWorldCoordinates(worldPosition);
      // Convert it back into world position
      double spacing[3];
      this->m_Pool->GetItem(i)->GetInput()->GetSpacing(spacing);
      double correctedPosition[3];
      correctedPosition[0] = static_cast<double>(index[0]) * spacing[0];
      correctedPosition[1] = static_cast<double>(index[1]) * spacing[1];
      correctedPosition[2] = static_cast<double>(index[2]) * spacing[2];
      oPoints->InsertNextPoint(correctedPosition);
      }
    }

  return oPoints;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoImageView3D::
ClearAllSeeds()
{
  for (unsigned int i = 0; i < this->SeedWidget.size(); i++)
    {
    for (int k = this->SeedRep[i]->GetNumberOfSeeds() - 1; k >= 0; --k)
      {
      this->SeedWidget[i]->DeleteSeed(k);
      this->SeedRep[i]->RemoveLastHandle();
      }
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoImageView3D::
MeshPickingMode()
{
  DefaultMode();

  EnableMeshPickingMode(true);
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoImageView3D::
ContourPickingMode()
{
  DefaultMode();

  EnableContourPickingMode(true);
}
//-------------------------------------------------------------------------
void
QGoImageView3D::
EnableContourPickingMode(bool iEnable)
{
  /// \todo move this code in the Collection code
  int N = this->m_Pool->GetNumberOfItems();

  for (int i = 0; i < N; ++i)
    {
    vtkViewImage2D* View = this->m_Pool->GetItem(i);
    View->SetInteractionStyle(
      vtkInteractorStyleImage2D::InteractionTypeContourPicking);
    View->SetWheelInteractionStyle(
      vtkInteractorStyleImage2D::InteractionTypeSlice);
    vtkInteractorStyleImage2D* t =
      vtkInteractorStyleImage2D::SafeDownCast(View->GetInteractorStyle());
    if (t)
      {
      if (iEnable)
        {
        t->StartPick();
        }
      else
        {
        t->EndPick();
        }
      }
    }
}

//-------------------------------------------------------------------------
std::list<vtkProp3D*>
QGoImageView3D::
GetListOfPickedContours()
{
  // Get picked contours from all views
  return m_Pool->GetCommand()->GetListOfPickedActors();
}

//-------------------------------------------------------------------------
std::list<vtkProp3D*>
QGoImageView3D::
GetListOfUnPickedContours()
{
  return m_Pool->GetCommand()->GetListOfUnPickedActors();
}

//-------------------------------------------------------------------------
void
QGoImageView3D::
EnableMeshPickingMode(bool iEnable)
{
  DefaultMode();

  vtkInteractorStyleImage3D* t = m_View3D->GetInteractorStyle3D();
  if (t)
    {
    if (iEnable)
      {
      t->EnablePickMode();
      t->StartPick();
      }
    else
      {
      t->DisablePickMode();
      t->EndPick();
      }
    }
}

//-------------------------------------------------------------------------
std::list<vtkProp3D*>
QGoImageView3D::
GetListOfModifiedActors3D()
{
  return m_View3D->GetCommand()->GetListOfModifiedActors();
}

//-------------------------------------------------------------------------
void
QGoImageView3D::
SetBox3DPicking(bool iValue)
{
  DefaultMode();
  m_View3D->GetCommand()->Enable3DBoxWidget(iValue);
}

//-------------------------------------------------------------------------
void
QGoImageView3D::
ResetWindowLevel()
{
  m_Pool->SyncResetWindowLevel();
}

//-------------------------------------------------------------------------
void
QGoImageView3D::
UpdateScalarBarIn3DWiew()
{
  m_Pool->GetItem(0)->GetLookupTable();
  m_View3D->SetLookupTable(m_Pool->GetItem(0)->GetLookupTable());
}
