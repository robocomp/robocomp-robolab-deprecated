/* -*-c++-*- 
 * 
 * osgART - ARToolKit for OpenSceneGraph
 * Copyright (C) 2005-2008 Human Interface Technology Laboratory New Zealand
 * 
 * This file is part of osgART 2.0
 *
 * osgART 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * osgART 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with osgART 2.0.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OSGART_H
#define OSGART_H

#include <QtGui>
#include <QtOpenGL/QGLWidget>
#include <QTimer>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/GraphicsWindow>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgDB/ReadFile>
#include <osg/Material>
#include <osg/Image>

#include <QMat/QMatAll>
#include <innermodel/innermodel.h>

#include <Camera.h>
#include <Armrocio.h>

#include "juego1.h"
#include "juego2.h"

using Qt::WindowFlags;


class OsgArt : public QGLWidget, public osgViewer::Viewer
{
Q_OBJECT

  
  private:
	osg::ref_ptr<osg::Group> root;
	osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;

	osg::ref_ptr<osg::Image> osgImage;
	osg::ref_ptr<osgART::Tracker> tracker;
	osg::ref_ptr<osgART::Calibration> calibration;

	osg::ref_ptr<osg::Group> videoBackground;
	osg::ref_ptr<osg::Camera> cam;
	Juego *juego;
	
	
	RoboCompCamera::CameraPrx camera;
	RoboCompCamera::TCamParams camParams;
	RoboCompCamera::imgType imgV;
	RoboCompDifferentialRobot::TBaseState bState;
	RoboCompCommonHead::THeadState hState;
	int imgSize, mThreshold;
	bool arriba,binarizar;
	Ipp8u *bufferImgResize;
	IppiSize bufferImgSize;
	Ipp8u *bufferImgResizeGray;
	
  protected:
	QTimer timer;
 
public:    
  OsgArt( RoboCompCamera::CameraPrx _camera,  QWidget* parent=0, const QGLWidget* shareWidget = 0, WindowFlags f = 0);
  void paintGL();
  void init();
  void update();
  osgViewer::GraphicsWindow* getGraphicsWindow() { return _gw.get(); }
  osg::Geode* ball (float size, osg::Vec4 color = osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
  void setJuego (QString name, QFrame* parentFrame, RoboCompArmrocio::ArmrocioPrx arm, RoboCompSpeech::SpeechPrx speech);

  ~OsgArt();  
  
  uchar *data;
  
  
public slots:
	void resizeOsgArt ();  
	void binarize();  
	void closeOsgArt( );
	void speechConseguido();

signals:
	void signalSpeechConseguido();
};

#endif
