#ifndef ROIDETECTOR_H
#define ROIDETECTOR_H

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <iostream>
#include <QtCore>

#include <ipp.h>
#include <Camera.h>
#include <Vision.h>
#include <CamMotion.h>

#include <qvisor/qvisor.h>

#include "tamanos.h"
#include "config.h"

#define WHARRIS (RADIUS*2-1)
#define N_ANILLOS 4
#define RADIO_ANILLO (200/N_ANILLOS)

#define NFEAT 4
#include <logpolar/lpolar.h>

//Dimensiones de Mapa_Umbral_Harris
#define PAN_RANK 320
#define TILT_RANK 240


using namespace RoboCompCamera;

class roiDetector: public QObject
{
Q_OBJECT
	struct PrismaROI
	{
		int xIni, yIni;
		long despIni;
		IppiSize sizeImg;
		IppiSize sizeROI;
	};
	
	struct TipoROIList
	{
		int numROIs[PIRNIVELES];
		int tambufROIs[PIRNIVELES];
		int * ROIList[PIRNIVELES];
	};

	typedef Ipp8u* TipoPiramide[PIRNIVELES];

public:
	roiDetector(const CameraPrx& camera, RoboCompVision::TCamParams params_, int nCam_, QObject *parent = 0 );
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
	void getWholePyr(imgType &pyr);
	void getROILevelPyr(int level, int xc, int yc, int w, int h, imgType &img);
	void getROIWholePyr(int xc, int yc, int w, int h, imgType &img);
	void getLevelROIList(int level, RoboCompVision::ROIVector &roiList);
	void getWholeROIList(RoboCompVision::ROIPyrVectors &roiList);
	inline bool isChecking() 											{ return CHECKING; };
	inline bool isMoving()												{ return ISMOVING; };
	void procesa(uchar *img, const RoboCompVision::TState & state);
	void swapBuffers();
	void getMaxPrismSize(int & w, int & h){w=sizePir[params.pyrLevels-3].width; h=sizePir[params.pyrLevels-3].height;};

private:	
	const CameraPrx& _camera;
	int  nCam;

	Ipp8u * imagen;
	imgType imagenV;
	
	TipoPiramide piramideL;
	TipoPiramide *ApiramideG, *BpiramideG, *LecpiramideG, *EscpiramideG;
	RoboCompVision::TCamParams params;
	int sizeWholePir;
	IppiSize * sizePir;
	PrismaROI * infoPrisma;
	int tamBuftmpPirG,tamBuftmpPirL;
	Ipp8u * buftmpPirG,* buftmpPirL;

	// Gauss
	IppiSize gaussSize;

	//Hessian
	Ipp32f * DX, * DY, * DXX, * DXY, * DYY, * Trace, * Det;
	Ipp32f kernelSum[25];
	
	//Harris-Laplace
	Ipp32f * respHarris[PIRNIVELES];
	int * esqHarris[PIRNIVELES];
	int tambufesqHarris[PIRNIVELES];
	int numesqHarris[PIRNIVELES];
	Ipp8u * buftmpHarris[PIRNIVELES];
	Ipp8u * mapaHarris[PIRNIVELES];
	Ipp32f Umbral_Harris, Umbral_Harris_Min;
	Ipp32f Umbral_Hess;
	Ipp8u Umbral_Laplacian;
	Ipp32f * Mapa_Umbral_Harris1, * Mapa_Umbral_Harris2;
	Ipp32f * Mapa_Umbral_Harris_Act, * Mapa_Umbral_Harris_Ant;

	//Swap lists
	TipoROIList * LecROIList, * EscROIList, * AROIList, * BROIList;
	bool A;
	
	LPolar *lp;
	bool CHECKING;												//true if there are connection problems
	bool ISMOVING;  											// false if CameraComp could not supply an image.
	RoboCompVision::ROIPyrVectors RListE, RListL;
	RoboCompVision::TState state, stateNew;
	void printFPS( );
	void imgToAng(int x, int y, float& pan, float& tilt);
	void angToMapIndex(float pan, float tilt, int &pan_i, int &tilt_i);
	
	int totalRois;

};

#endif
