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

#include "QGoTraceManualEditingWidget.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>

// DEBUG
#include <QtDebug>
#include <iostream>

QGoTraceManualEditingWidget::QGoTraceManualEditingWidget(QWidget *iParent):
  QWidget(iParent)
{
  this->setupUi(this);
  SetSelectedColorComboBox();
  SetTraceCollectionColorComboBox();
  SetCellTypeComboBox();
  SetSubCellTypeComboBox();
  UpdateTraceAndCollection("contour", "mesh");
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QGoTraceManualEditingWidget::
~QGoTraceManualEditingWidget()
{}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoTraceManualEditingWidget::SetListCollectionID(
  std::list< ItemColorComboboxData > iListExistingID,
  std::string iCollectionIDtoSelect)
{
  if ( !iCollectionIDtoSelect.empty() )
    {
    this->m_CollectionColorComboBox->SetItemsFromListWithColor( iListExistingID,
                                                                this->m_CollectionName->text().toStdString() );
    }
  else
    {
    this->m_CollectionColorComboBox->InitializeTheListWithColor( iListExistingID,
                                                                 this->m_CollectionName->text().toStdString() );
    }
  /** \todo Lydie: when using lineages, remove the following*/
  if ( this->m_CollectionName->text() == "lineage" ) //at that time we don't
                                                     // show lineages
    {
    this->m_CollectionColorComboBox->hide();
    }
  else
    {
    this->m_CollectionColorComboBox->show();
    }
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoTraceManualEditingWidget::SetListColors(
  std::list< ItemColorComboboxData > iListColors, std::string iColorToSelect)
{
  this->SetListItemAndSelect< QGoSelectedColorComboBox >(this->m_SelectedColorComboBox,
                                                         iListColors, iColorToSelect);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoTraceManualEditingWidget::SetListCellTypes(NamesDescrContainerType iCellTypesData,
                                              std::string iCellTypeToSelect)
{
  this->SetListItemAndSelect< QGoComboBox >(this->m_ChoseCellType,
                                            iCellTypesData, iCellTypeToSelect);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoTraceManualEditingWidget::SetListSubCellTypes(NamesDescrContainerType iSubCellTypesData,
                                                 std::string iSubCellTypeToSelect)
{
  this->SetListItemAndSelect< QGoComboBox >(this->m_ChoseSubCellType,
                                            iSubCellTypesData, iSubCellTypeToSelect);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoTraceManualEditingWidget::SetSelectedColorComboBox()
{
  this->m_SelectedColorComboBox = new QGoSelectedColorComboBox;

  this->VLayoutColor->addWidget(ColorLbl);
  this->VLayoutColor->addWidget(this->m_SelectedColorComboBox);

  QObject::connect( this->m_SelectedColorComboBox,
                    SIGNAL( ItemSelected(ItemColorComboboxData) ),
                    this, SIGNAL( NewSelectedColorActivated(ItemColorComboboxData) ) );

  QObject::connect( this->m_SelectedColorComboBox,
                    SIGNAL( AddNewColorActivated() ),
                    this, SIGNAL( AddNewColor() ) );

  QObject::connect( this->m_SelectedColorComboBox,
                    SIGNAL( DeleteActivated() ),
                    this, SIGNAL( DeleteColor() ) );
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoTraceManualEditingWidget::SetTraceCollectionColorComboBox()
{
  this->m_TraceName = new QLabel( tr("contour") );
  QFont ifont;
  ifont.setCapitalization(QFont::AllUppercase);
  ifont.setBold(true);
  this->m_TraceName->setFont(ifont);
  QHBoxLayout *HLayoutForTrace = new QHBoxLayout;
  HLayoutForTrace->addWidget(this->TraceLbl);
  HLayoutForTrace->addWidget(this->m_TraceName);
  QLabel *CollectionLbl = new QLabel( tr("Current Collection:") );
  ifont.setCapitalization(QFont::Capitalize);
  ifont.setPointSize(8);
  ifont.setUnderline(true);
  ifont.setWeight(50);
  ifont.setBold(false);
  CollectionLbl->setFont(ifont);
  this->m_CollectionColorComboBox = new QGoCollectionColorComboBox;
  this->m_CollectionName = new QLabel ( tr("mesh") );
  ifont.setUnderline(false);
  ifont.setCapitalization(QFont::AllUppercase);
  ifont.setBold(true);
  this->m_CollectionName->setFont(ifont);

  QHBoxLayout *HLayoutForCollection = new QHBoxLayout;
  HLayoutForCollection->addWidget(this->m_CollectionName);
  HLayoutForCollection->addWidget(this->m_CollectionColorComboBox);
  this->VLayoutTraceCollection->addLayout(HLayoutForTrace);
  this->VLayoutTraceCollection->addWidget(CollectionLbl);
  this->VLayoutTraceCollection->addLayout(HLayoutForCollection);

  QObject::connect( this->m_CollectionColorComboBox,
                    SIGNAL( ItemSelected(ItemColorComboboxData) ),
                    this, SIGNAL( NewCollectionActivated(ItemColorComboboxData) ) );

  QObject::connect( this->m_CollectionColorComboBox,
                    SIGNAL( NewCollectionToCreate() ),
                    this, SIGNAL( NewCollectionToBeCreated() ) );
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
/*void
QGoTraceManualEditingWidget::
SetEnableTraceCollectionColorBoxes(bool Enable)
{
  this->m_CollectionColorComboBox->setEnabled(Enable);
  this->m_SelectedColorComboBox->setEnabled(Enable);
}*/
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoTraceManualEditingWidget::SetCellTypeComboBox()
{
  this->m_ChoseCellType = new QGoComboBox("Add a new celltype...",
                                          this, "Delete a celltype...");
  QHBoxLayout *HLayoutForCellType = new QHBoxLayout;
  HLayoutForCellType->addWidget(m_ChoseCellType);
  this->VLayoutCellType->addLayout(HLayoutForCellType);

  QObject::connect( this->m_ChoseCellType,
                    SIGNAL( ItemSelected(std::string) ),
                    this, SIGNAL( NewCellTypeActivated(std::string) ) );

  QObject::connect( this->m_ChoseCellType,
                    SIGNAL( AddANewOneActivated() ),
                    this, SIGNAL( AddANewCellType() ) );

  QObject::connect( this->m_ChoseCellType,
                    SIGNAL( DeleteActivated() ),
                    this, SIGNAL( DeleteCellType() ) );
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void
QGoTraceManualEditingWidget::SetSubCellTypeComboBox()
{
  this->m_ChoseSubCellType = new QGoComboBox("Add a new subcelltype...", this,
                                             "Delete a subcelltype...");
  this->VLayoutSubCellType->addWidget(LabelSubCellType);
  this->VLayoutSubCellType->addWidget(m_ChoseSubCellType);

  QObject::connect( this->m_ChoseSubCellType,
                    SIGNAL( ItemSelected(std::string) ),
                    this, SIGNAL( NewSubCellTypeActivated(std::string) ) );

  QObject::connect( this->m_ChoseSubCellType,
                    SIGNAL( AddANewOneActivated() ),
                    this, SIGNAL( AddANewSubCellType() ) );

  QObject::connect( this->m_ChoseSubCellType,
                    SIGNAL( DeleteActivated() ),
                    this, SIGNAL( DeleteSubCellType() ) );
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoTraceManualEditingWidget::SetCurrentCellType(
  std::string iCellTypeText)
{
  this->m_ChoseCellType->SetCurrentItem(iCellTypeText);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoTraceManualEditingWidget::SetCurrentSubCellType(
  std::string iSubCellTypeText)
{
  this->m_ChoseSubCellType->SetCurrentItem(iSubCellTypeText);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoTraceManualEditingWidget::SetCurrentColor(std::string iColorText)
{
  this->m_SelectedColorComboBox->SetCurrentItem(iColorText);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoTraceManualEditingWidget::SetCurrentCollectionID(std::string iID)
{
  this->m_CollectionColorComboBox->SetCurrentItem(iID);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoTraceManualEditingWidget::UpdateTraceAndCollection(
  std::string iTrace, std::string iCollection)
{
  std::cout << iTrace << std::endl;

  this->m_TraceName->setText( iTrace.c_str() );
  this->m_CollectionName->setText( iCollection.c_str() );
  //this->m_CollectionColorComboBox->SetTextToAdd(iCollection);
  if ( iTrace == "contour" || iTrace == "mesh" )
    {
    this->m_ChoseCellType->show();
    this->LabelCellType->show();
    this->m_ChoseSubCellType->show();
    this->LabelSubCellType->show();
    }
  else
    {
    this->m_ChoseCellType->hide();
    this->LabelCellType->hide();
    this->m_ChoseSubCellType->hide();
    this->LabelSubCellType->hide();
    }
  // update visualization
  this->show();
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
std::string QGoTraceManualEditingWidget::GetTraceName()
{
  return this->m_TraceName->text().toStdString();
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGoTraceManualEditingWidget::AddANewCollectionID(
  ItemColorComboboxData iNewCollectionID)
{
  this->m_CollectionColorComboBox->AddItemWithColor(iNewCollectionID, true);
}

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
