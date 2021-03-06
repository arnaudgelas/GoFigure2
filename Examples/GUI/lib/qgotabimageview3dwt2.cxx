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

#include <QApplication>
#include <QTimer>
#include <QMenuBar>

#include "vtkSmartPointer.h"
#include "vtkLSMReader.h"
#include "vtkImageData.h"

#include "itkMegaCaptureImport.h"
#include "QGoTabImageView3DwT.h"

int main(int argc, char **argv)
{
  if ( argc != 2 )
    {
    std::cerr << "qgotabimageview3dwt2 requires 2 arguments:" << std::endl;
    std::cerr << "1-filename (megacapture)" << std::endl;
    return EXIT_FAILURE;
    }
  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("MegasonLab");
  QCoreApplication::setOrganizationDomain("http://gofigure2.sourceforge.net");

  itk::MegaCaptureImport::Pointer importer = itk::MegaCaptureImport::New();
  importer->SetFileName(argv[1]);
  importer->Update();

  int t = importer->GetOutput().get< m_TCoord >().begin()->m_TCoord;

  QGoTabImageView3DwT *tab = new QGoTabImageView3DwT;
  tab->SetMegaCaptureFile(importer->GetOutput(), GoFigure::PNG,
                          importer->GetHeaderFilename(), t);
  tab->show();

  QMenuBar *               menubar = new QMenuBar;
  std::vector< QAction * > action_vector = tab->ViewActions();

  for ( std::vector< QAction * >::iterator q_it = action_vector.begin();
        q_it != action_vector.end();
        q_it++ )
    {
    menubar->addAction(*q_it);
    }
  menubar->show();

  QTimer *timer = new QTimer;
  timer->setSingleShot(true);
  QObject::connect( timer, SIGNAL( timeout() ), tab, SLOT( close() ) );
  QObject::connect( timer, SIGNAL( timeout() ), menubar, SLOT( close() ) );

  std::list< std::pair< QGoDockWidgetStatus *, QDockWidget * > > dockwidget_list = tab->DockWidget();

  for ( std::list< std::pair< QGoDockWidgetStatus *, QDockWidget * > >::iterator
        it = dockwidget_list.begin();
        it != dockwidget_list.end();
        it++ )
    {
    if ( it->second )
      {
      it->second->show();
      QObject::connect( timer, SIGNAL( timeout() ), it->second, SLOT( close() ) );
      }
    }

    {
    tab->SetTimePoint(0);
    timer->start(1000);
    }

  app.processEvents();

  int output = app.exec();

  app.closeAllWindows();

  delete menubar;
  delete tab;
  delete timer;

  return output;
}
