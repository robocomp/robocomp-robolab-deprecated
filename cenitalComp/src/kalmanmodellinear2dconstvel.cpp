#include "kalmanmodellinear2dconstvel.h"

KalmanModelLinear2DConstVel::KalmanModelLinear2DConstVel()
 : KalmanModel()
{
	A.resize(4,4);
	A(0,0) = 1.;A(0,1) = 0.;A(0,2) = 1.;A(0,3) = 0.;
	A(1,0) = 0.;A(1,1) = 1.;A(1,2) = 0.;A(1,3) = 1.;
	A(2,0) = 0.;A(2,1) = 0.;A(2,2) = 1.;A(2,3) = 0.;
	A(3,0) = 0.;A(3,1) = 0.;A(3,2) = 0.;A(3,3) = 1.;

}


KalmanModelLinear2DConstVel::~KalmanModelLinear2DConstVel()
{
}


void KalmanModelLinear2DConstVel::updateMean( QVec & state )
{
	state = A * state;
}

void KalmanModelLinear2DConstVel::updateVar( QMat & var )
{
   var = ( A * (var * A.transpose() ) );
}
