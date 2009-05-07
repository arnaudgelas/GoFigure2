/*=========================================================================
 Authors: The GoFigure Dev. Team.
 while at Megason Lab, Systems biology, Harvard Medical school, 2009

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


#ifndef __QGoImageInfo_h
#define __QGoImageInfo_h

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QWidget>

#include "MegaVTK2Configure.h"

class QGoImageInfo : public QWidget
{
  Q_OBJECT
public:
  explicit QMEGAVTKADDON2_EXPORT QGoImageInfo( QWidget* parent = 0 );
  virtual QMEGAVTKADDON2_EXPORT  ~QGoImageInfo( );

public slots:
  QMEGAVTKADDON2_EXPORT void setDimension( const unsigned int& );
  QMEGAVTKADDON2_EXPORT void setNumberOfChannels( const unsigned int& );
  QMEGAVTKADDON2_EXPORT void setSize( const std::vector< unsigned int >& );
  QMEGAVTKADDON2_EXPORT void setSpacing( const std::vector< float >& );
  QMEGAVTKADDON2_EXPORT void setPixelPosition( const std::vector< unsigned int >& );
  QMEGAVTKADDON2_EXPORT void setWorldPosision( const std::vector< float >& );
  QMEGAVTKADDON2_EXPORT void setTimePoint( const float& );
  QMEGAVTKADDON2_EXPORT void setTimeSpacing( const float& );
  QMEGAVTKADDON2_EXPORT void setValue( const std::vector< float >& );


protected:
QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QLabel *TImageStaticlabel;
    QLabel *DimensionStaticLabel;
    QLabel *DimensionLabel;
    QLabel *SizeStaticLabel;
    QLabel *SizeLabel;
    QLabel *SpacingLabel;
    QLabel *MemoryLabel;
    QLabel *PixelPosLabel;
    QLabel *WorldPositionLabel;
    QLabel *TimeLabel;
    QLabel *SpacingStaticLabel;
    QLabel *MemoryStaticLabel;
    QLabel *PositionLabel;
    QLabel *PixelPosStaticLabel;
    QLabel *WorldPosStaticLabel;
    QLabel *ChannelsStaticLabel;
    QLabel *TTimeStaticLabel;
    QLabel *TValueStaticLabel;
    QLabel *ValueStaticLabel;
    QLabel *ValueLabel;
    QLabel *ChannelsLabel;
    QLabel *TimeSpacingStaticLabel;
    QLabel *TimeSpacingLabel;
    QLabel *TimeStaticLabel;

  unsigned int m_Dimension;
  unsigned int m_Channel;
  std::vector< unsigned int > m_Size;
  std::vector< float >  m_Spacing;
  std::vector< unsigned int > m_PPos;
  std::vector< float > m_WPos;
  std::vector< float > m_Value;
  float m_TimeSpacing;
  float m_TimePoint;

  template< class TContainer >
  QString ConvertToQString( const TContainer& iVector )
  {
    if( iVector.empty() )
      return QString();
    else
    {
      size_t size = iVector.size();

      if( size == 1 )
      {
        return QString( "%1" ).arg( *iVector.begin() );
      }
      else
      {
        QString v( "[ ");
        unsigned int i = 0;
        for( typename TContainer::const_iterator it = iVector.begin();
             it != iVector.end();
             ++it, ++i )
        {
          if( i == ( size - 1 ) )
            v.append( QString( "%1 ]" ).arg( *it ) );
          else
            v.append( QString( "%1 ; " ).arg( *it ) );
        }
        return v;
      }
    }
  }

  void setupUi( QWidget* Form );
  void retranslateUi(QWidget* Form);
private:
  QGoImageInfo( const QGoImageInfo& );
  void operator = ( const QGoImageInfo& );
};

#endif