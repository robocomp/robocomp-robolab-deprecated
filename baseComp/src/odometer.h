#ifndef ODOMETER_H
#define ODOMETER_H

#include <QtCore>
#include "basehandler.h"

/**
	@author authorname <authormail>
*/
class Odometer : public QObject
{
	Q_OBJECT
public:
    Odometer(BaseHandler *baseHandler);
    ~Odometer();
	void getBasePosition(float &x , float &y){x = POSX ; y = POSY;};
private:
	QTimer *timer;
	QTime *reloj;
	BaseHandler *_baseHandler;
	int TIMER_PERIOD;
	float POSX, POSY;
public slots:
	void compute();  //It should go in the Microcontroller at 500 Hz
	void resetOdometer();
};

#endif
