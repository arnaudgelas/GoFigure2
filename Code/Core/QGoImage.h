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

#ifndef __QGoImage_h
#define __QGoImage_h

#include <vector>

#include "itkLightObject.h"
#include "itkObjectFactory.h"

class vtkImageData;

/**
\class QGoImage
\brief Multichannel 3D Image Representation in GoFigure2
*/
class QGoImage : public itk::LightObject
{
public:
  typedef QGoImage Self;
  typedef itk::SmartPointer< Self > Pointer;

  itkNewMacro( Self );

  void SetNthChannelVTKImage( const unsigned int& iId, vtkImageData* iImage );
  void SetVTKImages( const std::vector< vtkImageData* >& iImages );
  
  vtkImageData* GetNthChannelVTKImage( const unsigned int& iId ) const;
  std::vector< vtkImageData* > GetVTKImages() const;

  void SetNumberOfChannels( const size_t& iSize );
  size_t GetNumberOfChannels() const;
  
  unsigned int GetImageDimension() const;
  std::vector< unsigned int > GetSize() const;
  std::vector< double > GetSpacing() const;
  std::vector< double > GetOrigin() const;
  
protected:
  QGoImage();
  ~QGoImage();

  //--- QUESTION ---
  // std::vector< vtkImageData* > or 
  // std::map< std::string, vtkImageData* > ???
  std::vector< vtkImageData* > m_VTKImages;
  
private:
  void operator = ( const Self& );
  QGoImage( const Self& );
};
#endif

