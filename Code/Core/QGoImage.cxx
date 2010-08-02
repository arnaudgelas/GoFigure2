/*=========================================================================
  Author: $Author:$  // Author of last commit
  Version: $Rev:$  // Revision of last commit
  Date: $Date:$  // Date of last commit
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

#include "QGoImage.h"

#include "vtkImageData.h"

//-------------------------------------------------------------------------
QGoImage::QGoImage() : m_VTKImages( 1, NULL )
{}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QGoImage::~QGoImage()
{}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
unsigned int 
QGoImage::
GetImageDimension() const
{
  int dim[3];
  m_VTKImages[0]->GetDimensions( dim );
  
  if( (dim[0] == 1) || (dim[1] == 1) || (dim[2] == 1) )
    {
    return 2;
    }
  else
    {
    return 3;
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::vector< unsigned int >
QGoImage::
GetSize() const
{
  unsigned int Dimension = this->GetImageDimension();
  std::vector< unsigned int > oSize( Dimension );
  
  int size[3];
  m_VTKImages[0]->GetDimensions( size );
  for( unsigned int i = 0; i < Dimension; i++ )
    {
    oSize[i] = static_cast< unsigned int >( size[i] );
    }
  return oSize;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::vector< double > 
QGoImage::
GetSpacing() const
{
  unsigned int Dimension = this->GetImageDimension();
  std::vector< double > oSpacing( Dimension );
  
  double spacing[3];
  m_VTKImages[0]->GetSpacing( spacing );
  for( unsigned int i = 0; i < Dimension; i++ )
    {
    oSpacing[i] = spacing[i];
    }
  return oSpacing;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::vector< double >
QGoImage::
GetOrigin() const
{
  unsigned int Dimension = this->GetImageDimension();
  std::vector< double > oOrg( Dimension );
  
  double origin[3];
  m_VTKImages[0]->GetOrigin( origin );
  for( unsigned int i = 0; i < Dimension; i++ )
    {
    oOrg[i] = origin[i];
    }
  return oOrg;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void 
QGoImage::
SetNthChannelVTKImage( const unsigned int& iId, vtkImageData* iImage )
{
  if( iId < m_VTKImages.size() )
    {
    m_VTKImages[iId] = iImage;
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void 
QGoImage::
SetVTKImages( const std::vector< vtkImageData* >& iImages )
{
  m_VTKImages = iImages;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
vtkImageData* 
QGoImage::
GetNthChannelVTKImage( const unsigned int& iId ) const
{
  if( iId < m_VTKImages.size() )
    {
    return m_VTKImages[iId];
    }
  else
    {
    return NULL;
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::vector< vtkImageData* > 
QGoImage::
GetVTKImages() const
{
  return m_VTKImages;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
size_t 
QGoImage::
GetNumberOfChannels() const
{
  return m_VTKImages.size();
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void 
QGoImage::
SetNumberOfChannels( const size_t& iN )
{
  m_VTKImages.resize( iN );
}
//-------------------------------------------------------------------------



