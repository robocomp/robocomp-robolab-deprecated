#ifndef WORKER_H
#define WORKER_H

#include <QtGui>
#include "ui_cenitalDlg.h"
#include <Camera.h>
#include <DifferentialRobot.h>
#include <CommonHead.h>
#include <ipp.h>
#include "qworld/qworld.h"
#include <QMat/qmat.h>
#include "roidetector.h"

#define CHECK_PERIOD 5000
#define BASIC_PERIOD 18

//Ancho y alto depende de la resolución
#define ANCHO_ROBOT 157
#define ALTO_ROBOT 155
#define LADO_ROBOT ANCHO_ROBOT
//#define LADO_ROBOT 45
#define AREA_ROBOT (ANCHO_ROBOT*ALTO_ROBOT)
#define N_POINTS_MODEL 5
#define MAX_VAR_ALLOWED 200
#define LADO_COMP 208
#define AREA_COMP (LADO_COMP*LADO_COMP)
#define DOWN_SIZE 10

//! Worker class for cenitalComp
/*!
   A more elaborate class description.
	@author Pablo Bustos
*/

class Worker : public QWidget , public Ui::CenitalDlg
{
Q_OBJECT
public:
    Worker(RoboCompCamera::CameraPrx _cameraPrx,  QWidget *parent = 0);
    ~Worker();
	QMutex *mutex;					//Shared mutex with servant

private:
	RoboCompCamera::CameraPrx cameraPrx;
	RoboCompCamera::TCamParams params;
	RoboCompCamera::imgType imagenV;
	RoboCompCommonHead::THeadState hState;
	RoboCompDifferentialRobot::TBaseState bState;
	QImage *img1,*img2,*img3,*img4,*imgG,*imgR,img;
	qWorld *visor1,*visor2,*visor3,*visor4;
	
	Ipp8u *imagen, *bufUndist;
	Ipp8u *auxRGB,*auxRect,*auxPlanos[3];
	
	IppiRect rect;
	QTimer timer,trecord;
	IppiSize roiSize;
	int step, stepRGB;

	int xRobot, yRobot, wRobot, hRobot;

	inline int printFPS();
	void iniDistortion();
	//void segmentate(int &cx, int &cy);

	float RHOX, RHOY, KHAX, KHAY, FOCAL;

	typedef struct
	{
		int x;		 // x coordinate
		int y;		 // y coordinate
		float alfa; // alfa coordinate
		bool match;
		int xm;
		int ym;
		float alfam;
	} TProj;

	typedef QVector<TProj> TProjList;

	//Segmentate
// 	uchar *imgSeg;
// 	void segmentateColor(uchar *planosRGB[3], uchar *imgSeg, QColor c);


	//Canny
// 	Ipp8u* cannyImg;
// 	void cannyFilterRegion(uchar *imgIn, uchar *imgOut, int cx, int cy, int sx, int sy);
// 	IppiSize srcSize;
// 	Ipp16s *img16;
// 	Ipp16s *SobelHorz;
//  	Ipp16s *SobelVert;
// 	Ipp8u *tmpbuf,*img8, *imgROI;
// 	int width16;
// 	void iniCanny(int sx, int sy );
 	int MINCANNY, MAXCANNY;

	//Regiones
	roiDetector *roiProcess;
	//void drawRegions(RoboCompVision::ROIPyrVectors regionList, QColor c=Qt::black,  int levMin=0, int levMax=0);
	//void drawRegions(RoboCompVision::TRoi region, const QColor c=Qt::black);

	//Visores
	uchar *virtualImg, *realImg;
	qWorld *realVisor, *virtualVisor;

	//Mouse
	bool mousePressed;
	QPoint iniPoint, currentPoint;

	//Widget extras
	//void widgetExtras();

	//Saliency
	//void saliencyOfRegions( RoboCompVision::ROIPyrVectors &regionList, RoboCompVision::TRoi &roi, int &sal );

	//Guess
	//bool guess( QString s, uchar *planosRGB[3], RoboCompVision::ROIPyrVectors &regionList, RoboCompVision::TRoi &roi , QColor color);
	bool newRoiBool;
	RoboCompVision::TRoi newRoi;

	int c2sX(float n);
	int c2sY(float n);
 	float s2cX(int n);
	float s2cY(int n);

	//Extract roi
	//void extractRoi(uchar*, int, int, int, uchar*, IppiSize &);
	int bufDownSample;
	uchar *realImgDown, *virtualImgRot;
	qWorld *measZVisor, *measRVisor;
	uchar *measImgReal, *measImgVirtual;
	int imagenChange;
	typedef struct{
	      Ipp32f *xMap, *yMap;
	      int xStep,yStep;
	}p;
	QVector<p> img_vector;

public slots:
	void compute();
	void on_sbRhoX_valueChanged(int val);
	void on_sbRhoY_valueChanged(int val);
	void on_sbKhaX_valueChanged(int val);
	void on_sbKhaY_valueChanged(int val);
	void on_pbResetK_clicked();
	void newTrackWin(QRect);
	void mouseClickSignal(QPoint);
	void mouseReleaseSignal(QPoint);
	void mouseMovementSignal(QPoint);
	void crossToggled(bool);
	void on_hsQ_valueChanged(int val);
	void on_hsR_valueChanged(int val);
	void on_pbRecord_clicked();
	void record();
	void changeImagen();
// 	void on_hsMinCanny_valueChanged(int v);
// 	void on_hsMaxCanny_valueChanged(int v);

};

#endif
