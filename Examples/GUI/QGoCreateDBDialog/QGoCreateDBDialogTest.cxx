#include <qapplication.h>
#include "QGoCreateDBDialog.h"

#include "stdio.h"

int main( int argc, char** argv )
{
  QApplication app(argc, argv);

  QGoCreateDBDialog* dialog = new QGoCreateDBDialog;

  return 0;
}
