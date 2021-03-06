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
#ifndef __ContourToMeshFilter_txx
#define __ContourToMeshFilter_txx

#include "vtkAppendPolyData.h"
#include "vtkMath.h"
#include "vtkSmartPointer.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkNormalEstimationFilter.h"
#include "vtkPoissonReconstruction.h"
#include "vtkPolyDataWriter.h"
#include "vtkFillHolesFilter.h"
#include "vtkPolylineDecimation.h"

#include <iostream>

namespace itk
{
/**
 * \class ContourToMeshFilter
 * \brief
 */
template< class TContainer >
ContourToMeshFilter< TContainer >::
ContourToMeshFilter():
  m_Output(NULL),
  m_ThresholdNumberOfPoints( 20 ),
  m_TargetNumberOfPoints( 20 )
{}

template< class TContainer >
ContourToMeshFilter< TContainer >::~ContourToMeshFilter()
{}

template< class TContainer >
void
ContourToMeshFilter< TContainer >::ProcessContours(const ContainerType & iContainer)
{
  if ( iContainer.empty() )
    {
    std::cerr << "error, no contours!" << std::endl;
    return;
    }
  else
    {
    vtkSmartPointer< vtkAppendPolyData > append =
      vtkSmartPointer< vtkAppendPolyData >::New();

    // compute center of mass
    ContainerConstIterator it = iContainer.begin();

    std::vector< vtkSmartPointer< vtkPolylineDecimation > >
      decimate( iContainer.size() );

    size_t i = 0;

    while ( it != iContainer.end() )
      {
      if ( ( *it ) )
        {
        if( (*it)->GetNumberOfPoints() > m_ThresholdNumberOfPoints )
          {
          decimate[i] = vtkSmartPointer< vtkPolylineDecimation >::New();
          decimate[i]->SetInput( *it );
          decimate[i]->SetTargetReduction( 1. -
            static_cast< double >( m_TargetNumberOfPoints ) /
            static_cast< double >( (*it)->GetNumberOfPoints() ) );
          decimate[i]->Update();

          append->AddInput( decimate[i]->GetOutput() );
          }
        else
          {
          append->AddInput(*it);
          }
        }
      ++it;
      ++i;
      }

    vtkSmartPointer< vtkNormalEstimationFilter > normal_filter =
      vtkSmartPointer< vtkNormalEstimationFilter >::New();
    normal_filter->SetInput( append->GetOutput() );
    normal_filter->Update();

    // run the Poisson Reconstruction
    vtkSmartPointer< vtkPoissonReconstruction > poissonFilter =
      vtkSmartPointer< vtkPoissonReconstruction >::New();
    poissonFilter->SetInput( normal_filter->GetOutput() );
    poissonFilter->SetDepth(12);
    poissonFilter->SetConfidence(1.);
    poissonFilter->Update();

    vtkSmartPointer< vtkFillHolesFilter > fillFilter =
      vtkSmartPointer< vtkFillHolesFilter >::New();
    fillFilter->SetInput( poissonFilter->GetOutput() );
    fillFilter->Update();

    if ( !m_Output )
      {
      m_Output = vtkPolyData::New();
      }
    m_Output->ShallowCopy( fillFilter->GetOutput() );

/*
    double bounds[6];
    m_Output->GetBounds(bounds);

    vtkSmartPointer< vtkPolyDataWriter > writer =
      vtkSmartPointer< vtkPolyDataWriter >::New();
    writer->SetInput( m_Output );
    writer->SetFileName( "mesh.vtk" );
    writer->Write();
*/
    }
}

template< class TContainer >
vtkPolyData *
ContourToMeshFilter< TContainer >::GetOutput()
{
  return m_Output;
}
}
#endif
