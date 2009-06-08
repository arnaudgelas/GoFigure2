/*=========================================================================
  Author: $Author$  // Author of last commit
  Version: $Rev$  // Revision of last commit
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

#include "vtkPolyDataMySQLTextReader.h"

#include "vtkObjectFactory.h"
#include "vtkCellArray.h"

#include <sstream>

vtkCxxRevisionMacro(vtkPolyDataMYSQLTextReader, "$Revision$");
vtkStandardNewMacro(vtkPolyDataMYSQLTextReader);


vtkPolyDataMYSQLTextReader::vtkPolyDataMYSQLTextReader() :
  m_Text(""), IsContour( true )
{}

vtkPolyDataMYSQLTextReader::~vtkPolyDataMYSQLTextReader()
{}

vtkPolyData* vtkPolyDataMYSQLTextReader::GetPolyData( std::string iString )
{
  if( IsContour )
    {
    return GetContour();
    }
  else
    {
    return GetMesh();
    }
}


vtkPolyData* vtkPolyDataMYSQLTextReader::GetContour( )
{
  vtkPolyData* oContour = vtkPolyData::New();
  vtkPoints* points = vtkPoints::New();

  std::stringstream str( m_Text );

  vtkIdType N;
  str >>N;
  points->SetNumberOfPoints( N );

  double pt[3];

  for( vtkIdType i = 0; i < N; i++ )
    {
    str >>pt[0] >>pt[1] >>pt[2];
    points->SetPoint( i, pt );
    }
  oContour->SetPoints( points );

  vtkCellArray* cells = vtkCellArray::New();
  vtkIdList* id_list = vtkIdList::New();
  id_list->SetNumberOfIds(2);
  for( vtkIdType i = 0; i < N-1; i++ )
    {
    id_list->SetId( 0, i );
    id_list->SetId( 1, i+1 );
    cells->InsertNextCell( id_list );
    }
  oContour->SetPolys( cells );
  return oContour;
}

vtkPolyData* vtkPolyDataMYSQLTextReader::GetMesh( )
{
  vtkPolyData* oMesh = vtkPolyData::New();
  vtkPoints* points = vtkPoints::New();

  std::stringstream str( m_Text );

  vtkIdType N;
  str >>N;
  points->SetNumberOfPoints( N );

  double pt[3];

  for( vtkIdType i = 0; i < N; i++ )
    {
    str >>pt[0] >>pt[1] >>pt[2];
    points->SetPoint( i, pt );
    }
  oMesh->SetPoints( points );

  vtkCellArray* cells = vtkCellArray::New();
  str >>N;

  vtkIdList* cell_points = vtkIdList::New();
  vtkIdType NbOfPointsInCell;
  vtkIdType id;

  for( vtkIdType i = 0; i < N; i++ )
    {
    str >>NbOfPointsInCell;
    cell_points->Reset();
    for( vtkIdType k = 0; k < NbOfPointsInCell; k++ )
      {
      str >>id;
      cell_points->InsertNextId( id );
      }
    cells->InsertNextCell( cell_points );
    }
  oMesh->SetPolys( cells );
  return oMesh;
}