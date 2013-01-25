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
#include "worker.h"
/**
* \brief Default constructor
*/
Worker::Worker(RoboCompKinect::KinectPrx kinectprx, QObject *parent) : Ui_evaluacionForm()
{
	kinect = kinectprx;
	mutex = new QMutex();
	ippSetNumThreads(1);
	setupUi(this);
	imgAlto=480;imgAncho=640;
	maxDepth=10.0;minDepth=0.5;
	ippSizeImage.height=imgAlto;
	ippSizeImage.width=imgAncho;
	
	qImgRGB = new QImage(imgAncho,imgAlto,QImage::Format_RGB888 );
	qImgRGBZ = new QImage(imgAncho,imgAlto,QImage::Format_RGB888 );
	qImgZ = new QImage(imgAncho,imgAlto,QImage::Format_Indexed8 );
	imgZ =ippsMalloc_8u(imgAlto*imgAncho);
	

	rcdRGB =  new RCDraw (imgAncho,imgAlto,qImgRGB,this->frameRGB);
	rcdRGBZ =  new RCDraw (imgAncho,imgAlto,qImgRGBZ,this->frameRGBZ);
	rcdZ =  new RCDraw (imgAncho,imgAlto,qImgZ,this->frameZ);
	roiGrab.setCoords(0,0,640,480);
	grab=false;capture=false;
	
	show();
	connect(&timer, SIGNAL(timeout()), this, SLOT(compute()));
	connect(rcdZ,SIGNAL(newCoor(QPointF)),this,SLOT(mousePress(QPointF)));
	connect(rcdZ,SIGNAL(endCoor(QPointF)),this,SLOT(mouseRelase(QPointF)));
	Period = BASIC_PERIOD;
	timer.start(Period);
}
void Worker::writeFile(QRect r)
{
	file.open(nameFile.toStdString().c_str(),ofstream::out | ofstream::app);
	
		for(int j=r.y();j<r.y()+ r.height();j++)
		{
				for(int i=r.x();i<r.x()+r.width();i++)
				{
					file << z_vectorIR[j*imgAncho+i] <<" ";
				}
				file << std::endl;
		}
       file << std::endl<<std::endl;       
	file.close();
}

void Worker::on_pbCapture_clicked()
{
	if (capture==false) {
		dialog.setDirectory(QDir::currentPath());
		nameFile= dialog.getOpenFileName(this,"open file");		
		qDebug()<<nameFile;
		file.open(nameFile.toStdString().c_str(),ofstream::out | ofstream::trunc);
	}
	else {
		file.close();
	}
	
	capture=!capture;
}

void Worker::compute( )
{
	rcdRGB->drawLineOnTop (QLine(imgAncho/2,0,imgAncho/2,imgAlto),Qt::darkRed,2);
	rcdRGB->drawLineOnTop (QLine(0,imgAlto/2,imgAncho,imgAlto/2),Qt::darkRed,2);
	rcdRGBZ->drawLineOnTop (QLine(imgAncho/2,0,imgAncho/2,imgAlto),Qt::green,2);
	rcdRGBZ->drawLineOnTop (QLine(0,imgAlto/2,imgAncho,imgAlto/2),Qt::green,2);
	rcdZ->drawLineOnTop (QLine(imgAncho/2,0,imgAncho/2,imgAlto),Qt::white,2);
	rcdZ->drawLineOnTop (QLine(0,imgAlto/2,imgAncho,imgAlto/2),Qt::white,2);
	
	try{
	kinect->getDataRGBZinRGB(imgRGB,z_vector,hState,bState);
	kinect->getDataRGBZinIR(imgRGB,z_vectorIR,hState,bState);
	memcpy(qImgRGB->bits(),&imgRGB[0], imgAncho*imgAlto*3);
	QApplication::processEvents();
	if (capture)
	{
		writeFile(roiGrab);
		sbCapturas->setValue(sbCapturas->value()-1);
		if(sbCapturas->value() <= 0)
		{
			capture = false;
			pbCapture->setChecked(false);
		}
	}
	int indice=0;
	for (int x=0; x <imgAncho; x++ )
	{
		for (int y=0; y <imgAlto; y++ )
		{	indice =y*imgAncho+x;
			if (z_vector[indice]<0.2  )
			{
				imgRGB[indice*3]=255;
				imgRGB[indice*3+1]=0;
				imgRGB[indice*3+2]=0;
				
			}
			else 
			{
				if (z_vector[indice]<minDepth or z_vector[indice]>maxDepth )
				{
					imgRGB[indice*3]=0;
					imgRGB[indice*3+1]=0;
					imgRGB[indice*3+2]=0;
				}
			}
			if (z_vectorIR[indice]<0.2  ){
				imgZ[indice]=0;
			}
			else {
			uint color =  (z_vectorIR[indice]/maxDepth) *255;
			imgZ[indice]=color;
			}
		}
	}
	
	memcpy(qImgRGBZ->bits(),&imgRGB[0], imgAncho*imgAlto*3);
	memcpy(qImgZ->bits(),&imgZ[0], imgAncho*imgAlto);
	}
	catch ( Ice::Exception e ) {
		qDebug()<<"Error talking to kinect: "<<e.what();
	}
	if (grab)
		rcdZ->drawSquare(roiGrab,Qt::white);
	rcdRGB->update();rcdZ->update();rcdRGBZ->update();
}

void Worker::mousePress(QPointF p)
{
	grab=false; 
	roiGrab.setTopLeft(QPoint(p.x(),480-p.y()));
	qDebug()<<roiGrab.topLeft()<<"press";
}

void Worker::mouseRelase (QPointF p)
{
	grab=true; 
	qDebug()<<p<<"relase";
	if(cbColumna->isChecked())
		roiGrab.setBottomRight(QPoint(roiGrab.topLeft().x(),p.y()));
	else
		roiGrab.setBottomRight(p.toPoint());
}


/**
* \brief Default destructor
*/
Worker::~Worker()
{
	ippsFree(imgZ);
}
void Worker::killYourSelf()
{
	rDebug("Killing myself");
	emit kill();
	exit(1);
}
/**
* \brief Change compute period
* @param per Period in ms
*/
void Worker::setPeriod(int p)
{
	rDebug("Period changed"+QString::number(p));
	Period = p;
	timer.start(Period);
}
