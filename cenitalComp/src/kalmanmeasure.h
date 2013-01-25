///Abstract class to derive specific measure models for the KalmanXXX classes


#ifndef KALMANMEASURE_H
#define KALMANMEASURE_H

#include <QMat/qmat.h>
#include <QMat/qvec.h>

/**
	@author Pablo Bustos <authormail>
*/

class KalmanMeasure{

public:
    KalmanMeasure(){};
    virtual ~KalmanMeasure(){};

	virtual void getMeasure( QVec &state, QVec &zE) = 0;
};

#endif
