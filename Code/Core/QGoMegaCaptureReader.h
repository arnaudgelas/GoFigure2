#ifndef QGOMEGACAPTUREREADER_H
#define QGOMEGACAPTUREREADER_H

#include <QObject>

class QString;
class QFile;
class QXmlStreamReader;

class QGoMegaCaptureReader : public QObject
{
public:
  QGoMegaCaptureReader();
  ~QGoMegaCaptureReader();

  void Read( const QString& iFilename );

protected:
  QXmlStreamReader* xmlStream;
  QFile* file;

  void ReadImagingSession();
  void ReadImagesInformation();
  void ReadRange();
  void ReadCoordinate();
  void ReadSampling();
  void ReadSpatialSampling( double iFactor );
  void ReadTemporalSampling( double iFactor );
  void ReadChannelList();
  void ReadChannel();
  void ReadColor();
  void ReadMicroscopeSettings();
  void ReadImageData();
  void ReadPlateList();
  void ReadRowList();
  void ReadColumnList();
  void ReadZTileList();
  void ReadYTileList();
  void ReadXTileList();
  void ReadTimePointList();
  void ReadZSliceList();
  void ReadImage();

  unsigned int m_PCoord;
  unsigned int m_RCoord;
  unsigned int m_CCoord;
  unsigned int m_ZTileCoord;
  unsigned int m_YTileCoord;
  unsigned int m_XTileCoord;
  unsigned int m_TCoord;
  unsigned int m_ZCoord;

private:
  Q_DISABLE_COPY( QGoMegaCaptureReader );
};

#endif // QGOMEGACAPTUREREADER_H
