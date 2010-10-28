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

#include "TrackContainer.h"

#include "vtkActor.h"

//-------------------------------------------------------------------------
TrackContainer::TrackContainer(QObject *iParent,
                               QGoImageView3D *iView):QObject(iParent),
  m_ImageView(iView),
  m_CurrentElement(),
  m_HighlightedProperty(NULL)
{}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
TrackContainer::
~TrackContainer()
{
  MultiIndexContainer::iterator it = m_Container.begin();

  while ( it != m_Container.end() )
    {
/*
    std::map<int, vtkPolyData*>::iterator mapIt;
    for ( mapIt=it->Nodes.begin() ; mapIt != it->Nodes.end(); mapIt++ )
      {
    it->Nodes.erase(mapIt);
      }
*/
    if ( it->ActorXY )
      {
      it->ActorXY->Delete();
      }
    if ( it->ActorXZ )
      {
      it->ActorXZ->Delete();
      }
    if ( it->ActorYZ )
      {
      it->ActorYZ->Delete();
      }
    if ( it->ActorXYZ )
      {
      it->ActorXYZ->Delete();
      }

    ++it;
    }
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
TrackContainer::Print()
{
  this->Print( m_Container.begin(), m_Container.end() );
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
TrackContainer::InsertCurrentElement()
{
  m_Container.insert(m_CurrentElement);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void TrackContainer::UpdateCurrentElementFromVisu(std::vector< vtkActor * > iActors,
                                                        vtkPolyData *iNodes,
                                                        const unsigned int & iT,
                                                        const bool & iHighlighted,
                                                        const bool & iVisible)
{
  if ( iActors.size() != 4 )
    {
    std::cerr << "iActors.size() != 4" << std::endl;
    return;
    }
  else
    {
    this->m_CurrentElement.ActorXY = iActors[0];
    this->m_CurrentElement.ActorXZ = iActors[1];
    this->m_CurrentElement.ActorYZ = iActors[2];
    this->m_CurrentElement.ActorXYZ = iActors[3];
    }

  // add a polydata in the map
  // unique time autorized yet
  //std::map<int, vtkPolyData*>::iterator it;
  //it = this->m_CurrentElement.Nodes.begin();

  //this->m_CurrentElement.Nodes[iT]=iNodes;//.insert(it, std::pair<int,vtkPolyData*>(iT,iNodes));

  this->m_CurrentElement.Highlighted = iHighlighted;
  this->m_CurrentElement.Visible = iVisible;

  if ( iHighlighted )
    {
    this->m_CurrentElement.ActorXY->SetProperty(this->m_HighlightedProperty);
    this->m_CurrentElement.ActorXZ->SetProperty(this->m_HighlightedProperty);
    this->m_CurrentElement.ActorYZ->SetProperty(this->m_HighlightedProperty);
    this->m_CurrentElement.ActorXYZ->SetProperty(this->m_HighlightedProperty);
    }
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void TrackContainer::UpdateCurrentElementFromDB(unsigned int iTraceID,
                                                      double irgba[4])
{
  this->m_CurrentElement.TraceID = iTraceID;
  this->m_CurrentElement.rgba[0] = irgba[0];
  this->m_CurrentElement.rgba[1] = irgba[1];
  this->m_CurrentElement.rgba[2] = irgba[2];
  this->m_CurrentElement.rgba[3] = irgba[3];
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
TrackContainer::Insert(const TrackStructure & iE)
{
  m_Container.insert(iE);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
TrackContainer::ResetCurrentElement()
{
  m_CurrentElement = TrackStructure();
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
TrackContainer::ShowActorsWithGivenTimePoint(const unsigned int & iT)
{
  /*
  if ( iT != m_TCoordMinMin )
    {
    MultiIndexContainerTCoordMinMinIterator it, it0, it1;

    boost::tuples::tie(it0, it1) = m_Container.get< TCoordMinMin >().equal_range(iT);

    ChangeActorsVisibility< TCoordMinMin >(m_Container.get< TCoordMinMin >().begin(), it0, false);
    ChangeActorsVisibility< TCoordMinMin >(it0, it1, true);
    ChangeActorsVisibility< TCoordMinMin >(it1, m_Container.get< TCoordMinMin >().end(), false);

    m_TCoordMin = iT;
    }*/
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
TrackContainer::AddActorsWithGivenTimePoint(const unsigned int & iT)
{
  /*
  MultiIndexContainerTCoordMinIterator it0, it1;

  boost::tuples::tie(it0, it1) = m_Container.get< TCoordMin >().equal_range(iT);

  ChangeActorsVisibility< TCoordMin >(it0, it1, true);

  m_ImageView->UpdateRenderWindows();
  */
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool
TrackContainer::RemoveElementFromVisualizationWithGivenTraceID(
  const unsigned int & iId)
{
  MultiIndexContainerTraceIDIterator
    it = m_Container.get< TraceID >().find(iId);

  if ( it != m_Container.get< TraceID >().end() )
    {
    if ( it->ActorXY )
      {
      this->m_ImageView->RemoveActor(0, it->ActorXY);
      }
    if ( it->ActorXZ )
      {
      this->m_ImageView->RemoveActor(1, it->ActorXZ);
      }
    if ( it->ActorYZ )
      {
      this->m_ImageView->RemoveActor(2, it->ActorYZ);
      }
    if ( it->ActorXYZ )
      {
      this->m_ImageView->RemoveActor(3, it->ActorXYZ);
      }

    this->m_CurrentElement = *it;

    m_Container.get< TraceID >().erase(it);

    m_ImageView->UpdateRenderWindows();

    return true;
    }
  return false;
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool
TrackContainer::UpdateElementHighlightingWithGivenTraceID(const unsigned int & iId)
{
  MultiIndexContainerTraceIDIterator
    it = m_Container.get< TraceID >().find(iId);

  vtkProperty *temp_property = NULL;

  if ( it != m_Container.get< TraceID >().end() )
    {
    if ( it->Highlighted )
      {
      temp_property = vtkProperty::New();
      temp_property->SetColor(it->rgba[0],
                              it->rgba[1],
                              it->rgba[2]);
      temp_property->SetOpacity(it->rgba[3]);
      }
    else
      {
      temp_property = this->m_HighlightedProperty;
      }

    if ( it->ActorXY )
      {
      it->ActorXY->SetProperty(temp_property);
      }
    if ( it->ActorXZ )
      {
      it->ActorXZ->SetProperty(temp_property);
      }
    if ( it->ActorYZ )
      {
      it->ActorYZ->SetProperty(temp_property);
      }
    if ( it->ActorXYZ )
      {
      it->ActorXYZ->SetProperty(temp_property);
      }

    if ( it->Highlighted )
      {
      temp_property->Delete();
      }

    TrackStructure tempStructure(*it);
    tempStructure.Highlighted = !it->Highlighted;

    m_Container.get< TraceID >().replace(it, tempStructure);
    m_ImageView->UpdateRenderWindows();

    return true;
    }

  return false;
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool
TrackContainer::UpdateElementVisibilityWithGivenTraceID(const unsigned int & iId)
{
  MultiIndexContainerTraceIDIterator
    it = m_Container.get< TraceID >().find(iId);

  typedef void ( QGoImageView3D::*ImageViewMember )(const int &, vtkActor *);
  ImageViewMember f;

  if ( it != m_Container.get< TraceID >().end() )
    {
    if ( it->Visible )
      {
      f = &QGoImageView3D::RemoveActor;
      }
    else
      {
      f = &QGoImageView3D::AddActor;
      }

    if ( it->ActorXY )
      {
      ( m_ImageView->*f )(0, it->ActorXY);
      }
    if ( it->ActorXZ )
      {
      ( m_ImageView->*f )(1, it->ActorXZ);
      }
    if ( it->ActorYZ )
      {
      ( m_ImageView->*f )(2, it->ActorYZ);
      }
    if ( it->ActorXYZ )
      {
      ( m_ImageView->*f )(3, it->ActorXYZ);
      }

    if ( it->ActorXY )
      {
      it->ActorXY->SetVisibility(!it->Visible);
      }
    if ( it->ActorXZ )
      {
      it->ActorXZ->SetVisibility(!it->Visible);
      }
    if ( it->ActorYZ )
      {
      it->ActorYZ->SetVisibility(!it->Visible);
      }
    if ( it->ActorXYZ )
      {
      it->ActorXYZ->SetVisibility(!it->Visible);
      }

    TrackStructure tempStructure(*it);
    tempStructure.Visible = !it->Visible;

    m_Container.get< TraceID >().replace(it, tempStructure);

    m_ImageView->UpdateRenderWindows();
    return true;
    }

  return false;
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool
TrackContainer::DeleteElement(const unsigned int & iId)
{
  MultiIndexContainerTraceIDIterator
    it = m_Container.get< TraceID >().find(iId);

  if ( it != m_Container.get< TraceID >().end() )
    {
    if ( it->ActorXY )
      {
      this->m_ImageView->RemoveActor(0, it->ActorXY);
      it->ActorXY->Delete();
      }
    if ( it->ActorXZ )
      {
      this->m_ImageView->RemoveActor(1, it->ActorXZ);
      it->ActorXZ->Delete();
      }
    if ( it->ActorYZ )
      {
      this->m_ImageView->RemoveActor(2, it->ActorYZ);
      it->ActorYZ->Delete();
      }
    if ( it->ActorXYZ )
      {
      this->m_ImageView->RemoveActor(3, it->ActorXYZ);
      it->ActorXYZ->Delete();
      }

    /*
    if ( it->Nodes )
      {
      it->Nodes->Delete();
      }
      */

    m_Container.get< TraceID >().erase(it);

    m_ImageView->UpdateRenderWindows();
    return true;
    }

  return false;
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::list< unsigned int >
TrackContainer::DeleteAllHighlightedElements()
{
  MultiIndexContainerHighlightedIterator it0, it1, it_t;

  boost::tuples::tie(it0, it1) =
    m_Container.get< Highlighted >().equal_range(true);

  std::list< unsigned int > oList;

  while ( it0 != it1 )
    {
    oList.push_back(it0->TraceID);

    if ( it0->ActorXY )
      {
      this->m_ImageView->RemoveActor(0, it0->ActorXY);
      it0->ActorXY->Delete();
      }
    if ( it0->ActorXZ )
      {
      this->m_ImageView->RemoveActor(1, it0->ActorXZ);
      it0->ActorXZ->Delete();
      }
    if ( it0->ActorYZ )
      {
      this->m_ImageView->RemoveActor(2, it0->ActorYZ);
      it0->ActorYZ->Delete();
      }
    if ( it0->ActorXYZ )
      {
      this->m_ImageView->RemoveActor(3, it0->ActorXYZ);
      it0->ActorXYZ->Delete();
      }
    /*
    if ( it0->Nodes )
      {
      it0->Nodes->Delete();
      }
      */

    it_t = it0;
    ++it0;

    m_Container.get< Highlighted >().erase(it_t);
    }

  m_ImageView->UpdateRenderWindows();

  return oList;
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::list< unsigned int >
TrackContainer::GetHighlightedElementsTraceID()
{
  MultiIndexContainerHighlightedIterator it0, it1;

  boost::tuples::tie(it0, it1) =
    m_Container.get< Highlighted >().equal_range(true);

  std::list< unsigned int > oList;
  while ( it0 != it1 )
    {
    oList.push_back(it0->TraceID);
    ++it0;
    }
  return oList;
}

//-------------------------------------------------------------------------
std::list< unsigned int >
TrackContainer::UpdateAllHighlightedElementsWithGivenColor(QColor iColor)
{
  MultiIndexContainerHighlightedIterator it0, it1;

  boost::tuples::tie(it0, it1) =
    m_Container.get< Highlighted >().equal_range(true);

  double r(1.), g(1.), b(1.), a(1.);
  if ( iColor.isValid() )
    {
    iColor.getRgbF(&r, &g, &b, &a);
    }

  std::list< unsigned int > oList;
  while ( it0 != it1 )
    {
    TrackStructure temp(*it0);
    temp.rgba[0] = r;
    temp.rgba[1] = g;
    temp.rgba[2] = b;
    temp.rgba[3] = a;

    m_Container.get< Highlighted >().replace(it0, temp);

    oList.push_back(it0->TraceID);

    ++it0;
    }

  return oList;
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
TrackContainer::SetHighlightedProperty(vtkProperty *iProperty)
{
  this->m_HighlightedProperty = iProperty;

  MultiIndexContainerHighlightedIterator it0, it1;
  boost::tuples::tie(it0, it1) =
    m_Container.get< Highlighted >().equal_range(true);

  while ( it0 != it1 )
    {
    if ( it0->Highlighted )
      {
      if ( it0->ActorXY )
        {
        it0->ActorXY->SetProperty(this->m_HighlightedProperty);
        }
      if ( it0->ActorXZ )
        {
        it0->ActorXZ->SetProperty(this->m_HighlightedProperty);
        }
      if ( it0->ActorYZ )
        {
        it0->ActorYZ->SetProperty(this->m_HighlightedProperty);
        }
      if ( it0->ActorXYZ )
        {
        it0->ActorXYZ->SetProperty(this->m_HighlightedProperty);
        }
      }

    ++it0;
    }
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
vtkProperty *
TrackContainer::GetHighlightedProperty()
{
  return m_HighlightedProperty;
}

//-------------------------------------------------------------------------