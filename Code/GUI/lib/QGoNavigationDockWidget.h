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

#ifndef __QGoNavigationDockWidget_h
#define __QGoNavigationDockWidget_h

#include <QDockWidget>
#include <QComboBox>
#include "ui_NavigationDockWidget.h"
#include "GoFigureGlobalDefinition.h"

#include "QGoGUILibConfigure.h"

class QHBoxLayout;
class QVBoxLayout;

/** \class QGoNavigationDockWidget
 *  \brief Dock Widget for browsing images (changing slice, time point...)
 *  \ingroup GUI
 */
class QGOGUILIB_EXPORT QGoNavigationDockWidget:
  public QDockWidget,
  private Ui::NavigationDockWidget
{
  Q_OBJECT
public:
  /** \brief Constructor */
  explicit QGoNavigationDockWidget(
    QWidget *parent = 0,
    const GoFigure::TabDimensionType & iDim = GoFigure::THREE_D_WITH_T);

  /** \brief Destructor */
  ~QGoNavigationDockWidget();

  /** \brief Set the number of channels */
  void SetNumberOfChannels(const unsigned int & iN);

  /** \brief Set channel name
  *    \param[in] i channel id
  *    \param[in] iText channel name
  */
  void SetChannel( const unsigned int & i, const QString & iText = QString() );

 /** \brief Set the extent of the images in the X direction
 * \param[in] iMin XMin
 * \param[in] iMax XMax
 */
  void SetXMinimumAndMaximum(const int & iMin, const int & iMax);

 /** \brief Set the extent of the images in the Y direction
 * \param[in] iMin YMin
 * \param[in] iMax YMax
 */
  void SetYMinimumAndMaximum(const int & iMin, const int & iMax);

  /** \brief Set the extent of the images in the Z direction
 * \param[in] iMin ZMin
 * \param[in] iMax ZMax
 */
 void SetZMinimumAndMaximum(const int & iMin, const int & iMax);

 /** \brief Set the extent of the images in the Time
 * \param[in] iMin TMin
 * \param[in] iMax TMax
 */
  void SetTMinimumAndMaximum(const int & iMin, const int & iMax);

  /** \brief Get the selected channel id */
  int  GetCurrentChannel() const;

  /** \brief Select and show all channels */
  bool ShowAllChannels()   const;

  /** \brief Get the channel name */
  QString GetChannelName(const int &);

public slots:
  /** Set X Slice */
  void SetXSlice(int iSlice);

  /** Set Y Slice */
  void SetYSlice(int iSlice);

  /** Set Z Slice */
  void SetZSlice(int iSlice);

  /** Set Time Point */
  void SetTSlice(int iSlice);

  /** \todo (Nicolas) to document */
  void MoveToPreviousTimePoint();

  /** \todo (Nicolas) to document */
  void MoveToNextTimePoint();

signals:
  void ShowAllChannelsChanged(bool iChanged);

  void ShowOneChannelChanged(int Channel);

  void XSliceChanged(int Slice);

  void YSliceChanged(int Slice);

  void ZSliceChanged(int Slice);

  void TSliceChanged(int Slice);

protected:
  GoFigure::TabDimensionType m_Dimension;
};
#endif
