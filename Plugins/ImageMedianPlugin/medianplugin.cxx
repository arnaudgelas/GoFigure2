#include "medianplugin.h"

#include <QAction>
#include <iostream>

Q_EXPORT_PLUGIN2( QGoIdentityImageFilterPlugin, medianplugin )

medianplugin::medianplugin( QObject* iParent ) : QGoImageFilterPluginBase( iParent )
{
  this->SetAllRequiredInformation();
  preprocess = new QCellPreprocess();
  QObject::connect( preprocess, SIGNAL( Done() ), this, SIGNAL( Done() ) );
}

medianplugin::~medianplugin()
{}

std::vector< QAction* > medianplugin::Actions()
{
  QAction* action = new QAction( tr( "&Median" ), this );

  QObject::connect( action, SIGNAL( triggered ),
    this, SLOT( Process( ) ) );

  std::vector< QAction* > oActionList;
  oActionList.push_back( action );

  return oActionList;
}

QToolBar* medianplugin::ToolBar()
{
  return 0;
}

QMenu* medianplugin::Menu()
{
  return 0;
}

QDockWidget* medianplugin::DockWidget()
{
  return 0;
}

QWidget* medianplugin::AdditionalWidget()
{
  return 0;
}

std::list< GoFigure::TabDimensionType >
medianplugin::TabElementCompatibility() const
{
  std::list< GoFigure::TabDimensionType > oCompatibility;
  oCompatibility.push_back( GoFigure::TWO_D );
  oCompatibility.push_back( GoFigure::THREE_D );

  return oCompatibility;
}

void medianplugin::Process( )
{
  preprocess->SetInput( m_VTKInput );
  preprocess->setVisible( true );
//   m_VTKOutput = preprocess->GetOutput();
  std::cout << "medianplugin::Process" << std::endl;
}

void medianplugin::SetAllRequiredInformation()
{
  this->m_Information.Name = "MedianFiltering";
  this->m_Information.Version = "1.0";
  this->m_Information.GoFigureCompatibilityVersion = "1.0";
  this->m_Information.Distributor = "Megason Lab";
  this->m_Information.Copyright = "Harvard Medical School";
  this->m_Information.License = "BSD";
  this->m_Information.Description = "Test: plugin which median filters the input";
}

