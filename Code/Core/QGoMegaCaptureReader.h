#ifndef QGOMEGACAPTUREREADER_H
#define QGOMEGACAPTUREREADER_H

#include <QObject>

class QString;
class QFile;
class QXmlStreamReader;

class QGoMegaCaptureReader : public QObject
{
    Q_OBJECT
public:
  QGoMegaCaptureReader();
  ~QGoMegaCaptureReader();

  void Read( const QString& iFilename );

signals:
  void ChannelRead( );
  void ColorRead( );

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

  QString m_ProjectName;
  QString m_ImagingSessionName;
  QString m_ImagingSessionDescription;
  QString m_ImagingSessionCreationDate;
  QString m_FileType;

  unsigned int m_SizeX;
  unsigned int m_SizeY;

  double m_XTileOverlap;
  double m_YTileOverlap;
  double m_ZTileOverlap;

  double m_SpacingX;
  double m_SpacingY;
  double m_SpacingZ;
  double m_SpacingT;

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
