#include "cenitalI.h"

CenitalI::CenitalI( Worker *_worker, QObject *parent) : QObject(parent)
{
	worker = _worker;
	mutex = worker->mutex;				//share worker mutex
	// Component initialization ...
}


CenitalI::~CenitalI()
{
	// Free component resources here
}

// Component functions, implementation
