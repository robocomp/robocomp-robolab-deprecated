#ifndef KALMANMODELLINEAR2DCONSTVEL_H
#define KALMANMODELLINEAR2DCONSTVEL_H

#include <kalmanmodel.h>

/**
	@author authorname <authormail>
*/
class KalmanModelLinear2DConstVel : public KalmanModel
{
public:
    KalmanModelLinear2DConstVel();
    ~KalmanModelLinear2DConstVel();

	virtual void updateMean( QVec &state );
	virtual void updateVar( QMat &var );
	

private:
	QMat A;
};

#endif
