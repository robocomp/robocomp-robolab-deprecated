#ifndef KALMANMODELDIFFERENTIALWHEELS_H
#define KALMANMODELDIFFERENTIALWHEELS_H

#include <kalmanmodel.h>


/**
	@author authorname <authormail>
*/
class KalmanModelDifferentialWheels : public KalmanModel
{
public:
    KalmanModelDifferentialWheels();
    ~KalmanModelDifferentialWheels();

	virtual void updateMean( QVec &state );
	virtual void updateVar( QMat &var );

};

#endif
