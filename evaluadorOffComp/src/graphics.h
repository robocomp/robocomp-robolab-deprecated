#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <QtGui>
#include <set>
#include <list>
#include "ui_graphicsViewerDlg.h"
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <math.h>

#include <QDebug>
#include <qwt_plot_marker.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_zoomer.h>

using namespace std;
class graphics: public QWidget, public Ui_graphicsWindow
{
Q_OBJECT
  
private:
  
  QVector<double> waistX,waistY,waistZ;
  QVector<double> torsoX,torsoY,torsoZ;
  QVector<double> neckX,neckY,neckZ;
  QVector<double> rightShoulderX,rightShoulderY,rightShoulderZ,leftShoulderX,leftShoulderY,leftShoulderZ;
  QVector<double> rightKneeX,rightKneeY,rightKneeZ,leftKneeX,leftKneeY,leftKneeZ;
  QVector<double> rightHipX,rightHipY,rightHipZ,leftHipX,leftHipY,leftHipZ;
  QVector<double> rightElbowX,rightElbowY,rightElbowZ,leftElbowX,leftElbowY,leftElbowZ;

  ///Synthetic wave values.
  QVector<double> synthWave;
  ///Synthetic wave params.
  float synthGain, synthOffset, synthFreq, synthPhase;
  int synthSize;
  
  ///Zoomer of the QWT graph
  QwtPlotZoomer *some_zoomer;
  
  ///Synthetic wave.
  QwtPlotCurve *synthCurve;
  
  ///Torso
  QwtPlotCurve *TorsorxC, *TorsoryC, *TorsorzC;
  ///Waist
  QwtPlotCurve *waistrxC, *waistryC, *waistrzC;
  ///Neck
  QwtPlotCurve *nrxC, *nryC, *nrzC;
  ///Shoulders
  QwtPlotCurve *rsrxC, *rsryC, *rsrzC;
  QwtPlotCurve *lsrxC, *lsryC, *lsrzC;
  ///Knees
  QwtPlotCurve *rkrxC, *rkryC, *rkrzC;
  QwtPlotCurve *lkrxC, *lkryC, *lkrzC;
  ///Hips
  QwtPlotCurve *rhrxC, *rhryC, *rhrzC;
  QwtPlotCurve *lhrxC, *lhryC, *lhrzC;
  ///Elbows
  QwtPlotCurve *rerxC, *reryC, *rerzC;
  QwtPlotCurve *lerxC, *leryC, *lerzC;
    
  ///Vector de intervalos de periodo
  QVector<double> tiempo;
  
  //Parametros de Cabecera de fichero
  QString headerVersion,centerName,responsableName,patientName,dateSesion,timeInitial,timeFinal,joints;
  float period;

  ///Vertical and Horizontal Line Markers (time and value)
  QwtPlotMarker *mX,*mY;
  
  ///The joint which is refered to by the horizontal marker.
  QString jointHorizontalMarker;
  
public:
 
  graphics(QString filePath);
  
  void getDataFromFile(QString filePath);
  
  void addData(QString jointName, float rotationX, float rotationY, float rotationZ);
  
  QPen getColor(int Index);
  
  void drawWaist();
  void drawTorso();
  void drawShoulders(int);
  void drawKnees();
  void drawHips();
  void drawElbows();
  void drawNeck();
  void drawSynth();
  void drawMarkers(float mseconds);
  
  void storeHeaderParam(QStringList headerParam);
    
  ~graphics();

public slots:
  
//   void openFile();
  void drawAction();
  void refreshPlot();
  
  ///Torso
  void torsorx_HMarker();
  void torsory_HMarker();
  void torsorz_HMarker();
  
  ///Waist
  void waistrx_HMarker();
  void waistry_HMarker();
  void waistrz_HMarker();
  
  ///Shoulders
  void rsrx_HMarker();
  void rsry_HMarker();
  void rsrz_HMarker();
  void lsrx_HMarker();
  void lsry_HMarker();
  void lsrz_HMarker();

  ///Knees
  void rkrx_HMarker();
  void rkry_HMarker();
  void rkrz_HMarker();
  void lkrx_HMarker();
  void lkry_HMarker();
  void lkrz_HMarker();
  
  ///Hips
  void rhrx_HMarker();
  void rhry_HMarker();
  void rhrz_HMarker();
  void lhrx_HMarker();
  void lhry_HMarker();
  void lhrz_HMarker();
  
  ///Elbows
  void rerx_HMarker();
  void rery_HMarker();
  void rerz_HMarker();
  void lerx_HMarker();
  void lery_HMarker();
  void lerz_HMarker();
  
  ///Neck
  void nrx_HMarker();
  void nry_HMarker();
  void nrz_HMarker();

  /// Synthetic wave updating
  void updateSynth();
  void setSynthGain(int);
  void setSynthOffset(int);
  void setSynthFreq(int);
  void setSynthPhase(int);
  void initSynthWave();
  
  void disableAllJoints();
  void disablePreviousJoints();
};

#endif // GRAPHICS_H
