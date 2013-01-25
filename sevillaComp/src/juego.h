/*
 *    Copyright (C) 2010 by RoboLab - University of Extremadura
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef JUEGO_H
#define JUEGO_H

#include <osg/Camera>
#include <QtCore>
#include <osg/MatrixTransform>

#include <osgART/Marker>
#include <osgART/Tracker>
#include <osgART/Foundation>
#include <osgART/VideoLayer>
#include <osgART/PluginManager>
#include <osgART/VideoGeode>
#include <osgART/Utils>
#include <osgART/MarkerCallback>
#include <osgART/TransformFilterCallback>

#include <osgDB/FileUtils>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/Notify>
#include <osg/Matrix>
#include <osg/Texture>
#include <osg/Texture2D>
#include <osg/TextureRectangle>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>

#include <Armrocio.h>
#include <Speech.h>


class Juego : public QObject 
{
Q_OBJECT
public:
    Juego():QObject (){};
    virtual ~Juego(){};
	
	virtual void computeGame()=0;
	void pose(osg::Vec3 pos) {std::cout<<"x: "<<pos.x()<<" y: "<<pos.y()<<" z: "<<pos.z()<<std::endl;};
};

#endif