#ifndef __itkChanAndVeseSegmentationFilter_h
#define __itkChanAndVeseSegmentationFilter_h

#include "itkScalarChanAndVeseLevelSetFunctionData.h"
#include "itkConstrainedRegionBasedLevelSetFunctionSharedData.h"
#include "itkScalarChanAndVeseLevelSetFunction.h"
#include "itkScalarChanAndVeseSparseLevelSetImageFilter.h"
#include "itkAtanRegularizedHeavisideStepFunction.h"
#include "itkDanielssonDistanceMapImageFilter.h"

#include "itkImageToVTKImageFilter.h"

#include "vtkImageData.h"

namespace itk
{
/**
 * \class ChanAndVeseSegmentationFilter
 * \brief
*/
template< class TLevelSetImage, class TFeatureImage >
class ChanAndVeseSegmentationFilter : public Object
{
public:
  typedef ChanAndVeseSegmentationFilter Self;
  typedef SmartPointer< Self > Pointer;
  typedef SmartPointer< const Self > ConstPointer;
  typedef Object Superclass;

  /** Run-time type information (and related methods).   */
  itkTypeMacro( ChanAndVeseSegmentationFilter, Object );
  /** New macro for creation of through a Smart Pointer   */
  itkNewMacro( Self );

  typedef TLevelSetImage LevelSetImageType;
  typedef typename LevelSetImageType::Pointer   LevelSetImagePointer;
  typedef typename LevelSetImageType::PointType LevelSetPointType;
  typedef typename LevelSetPointType::CoordRepType  LevelSetCoordRepType;
  typedef typename LevelSetImageType::IndexType LevelSetIndexType;
  typedef typename LevelSetIndexType::IndexValueType LevelSetIndexValueType;
  typedef typename LevelSetImageType::SizeType LevelSetSizeType;
  typedef typename LevelSetSizeType::SizeValueType LevelSetSizeValueType;
  typedef typename LevelSetImageType::RegionType LevelSetRegionType;
  typedef typename LevelSetImageType::PixelType LevelSetPixelType;

  typedef TFeatureImage FeatureImageType;
  typedef typename FeatureImageType::Pointer FeatureImagePointer;
  typedef typename FeatureImageType::SizeType FeatureSizeType;
  typedef typename FeatureImageType::SpacingType FeatureSpacingType;

  typedef ScalarChanAndVeseLevelSetFunctionData< LevelSetImageType,
    FeatureImageType >    DataHelperType;

  typedef ConstrainedRegionBasedLevelSetFunctionSharedData< LevelSetImageType,
    FeatureImageType, DataHelperType >
    SharedDataHelperType;

  typedef ScalarChanAndVeseLevelSetFunction< LevelSetImageType,
    FeatureImageType, SharedDataHelperType > LevelSetFunctionType;
  typedef ScalarChanAndVeseSparseLevelSetImageFilter< LevelSetImageType,
    FeatureImageType, LevelSetImageType, LevelSetFunctionType, SharedDataHelperType > MultiLevelSetType;
  typedef typename MultiLevelSetType::Pointer MultiLevelSetPointer;

  typedef AtanRegularizedHeavisideStepFunction< LevelSetPixelType, LevelSetPixelType >  DomainFunctionType;
  typedef typename DomainFunctionType::Pointer DomainFunctionPointer;

  typedef DanielssonDistanceMapImageFilter< LevelSetImageType,LevelSetImageType >
    DistanceFilterType;
  typedef typename DistanceFilterType::Pointer DistanceFilterPointer;

  typedef ImageToVTKImageFilter< LevelSetImageType > ConverterType;
  typedef typename ConverterType::Pointer ConverterPointer;

  itkStaticConstMacro( Dimension, unsigned int, LevelSetImageType::ImageDimension);


  void SetCenter( const LevelSetPointType& iC )
  {
    m_Center = iC;
  }
  LevelSetPointType GetCenter( ) const
  {
    return m_Center;
  }

