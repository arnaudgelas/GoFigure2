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
#include "QGoSynchronizedView2D.h"

#include "QGoSynchronizedView2DCallbacks.h"
#include "vtkImageData.h"
#include "vtkViewImage2D.h"
#include "vtkEventQtSlotConnect.h"

#include "SnapshotHelper.h"
#include "QGoImageView2D.h"
#include "QGoSynchronizedViewManager.h"



//--------------------------------------------------------------------------
/*
 *  Default Constructor.
 * \param iSynchronizedViewName
 * \param iParent
 */
QGoSynchronizedView2D::
QGoSynchronizedView2D( QString iViewName, QWidget *iParent ) :
  QGoSynchronizedView( iViewName,iParent )
{
}

//--------------------------------------------------------------------------
/*  Print self informations */
void
QGoSynchronizedView2D::
PrintOs(ostream &os)
{
  // if we have an imageview, the we print its image information
  if (m_currentImage != NULL)
    {
    os << "SynchronizedView 2D " << this << " contains :" << std::endl;
    m_currentImage->Print(os);
    }
  else
    {
    os << "SynchronizedView 2D " << this << " contains no Image :"<< std::endl;
    }
}


//--------------------------------------------------------------------------
QGoSynchronizedView2D::
  ~QGoSynchronizedView2D()
{
  // remove the comparer from the orchestra
  if (m_currentViewManager != NULL)
    {
    m_currentViewManager->removeSynchronizedView2D(this);
    m_currentViewManager = NULL;
    }
}


//--------------------------------------------------------------------------
/*  returns the type of comparer (2 for 2D, 3 for 3D) */
int
QGoSynchronizedView2D::
GetSynchronizedViewType()
{
  return 2;
}


//--------------------------------------------------------------------------
/* set the image to be displaid */
void
QGoSynchronizedView2D::
SetImage(vtkImageData* iImage)
{
  if (iImage == NULL)
    {
    return;
    }
  else
    {
    // if there is no viewer, we create one
    if (static_cast<QGoImageView2D*>(m_currentView) == NULL)
      {
      createViewer();
      }
    // set the image to the current view
    static_cast<QGoImageView2D*>(m_currentView)->SetImage(iImage);
    // update current image
    m_currentImage = iImage;

    this->Update();
    }
}



/*
  Private
*/


//--------------------------------------------------------------------------
/*  create the viewer contained in the widget */
void
QGoSynchronizedView2D::
createViewer()
{
  // if there is already a viewer
  if (static_cast<QGoImageView2D*>(m_currentView) != NULL)
    {
    return;
    }
  else
    {
    // else we create one
    m_currentView = new QGoImageView2D(this);
    static_cast<QGoImageView2D*>
      (m_currentView)->setContentsMargins( 1, 1, 1, 1 );
    // setup position of the widget
    this->gridLayout->addWidget(static_cast<QGoImageView2D*>(m_currentView));
    }
}


//--------------------------------------------------------------------------
QGoImageView2D*
QGoSynchronizedView2D::
GetImageView()
{
  if ( HasViewer() )
    {
    return static_cast<QGoImageView2D*>(m_currentView);
    }
  else
    {
    return NULL;
    }
}


//--------------------------------------------------------------------------
QString
QGoSynchronizedView2D::
SnapshotViewXY( const GoFigure::FileType& iType,
  const QString& iBaseName)
{
  if (!HasViewer())
    {
    return tr("");
    }
  else
    {
    return GetImageView()->SnapshotViewXY( iType, iBaseName );
    }
}
