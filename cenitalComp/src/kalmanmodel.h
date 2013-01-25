///Abstract class to derive specific models for the KalmanXXX classes

#ifndef KALMANMODEL_H
#define KALMANMODEL_H

#include <QMat/qmat.h>
#include <QMat/qvec.h>

/**
	@author Pablo Bustos <authormail>
*/


class KalmanModel{

public:
    KalmanModel(){};
    virtual ~KalmanModel(){};

	virtual void updateMean( QVec &state ) = 0;
	virtual void updateVar( QMat &var ) = 0;


};

#endif
