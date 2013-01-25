#ifndef KALMANMEASURESQUARE5POINTS_H
#define KALMANMEASURESQUARE5POINTS_H

#include <kalmanmeasure.h>


/**
	@author authorname <authormail>
*/
class KalmanMeasureSquare5Points : public KalmanMeasure
{
public:
    KalmanMeasureSquare5Points(int width_, int height_);
    ~KalmanMeasureSquare5Points();
	
	virtual void getMeasure( QVec &state, QVec &zE);

private:
	float w, w2;
	float h, h2;

};

#endif
