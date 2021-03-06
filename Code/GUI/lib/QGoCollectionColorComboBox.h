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

#ifndef __QGoCollectionColorComboBox_h
#define __QGoCollectionColorComboBox_h

#include "QGoColorComboBox.h"

/**
\class QGoCollectionColorComboBox
\brief this class inherits from QGoColorComboBox, has only the add item option and
update the text according to the iCollectionName. It handles the printing of the
collection IDs
\ingroup GUI
*/
class QGOGUILIB_EXPORT QGoCollectionColorComboBox:public QGoColorComboBox
{
  Q_OBJECT
public:
  explicit QGoCollectionColorComboBox(QWidget *iparent = 0);
  virtual ~QGoCollectionColorComboBox();
signals:
  void NewCollectionToCreate();

public slots:
  /**
  \brief clear the items already in the combobox,displayed the one in the iDataFromList,
  the items to add/delete and adjust the text to add an item according to the iCollectionName.
  \param[in] iDataFromList contains the names and QColor of the items to be
  displayed in the combobox
  \param[in] iCollectionName name to use in the add an item
  \overload QGoColorComboBox::SetItemsFromListWithColor(
  std::list< ItemColorComboboxData > iDataFromList)
  */
  void SetItemsFromListWithColor(std::list< ItemColorComboboxData > iDataFromList,
                                 std::string iCollectionName);

  /**
  \brief
  \param[in] iCollectionName name of the collection to add in the text to add a new
  collection
  \overload QGoColorComboBox::InitializeTheListWithColor(
  std::list< ItemColorComboboxData > iDataFromList)
  */
  void InitializeTheListWithColor(std::list< ItemColorComboboxData > iDataFromList,
                                  std::string iCollectionName);

protected slots:
  //mother class method
  virtual void ActionWhenNewOneRequested();
};

#endif
