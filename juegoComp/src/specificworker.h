/*
 *    Copyright (C) 2006-2010 by RoboLab - University of Extremadura
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
#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <rcdraw/rcdraw.h>
#include <osgviewer/osgview.h>
#include <osg/io_utils>

#include <innermodel/innermodelviewer.h>
#include <innermodel/innermodel.h>

#include <etapa.h>

/**
       \brief
       @author authorname
*/

class SpecificWorker : public GenericWorker
{
Q_OBJECT
protected:
     void closeEvent(QCloseEvent *event);

private:
	
	int visible,lastVisible;
	QVec cesta, arbol, manzana;
	
  
	OsgView *osg;
	InnerModel *innerModel;
	InnerModelViewer *imv;
	int osgWidth,osgHeight;
	
	
	int alto, ancho;
	
	bool juegoCargado;
	int numEtapaActual;
	Etapa* etapaActual;
	QList<Etapa*> listaEtapas;
	QString nombreObjetivo;
	
	RCDraw *drawRGB;	
	RoboCompTrackeropenni::imgType imgTypeRGB;
	RoboCompJuego::imgType imgTypeAR;
	RoboCompTrackeropenni::RTMatrixList RTMatrixMap;	
	RoboCompTrackeropenni::jointListType jointList;
	RoboCompTrackeropenni::trackingState stateTrack;
	QMutex *mutexImgAR,*mutexJointList,*mutexRTMatrixMap;
	
	///fichero volcado juego
	std::ofstream dumpFile;
	bool dump;	
	bool evaluador;	
	osg::ref_ptr<osg::Image> osgImage;    
			
	void updateJoints ();	

	 ///utils   
	///0 x, 1 y, 2 z translation values
	QVec stdToQVecTranslation(std::vector<float> v){
		QVec qv (3);
		qv[0]=v[0];qv[1]=v[1];qv[2]=v[2];
		return qv;
	}	
	///3 rx, 4 ry, 5 rz rotation values
	QVec stdToQVecRotation(std::vector<float> v){
		QVec qv (3);
		qv[0]=v[3];qv[1]=v[4];qv[2]=v[5];
		return qv;
	}
    void reloadJuego();
    void poseOpenNi();
    void poseLimpia();
	

public:
	SpecificWorker(MapPrx& mprx);	
	~SpecificWorker();
	void setParams(RoboCompCommonBehavior::ParameterList params);
	void switchFrame();
	void  getARimg(imgType& img, int& width, int& height);
    bool setJuego(const string& text);
	bool saveFile(const std::string& name,const string& header);
	void  getJointsPosition(jointListType& myJointList);
	void  getRTMatrixList(RTMatrixList& myRTMatList);
	void setGameRange(string game,float min,float max);
	void getGameRange(string game, float& min, float& max);
	void setSceneObjectPosition(const std::string& object, int x, int y,int z);
	void getSceneObjectPosition(const std::string& object,int &x,int &y,int &z);
	
	void  stopRecording();
	void  playVideo(const string& path);
	void  makeVideoVisible();
	void  makeARVisible();
	void  stopVideo();
	void  pauseVideo();
	void  playAr();
    void resizeEvent(QResizeEvent* event);
	
	
	
public slots:
 	void compute();
	void keyAction(QString key);
    QString stateTrackToQstring(const RoboCompTrackeropenni::trackingState s);
    
    
	///AÑADIDO
	void invisible();
};

#endif