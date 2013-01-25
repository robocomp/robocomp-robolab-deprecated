#include "kalmanmeasuresquare5points.h"

KalmanMeasureSquare5Points::KalmanMeasureSquare5Points(int width_, int height_)
 : KalmanMeasure()
{
	w = (float)width_;
	h = (float)height_;
	w2 = w / 2.;
	h2 = h / 2.;
}


KalmanMeasureSquare5Points::~KalmanMeasureSquare5Points()
{
}

void KalmanMeasureSquare5Points::getMeasure(  QVec & state, QVec & zE )
{
	//	int tamRob = 68;
	//Dimensiones
	//const float w = 160.;
// 	float w = 155.;
// 	float w2 = w/2.;
// 	float h = 160.;
// 	float h2 = h/2.;
// 	
	//Puntos del modelo canónico. Siempre los mismos
	QMat localZ(2,5);
	localZ(0,0) = 0.;
	localZ(1,0) = 0.;
	localZ(0,1) = -w2;
	localZ(1,1) = -h2;
	localZ(0,2) = -w2;
	localZ(1,2) = h2;
	localZ(0,3) = w2;
	localZ(1,3) = h2;
	localZ(0,4) = w2;
	localZ(1,4) = -h2;
	
	//Rotamos alfa
	Rot2D rot(state.getVal( 2 ));
	localZ = rot * localZ ;
	
	//Transladamos a la posición actual
	zE(0) = localZ(0,0) + state(0);
	zE(1) = localZ(1,0) + state(1);
	zE(2) = localZ(0,1) + state(0);
	zE(3) = localZ(1,1) + state(1);
	zE(4) = localZ(0,2) + state(0);
	zE(5) = localZ(1,2) + state(1);
	zE(6) = localZ(0,3) + state(0);
	zE(7) = localZ(1,3) + state(1);
	zE(8) = localZ(0,4) + state(0);
	zE(9) = localZ(1,4) + state(1);

	//zE.print("zM");
}


