#include "baseI.h"

BaseI::BaseI( BaseHandler *bh , QObject *parent) : QObject(parent) , _bh(bh)
{
	mutex = new QMutex();
	_bh->initMutex(mutex);
}

BaseI::~BaseI()
{
	delete mutex;
}

// Component functions, implementation


BaseHandler *BaseI::getBaseHandler()
{
	QMutexLocker lock(mutex);
	return _bh;
}
//new interface
bool BaseI::setMotorsStateNormal(const Ice::Current &)
{
	QMutexLocker lock(mutex);
	return _bh->setMotorsState( MOTORS_STATE_NORMAL );
}

bool BaseI::setMotorsStateFree(const Ice::Current &)
{
	QMutexLocker lock(mutex);
	return _bh->setMotorsState( MOTORS_STATE_FREE );
}

bool BaseI::setMotorsStateBreak(const Ice::Current &)
{
	QMutexLocker lock(mutex);
	return _bh->setMotorsState( MOTORS_STATE_BREAK );
}

bool BaseI::stopBase(const Ice::Current &)
{
	QMutexLocker lock(mutex);
	return _bh->stopBase();
}

bool BaseI::setSpeedBase(Ice::Float adv, Ice::Float rot, const Ice::Current &)
{
	bool rvalue;
	QMutexLocker lock(mutex);
	rvalue=_bh->setSpeedBase( adv , rot  );
	return rvalue;
}

bool BaseI::setSpeedWheels(Ice::Float left, Ice::Float right, const Ice::Current &)
{
	QMutexLocker lock(mutex);
	return _bh->setSpeedWheels( left , right  );
}

bool BaseI::getSpeedBase(Ice::Float & adv, Ice::Float & rot, const Ice::Current &)
{
/*	QMutexLocker lock(mutex);
	return false;*/
	adv=adv;rot=rot;
	return false;
}

bool BaseI::resetOdometer(const Ice::Current &)
{
	QMutexLocker lock(mutex);	
	return _bh->resetOdometer();
}

void BaseI::setOdometer(const RoboCompBase::TBaseState &state, const Ice::Current &)
{
	QMutexLocker lock(mutex);	
	_bh->setOdometer( state);
}

bool BaseI::getPosWheels(Ice::Int & izq, Ice::Int & der, const Ice::Current &)
{
	/*QMutexLocker lock(mutex);
	return _bh->getPosWheels(izq, der);*/
	izq=izq;der=der;
	return false;
}

bool BaseI::getSpeedWheels(Ice::Float & izq, Ice::Float & der, const Ice::Current &)
{
	QMutexLocker lock(mutex);
	return _bh->getSpeedWheels(izq, der);
}

bool BaseI::getPosBase(Ice::Float & x, Ice::Float & y, Ice::Float & alfa, const Ice::Current &)
{
/*	QMutexLocker lock(mutex);*/
	//return _bh->getPosBase(x ,y, alfa);
	x=x;y=y;alfa=alfa;
	return false;
}

bool BaseI::setPosWheels(Ice::Int left, Ice::Float vi, Ice::Int right, Ice::Float vr, const Ice::Current &)
{
/*	QMutexLocker lock(mutex);*/
	left=left;vi=vi;right=right;vr=vr;
	return false;
}

bool BaseI::setPosBase(Ice::Int x, Ice::Int y, Ice::Float alfa, Ice::Float adv, Ice::Float rot, bool angleCorrection, const Ice::Current &)
{
	QMutexLocker lock(mutex);
	return _bh->setPosBase(x ,y, alfa, adv, rot, angleCorrection);
}

void BaseI::stopSetPosBase(const Ice::Current &){
	_bh->stopSetPosBase();
}

bool BaseI::gotoMagDir(Ice::Int d, Ice::Float v, const Ice::Current &)
{
	QMutexLocker lock(mutex);
	d=d; v=v;
	return false;
}

void BaseI::getBaseState(RoboCompBase::TBaseState &bState_, const Ice::Current &)
{
	QMutexLocker lock(mutex);
	bState_ = _bh->getBaseState();
}


