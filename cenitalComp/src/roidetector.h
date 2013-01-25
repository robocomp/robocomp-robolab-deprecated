#ifndef ROIDETECTOR_H
#define ROIDETECTOR_H

#include <QtCore>
#include <ipp.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <Camera.h>
#include <Vision.h>
#include <CamMotion.h>
#include <qvisor/qvisor.h>

#include "config.h"

#define WHARRIS (RADIUS*2-1)
#define N_ANILLOS 4
#define RADIO_ANILLO (200/N_ANILLOS)
#define ECC 98
#define ANG 194
#define WENV 13
#define HENV 13
#define WCORR 20
#define HCORR 20
#define PIRNIVELES 6
#define RADIUS 3
#define UMBRAL_HESS 15
#define UMBRAL_LAPLACIAN 10
#define UMBRAL_HARRIS 1.0
#define CHECK_PERIOD 5000

#define NFEAT 4

using namespace RoboCompCamera;

class roiDetector: public QObject
{
Q_OBJECT
	typedef struct
	{
		int xIni, yIni;
		long despIni;
		IppiSize sizeImg;
		IppiSize sizeROI;
	} PrismaROI;

	typedef struct
	{
		int numROIs[PIRNIVELES];
		int tambufROIs[PIRNIVELES];
		int * ROIList[PIRNIVELES];

	} TipoROIList;

typedef Ipp8u* TipoPiramide[PIRNIVELES];

public:
  	roiDetector(RoboCompVision::TCamParams params_, int nCam_, QObject *parent = 0 );
  	~roiDetector();

	void iniPiramide();
	void iniHessian();
	void delHessian();
	void getPiramide(Ipp8u * img);
	void MaxLaplacianHarrisPrisma(int x, int y, int scale, int &level, Ipp8u &Laplacian);
	void MaxLaplacianHarris(int x, int y, int scale, int &level, Ipp8u &Laplacian);
	void extraerROIHarrisPrisma();
	void extraerROIHarris();
	void extraerROIHessian();
	void extraerMSER();

    void getLevelPyr(int level, imgType &img);
    void getWholePyr(imgType &pyr, RoboCompCamMotion::THeadState&, RoboCompDifferentialRobot::TBaseState& );
	void getROILevelPyr(int level, int xc, int yc, int w, int h, imgType &img);
	void getROIWholePyr(int xc, int yc, int w, int h, imgType &img);
    void getLevelROIList(int level, RoboCompVision::ROIVector &roiList);
    void getWholeROIList(RoboCompVision::ROIPyrVectors &roiList);
	inline bool isChecking() { return CHECKING; };
	inline bool isMoving() { return ISMOVING; };

private:
	//const CameraPrx& _camera;
	int  nCam;

	Ipp8u * imagen;
	imgType imagenV;

	TipoPiramide piramideL;
	TipoPiramide *ApiramideG, *BpiramideG, *LecpiramideG, *EscpiramideG;
	std::vector<std::vector<Ipp8u*> > pirE, pirL;
	RoboCompVision::TCamParams params;
	int sizeWholePir;
	IppiSize sizePir[PIRNIVELES];
	PrismaROI infoPrisma[PIRNIVELES];
	int tamBuftmpPirG,tamBuftmpPirL;
	Ipp8u * buftmpPirG,* buftmpPirL;

	//Hessian
	Ipp32f * DX, * DY, * DXX, * DXY, * DYY, * Trace, * Det;
	Ipp32f kernelSum[25];

	Ipp32f * respHarris[PIRNIVELES];
	int * esqHarris[PIRNIVELES];
	int tambufesqHarris[PIRNIVELES];
	int numesqHarris[PIRNIVELES];
	Ipp8u * buftmpHarris[PIRNIVELES];
	Ipp8u * mapaHarris[PIRNIVELES];

	TipoROIList * LecROIList, * EscROIList, * AROIList, * BROIList;
	bool A;

	Ipp32f Umbral_Harris;
	Ipp32f Umbral_Hess;
	Ipp8u Umbral_Laplacian;

	std::string _timeStamp;
	QTimer * timer;
	int BASIC_PERIOD;

	QMutex * mu;
	bool CHECKING;     //true if there are connection problems
	bool ISMOVING;  // false if CameraComp could not supply an image.
	RoboCompVision::ROIPyrVectors RListE, RListL;
	RoboCompCamMotion::THeadState hState;
	RoboCompDifferentialRobot::TBaseState bState;
	void printFPS( );

public slots:
	 void procesa(uchar *img, RoboCompVision::ROIPyrVectors &rl);

};

#endif
