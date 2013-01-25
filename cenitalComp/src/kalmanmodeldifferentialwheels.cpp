
#include "kalmanmodeldifferentialwheels.h"

KalmanModelDifferentialWheels::KalmanModelDifferentialWheels()
 : KalmanModel()
{
}


KalmanModelDifferentialWheels::~KalmanModelDifferentialWheels()
{
}


void KalmanModelDifferentialWheels::updateMean( QVec & state )
{
	float *v = &state(3);
	float *w = &state(4);
	float *x = &state(0);
	float *y = &state(1);
	float *alfa = &state(2);
 	*x = *x + (*v) * sin(*alfa);
 	*y = *y - (*v) * cos(*alfa);
	*alfa = *alfa + *w;		
}

void KalmanModelDifferentialWheels::updateVar( QMat & var )
{
 var=var;
}