  void SetRadius( const LevelSetCoordRepType& iR )
  {
    m_Radius = iR;
  }
  LevelSetCoordRepType GetRadius( ) const
  {
    return m_Radius;
  }

  void SetFeatureImage( FeatureImageType* iImage )
  {
    m_FeatureImage = iImage;
  }

  void Update()
  {
    GenerateData();
  }

  vtkImageData* GetOutput()
  {
    return m_VTKImage;
  }

protected:
  ChanAndVeseSegmentationFilter() : m_VTKImage( 0 ), m_FeatureImage( 0 )
  {
    m_Converter = ConverterType::New();
    m_Center.Fill( 0. );
    m_Size.Fill( 0 );
    m_Radius = 0.;
  }

  ~ChanAndVeseSegmentationFilter()
  {}

  vtkImageData*         m_VTKImage;
  ConverterPointer      m_Converter;
  FeatureImagePointer   m_FeatureImage;
  LevelSetPointType     m_Center;
  LevelSetSizeType      m_Size;
  LevelSetCoordRepType  m_Radius;


  void GenerateData()
  {
    if( m_FeatureImage.IsNull() )
    {
      std::cerr <<"m_FeatureImage is Null" <<std::endl;
      return;
    }
    FeatureSpacingType spacing = m_FeatureImage->GetSpacing();
    FeatureSizeType inputSize = m_FeatureImage->GetLargestPossibleRegion().GetSize();

    LevelSetIndexType start;
    LevelSetPointType origin;
    LevelSetIndexType cen;
    LevelSetSizeType cellSize;

    for( unsigned int j = 0; j < Dimension; j++ )
    {
      m_Size[j] =
        static_cast<LevelSetSizeValueType>( 2. * m_Radius / spacing[j] );
      start[j] = 0;
      origin[j] = m_Center[j] - m_Radius;
    }

    LevelSetRegionType region;
    region.SetSize( cellSize );
    region.SetIndex( start );

    LevelSetImagePointer levelset = LevelSetImageType::New();
    levelset->SetRegions( region );
    levelset->SetSpacing( spacing );
    levelset->SetOrigin( origin );
    levelset->Allocate();

    //NOTE: Create an initial signed distance function.
    // This could be avoided by initializing with an elliptical function
    levelset->SetPixel( cen, 1 );

    DistanceFilterPointer Dist = DistanceFilterType::New();
    Dist->SetInput( levelset );
    Dist->InputIsBinaryOn();
    Dist->SetUseImageSpacing( 1 );
    Dist->Update();

    DomainFunctionPointer domainFunction = DomainFunctionType::New();
    domainFunction->SetEpsilon( 1. );

    typedef std::vector< unsigned int > VectorType;
    VectorType lookUp( 1, 0 );

    MultiLevelSetPointer levelSetFilter = MultiLevelSetType::New();
    levelSetFilter->SetFunctionCount( 1 );
    levelSetFilter->SetLookup( lookUp );
    levelSetFilter->SetFeatureImage( m_FeatureImage );
    levelSetFilter->SetLevelSet( 0, Dist->GetOutput() );
    levelSetFilter->SetNumberOfIterations( 50 );
    levelSetFilter->SetMaximumRMSError( 0 );
    levelSetFilter->SetUseImageSpacing( true );

    levelSetFilter->GetDifferenceFunction(0)->SetDomainFunction( domainFunction );
    levelSetFilter->GetDifferenceFunction(0)->SetCurvatureWeight( 0. );
    levelSetFilter->GetDifferenceFunction(0)->SetAreaWeight( 0. );
    levelSetFilter->GetDifferenceFunction(0)->SetLambda1( 1. );
    levelSetFilter->GetDifferenceFunction(0)->SetLambda2( 1. );
    levelSetFilter->GetOutput();


    m_Converter->SetInput( levelSetFilter->GetOutput() );
    m_Converter->Update();

    m_VTKImage = m_Converter->GetOutput();
  }

private:
  ChanAndVeseSegmentationFilter( const Self& );
  void operator = ( const Self& );
};
}
#endif
