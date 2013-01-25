/****************************************************************************
#     baseI.h - Definition of BaseI class.                      #
#     Copyright (C) 2006 Robolab-Unex						                #
#                                                                           #
# This program is free software; you can redistribute it and/or modify      #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation; either version 2 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# This program is distributed in the hope that it will be useful,           #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with this program; if not, write to the Free Software               #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA #
#                                                                           #
****************************************************************************/

#ifndef BASEI_H
#define BASEI_H

#include <string>
#include <iostream>
#include <QtCore>
#include <Base.h>
#include <basehandler.h>


using namespace RoboCompBase;

class BaseI : public QObject , public virtual RoboCompBase::Base
{
    Q_OBJECT
    public:
        BaseI( BaseHandler *baseHandler, QObject *parent = 0 );
        ~BaseI();
    	
        // Component public functions

		void getBaseState( RoboCompBase::TBaseState&, const Ice::Current &);	

		bool setSpeedBase(Ice::Float adv, Ice::Float rot, const Ice::Current& = Ice::Current());
		bool getSpeedBase(Ice::Float &adv, Ice::Float &rot, const Ice::Current& = Ice::Current());
		bool setPosBase(Ice::Int x, Ice::Int z, Ice::Float alfa, Ice::Float adv, Ice::Float rot, bool angleCorrection, const Ice::Current& = Ice::Current());
		bool getPosBase(Ice::Float &x, Ice::Float &z, Ice::Float &alfa, const Ice::Current& = Ice::Current());
			
		bool setSpeedWheels(Ice::Float left, Ice::Float right, const Ice::Current& = Ice::Current());
		bool getSpeedWheels(Ice::Float &left, Ice::Float &right, const Ice::Current& = Ice::Current());
		bool setPosWheels(Ice::Int left, Ice::Float vi, Ice::Int right , Ice::Float vr, const Ice::Current& = Ice::Current());
		bool getPosWheels(Ice::Int &left, Ice::Int &right, const Ice::Current& = Ice::Current());
		
		bool setMotorsStateNormal(const Ice::Current& = Ice::Current() );
        bool setMotorsStateFree( const Ice::Current& = Ice::Current() );
        bool setMotorsStateBreak( const Ice::Current& = Ice::Current() );
        
		bool stopBase( const Ice::Current& = Ice::Current() );
		bool resetOdometer(const Ice::Current& = Ice::Current());
		void setOdometer( const RoboCompBase::TBaseState &state, const Ice::Current& = Ice::Current());
		
		bool gotoMagDir(Ice::Int d , Ice::Float v, const Ice::Current& = Ice::Current());
		void stopSetPosBase(const Ice::Current& = Ice::Current());
		
		void setWorldAngle(Ice::Float, Ice::Float, const Ice::Current&){};
		void setRobotAngle(Ice::Float, Ice::Float, const Ice::Current&){};

        // Public Functions
        BaseHandler *getBaseHandler();

		QMutex *mutex;
    private:
        // Component private data
        BaseHandler *_bh;
     
    public slots:
};

#endif
