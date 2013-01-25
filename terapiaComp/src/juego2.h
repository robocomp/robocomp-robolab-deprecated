/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef JUEGO2_H
#define JUEGO2_H
#include "juego.h"
#include <qmatriz/QMatAll>
#include <QtGui>

class Juego2: public Juego
{
Q_OBJECT
  private:
	osg::ref_ptr<osg::Camera> cam;
	osg::ref_ptr<osgART::Tracker> tracker;
	osg::ref_ptr<osgART::Marker> marker;
	osg::ref_ptr<osgART::Marker> marker2;
	osg::ref_ptr<osg::MatrixTransform> arTransform;
	osg::ref_ptr<osg::MatrixTransform> arTransform2;
	
	RoboCompArmrocio::ArmrocioPrx arm;
	RoboCompSpeech::SpeechPrx speech;
	int mThreshold;
	bool arriba;
	
// 	float X,Y,Z, Xb,Yb,Zb;
	osg::Vec3 pBolaVerde, pBolaRoja;
	osg::Vec4 verde; 
	osg::Vec4 rojo; 
	
	//panel de control en el frame del worker
	QHBoxLayout *hLUmbral,*hLArm;
	QVBoxLayout *vLayout;
	QLCDNumber *lcdUmbral;
		
	QSlider *sliderUmbral;		
	QLabel *lUmbral,*lArm;	
	
	osg::Geode* ball (float size, osg::Vec4 color  = osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	osg::Geode* testCube(float size = 80.0f, osg::Vec4 color = osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	
	public:
		QRadioButton *rbArriba, *rbReposo,*rbAbajo, *rbBucle;
		QPushButton *pbFijaVerde, *pbFijaRoja,*pbConseguido,*pbBinarize;
	 
		Juego2 ( osg::ref_ptr< osg::Camera > _cam, osg::ref_ptr< osgART::Tracker > _tracker, RoboCompArmrocio::ArmrocioPrx _arm, RoboCompSpeech::SpeechPrx _speech, QFrame* parent = 0);
		void changePosition ( );	
		void computeGame();
		void updateBola(osg::Vec3 centro, osg::Vec4 color);
		QVec fijador ( );
		~Juego2 ();
	public slots:		
		void conseguido (bool );
		void vSlider (int );
		void fijaVerde (bool );
		void fijaRoja  (bool );
		void armArriba (bool );
		void armReposo (bool );
		void armAbajo (bool );
		void armLoop(bool);
};

#endif // JUEGO1_H
