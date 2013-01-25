#ifndef CENITALI_H
#define CENITALI_H

// QT includes
#include <QtCore>

// ICE includes
#include <Ice/Ice.h>
#include <Cenital.h>

// User includes...
#include <config.h>
#include "worker.h"

using namespace RoboCompCenital;

class CenitalI : public QObject , public virtual RoboCompCenital::Cenital
{
Q_OBJECT
public:
	CenitalI( Worker *_worker, QObject *parent = 0 );
	~CenitalI();

	QMutex *mutex;
private:

	Worker *worker;
public slots:
	

};

#endif
