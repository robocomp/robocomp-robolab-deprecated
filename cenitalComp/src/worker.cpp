#include "worker.h"

Worker::Worker(RoboCompCamera::CameraPrx _cameraPrx, QWidget *parent)
 : QWidget(parent) , cameraPrx(_cameraPrx)
{
	setupUi(this);
	show();
	ippSetNumThreads(1);
	RoboCompVision::TCamParams paramsLocal;
	//Read params from camera and add local params to RoboCompVision::TCamParams
	try
	{
		params = cameraPrx->getCamParams();
		qDebug() << "Reading cameraComp parameters...";
		qDebug() << "Focal: " << params.focal;
		qDebug() << "Width " <<  params.width;
		qDebug() << "Height " <<  params.height;
		qDebug() << "Size " <<  params.size;
		qDebug() << "NumCams " << params.numCams;
		qDebug() << "LeftCam " <<  params.leftCamera;
		qDebug() << "RightCam " <<  params.rightCamera;
		qDebug() << "BothCams " <<  params.bothCameras;
		paramsLocal.focal = params.focal;
		paramsLocal.width = params.width;
		paramsLocal.height = params.height;
		paramsLocal.numCams = params.numCams;
		paramsLocal.pyrLevels = PIRNIVELES;
		paramsLocal.radius = RADIUS;
		paramsLocal.size = params.size;
		paramsLocal.leftCamera = params.leftCamera;
		paramsLocal.rightCamera = params.rightCamera;
		paramsLocal.bothCameras = params.bothCameras;
		paramsLocal.FPS = params.FPS;
	}
	catch(const Ice::Exception& ex)
	{
		qFatal( "CenitalComp: Exception talking to CameraComp. Aborting..." );
	}

	//imagen = ippiMalloc_8u_C1( params.width, params.height, &step);
	
	auxRGB =  ippsMalloc_8u( params.size*3);
	auxPlanos[0] = ippsMalloc_8u( params.size);
	auxPlanos[1] = ippsMalloc_8u( params.size);
	auxPlanos[2] = ippsMalloc_8u( params.size);

	//VisionClass
// 	imgROI = new uchar[AREA_ROBOT];
// 	roiProcess = new roiDetector(paramsLocal,0);

	//Segmentate
// 	imgSeg = new uchar[params.size];

// 	imgROI = new uchar[AREA_ROBOT*4];
// 	img8 = new uchar[params.size];

	//Visor grande
	//  	visor = new QVision(params.width, params.height,auxRect);
	//  	visor->setParent(this);
	//  	visor->move(frame->x(), frame->y());
	// 	visor->show();

	img_vector.resize(4);
	
	
	
	
	
	img1 = new QImage(640,480,QImage::Format_RGB888);
	auxRect = ippsMalloc_8u( params.size*3);
	img2 = new QImage(640,480,QImage::Format_RGB888);
	img3 = new QImage(640,480,QImage::Format_RGB888);
	img4 = new QImage(640,480,QImage::Format_RGB888);
	imgG = new QImage(640*2,480*2,QImage::Format_RGB888);
	imgR = new QImage(800,600,QImage::Format_RGB888);

	visor1 = new qWorld(640, 480, img1, frame1);
	visor2 = new qWorld(640, 480, img2, frame2);
	visor3 = new qWorld(640, 480, img3, frame3);
	visor4 = new qWorld(640, 480, img4, frame4);
	
	//Mouse
	mousePressed = false;
	newRoiBool = false;

	RHOX = -0.00013;
	sbRhoX->setValue((int)(RHOX*10000));
	RHOY = -0.0007;
	sbRhoY->setValue((int)(RHOY*10000));
	KHAX = 0.005;
	sbKhaX->setValue((int)(KHAX*1000));
	KHAY = 0.002;
	sbKhaY->setValue((int)(KHAY*1000));
	FOCAL = 100.;

	iniDistortion();

	connect(visor1, SIGNAL( mouseClickSignal(QPoint)), this, SLOT(mouseClickSignal(QPoint)));
	connect(visor1, SIGNAL( mouseReleaseSignal(QPoint)), this, SLOT(mouseReleaseSignal(QPoint)));
	connect(visor1, SIGNAL( mouseMovementSignal(QPoint)), this, SLOT(mouseMovementSignal(QPoint)));
	connect(pbCross, SIGNAL( toggled(bool)), this, SLOT(crossToggled(bool)));
	connect(&timer, SIGNAL(timeout()), this, SLOT(compute()));
	connect(&trecord,SIGNAL(timeout()),this,SLOT(record()));
 	pBarVar->setMaximum(MAX_VAR_ALLOWED);
//
// 	connect(qvisor, SIGNAL(newRoi(QRect)), this, SLOT(newTrackWin(QRect)));
	connect(rbC1,SIGNAL(clicked()),this,SLOT(changeImagen()));
	connect(rbC2,SIGNAL(clicked()),this,SLOT(changeImagen()));
	connect(rbC3,SIGNAL(clicked()),this,SLOT(changeImagen()));
	connect(rbC4,SIGNAL(clicked()),this,SLOT(changeImagen()));
	
	
 	timer.start(BASIC_PERIOD);
// 	timer.setSingleShot(true);
//  	timer.start(100);
	imagenChange = 0;
	qDebug() << "End Worker Constructor";
	
}

Worker::~Worker()
{
}


void Worker::compute( )
{
	static RoboCompVision::TRoi roi;
	static RoboCompVision::ROIPyrVectors measuredList;
	static IppiSize roisize = { params.width/32, params.height/32};
	//static IppiRect srcRobot = { 0,0, params.width/32, params.height/32 };
	//static IppiRect destRobot = { 0,0,params.width/32, params.height/32};

	try
	{
		cameraPrx->getRGBPackedImage(0, imagenV, hState, bState);	
		memcpy(img3->bits(), &imagenV[0], params.size * 3);
		cameraPrx->getRGBPackedImage(1, imagenV, hState, bState);	
		memcpy(img2->bits(), &imagenV[0], params.size * 3);
		cameraPrx->getRGBPackedImage(2, imagenV, hState, bState);	
		memcpy(img4->bits(), &imagenV[0], params.size * 3);
		cameraPrx->getRGBPackedImage(3, imagenV, hState, bState);	
		memcpy(img1->bits(), &imagenV[0], params.size * 3);
		
		ippiRemap_8u_C3R(img3->bits(), roiSize, params.width*3, rect, img_vector[2].xMap, img_vector[2].xStep, img_vector[2].yMap, img_vector[2].yStep, auxRect, params.width*3, roiSize, IPPI_INTER_LINEAR);
		memcpy(img3->bits(), auxRect, params.size * 3);
		ippiRemap_8u_C3R(img2->bits(), roiSize, params.width*3, rect, img_vector[1].xMap, img_vector[1].xStep, img_vector[1].yMap, img_vector[1].yStep, auxRect, params.width*3, roiSize, IPPI_INTER_LINEAR);
		memcpy(img2->bits(), auxRect, params.size * 3);
		ippiRemap_8u_C3R(img4->bits(), roiSize, params.width*3, rect, img_vector[3].xMap,img_vector[3].xStep, img_vector[3].yMap, img_vector[3].yStep, auxRect, params.width*3, roiSize, IPPI_INTER_LINEAR);
		memcpy(img4->bits(), auxRect, params.size * 3);
		ippiRemap_8u_C3R(img1->bits(), roiSize, params.width*3, rect, img_vector[0].xMap, img_vector[0].xStep, img_vector[0].yMap, img_vector[0].yStep, auxRect, params.width*3, roiSize, IPPI_INTER_LINEAR);
		memcpy(img1->bits(), auxRect, params.size * 3);
		
		//ippiYUV422ToRGB_8u_C2C3R(&imagenV[0],params.width*2,auxRGB,params.width*3,roiSize);
		
		//ippiMirror_8u_C3IR(auxRGB, params.width*3, roiSize, ippAxsBoth);
		
		//ippiCopy_8u_C3P3R(auxRect,params.width*3,auxPlanos,params.width,roiSize);

		// 		roiProcess->procesa(img8, measuredList);
		// 		drawRegions(measuredList,Qt::green,3,3);
		// 
	}
	catch(const Ice::Exception& ex)
	{
		qDebug() << "Cenital: Generic exception talking to CameraComp. ";
		timer.setInterval(CHECK_PERIOD);
		return;
	}
	//pBarFPS->setValue( printFPS() );
	timer.setInterval(BASIC_PERIOD);
	//widgetExtras();
	visor1->update();
	visor2->update();
	visor3->update();
	visor4->update();
}



void Worker::iniDistortion()
{
	Ipp32f *xMap, *yMap;
	int xStep,yStep;
	roiSize.width = params.width;
	roiSize.height = params.height;
	int pBufsize;
	ippiUndistortGetSize(roiSize, &pBufsize);
	bufUndist = ippsMalloc_8u(pBufsize);
	rect.x = 0;
		rect.y = 0;
		rect.width = roiSize.width;
		rect.height = roiSize.height;
		
	for(int i=0;i<4;i++)
	{
		xMap = ippiMalloc_32f_C1(roiSize.width, roiSize.height, &xStep);
		yMap = ippiMalloc_32f_C1(roiSize.width, roiSize.height, &yStep);
		
		ippiCreateMapCameraUndistort_32f_C1R (xMap, xStep, yMap, yStep,roiSize,FOCAL, FOCAL, params.width/2., params.height/2., RHOX, RHOY, KHAX, KHAY, bufUndist);
		img_vector[i].xMap = xMap;
		img_vector[i].xStep = xStep;
		img_vector[i].yMap = yMap;
		img_vector[i].yStep = yStep;	
	}
}


void Worker::newTrackWin(QRect win)
{

	xRobot=win.x()+win.width()/2;
	yRobot=win.y()+win.height()/2;

	wRobot=(win.width()/2)*2+1;
	hRobot=(win.height()/2)*2+1;

	qDebug()<<"(x,y)"<<xRobot<<yRobot<<"(w,h)"<<wRobot<<hRobot;

	//tracker->trackWin(xRobot, yRobot, wRobot, hRobot);

}

inline int Worker::printFPS( )
{
	static int fps=0;
	static QTime ti;
	if ((++fps % 50) == 0)
	{
		int t = 50000 / ti.restart();
		qDebug()  << " CenitalComp fps: " << t;
		return t;
	}
	return 0;
}

// void Worker::segmentateColor(uchar *planosRGB[3], uchar *imgSeg, QColor c)
// {
// 	int i,j,ind;
// 	float mod, rn, gn, bn;
// 	static IppiSize roiSize = { params.width, params.height};
// 	c=c;
// 	//Ponemos a cero la imagen de segmentación
// 	ippiSet_8u_C1R(0, imgSeg, params.width, roiSize);
// 
// 	//Normalizamos y umbralizamos
// 	for(i=0; i<params.width;i++)
// 	{
// 		for(j=0;j<params.height;j++)
// 		{
//  			ind = j+i*params.height;
// 
//  			mod = sqrt((float)planosRGB[0][ind]*planosRGB[0][ind] + (float)planosRGB[1][ind]*planosRGB[1][ind] + (float)planosRGB[2][ind]*planosRGB[2][ind]);
//  			if( fabs(mod) >  0.00001)
//  			{
//  				rn = auxPlanos[0][ind] / mod;
//  				gn = auxPlanos[1][ind] / mod;
//  				bn = auxPlanos[2][ind] / mod;
//  			}
//  			else
//  				rn = gn = bn = 0.;
// 			if (c==Qt::red)
// 				if(((rn-gn) > 0.12) && (rn-bn)>0.12 )//&& (rn>0.7) && (gn<0.4) && (bn<0.4))
//  					imgSeg[ind] = 128;
//  			if (c==Qt::black)
// 				if(((bn < 0.6) && (rn < 0.6) && gn < 0.6))
//  					imgSeg[ind] = 128;
// 			if (c==Qt::blue)
// 				if(((bn-rn > 0.3) && bn-gn > 0.3))
//  					imgSeg[ind] = 128;
// 		}
// 	}
// }
// 
// 
// void Worker::segmentate(int &cxMax, int &cyMax)
// {
// 	int i,j,k,l, ind;
// 	float mod,rn,bn,gn;
// 	int centroRojosX=0,centroRojosY=0,rojos=0;
// 	int max = 5;
// 	cxMax=100;cyMax=100;
// 	int semiancho = LADO_ROBOT/2;
// 
// 	for(i=LADO_ROBOT; i<params.width-LADO_ROBOT;i+=semiancho/2)
// 	{
// 		for(j=LADO_ROBOT;j<params.height-LADO_ROBOT;j+=semiancho/2)
// 		{
// 			centroRojosX=0,centroRojosY=0,rojos=0;
// 			for(k=i;k<i+LADO_ROBOT;k++)
// 			{
// 				for(l=j;l<j+LADO_ROBOT;l++)
// 				{
// 					ind = l+k*params.width;
// 					mod = sqrt((float)auxPlanos[0][ind]*auxPlanos[0][ind] + (float)auxPlanos[1][ind]*auxPlanos[1][ind] + (float)auxPlanos[2][ind]*auxPlanos[2][ind]);
// 					rn = auxPlanos[0][ind] / mod;
// 					gn = auxPlanos[1][ind] / mod;
// 					bn = auxPlanos[2][ind] / mod;
// 					if(((rn-gn) > 0.1) && (rn-bn)>0.1)
// 					{
// 						rojos++;
// 						centroRojosX += l;
// 						centroRojosY += k;
// 					}
// 				}
// 			}
// 			centroRojosX /= rojos;
// 			centroRojosY /= rojos;
// 			if(rojos>max && centroRojosX < params.width && centroRojosY < params.height)
// 			{
// 				max = rojos;
// 				cxMax = centroRojosX;
// 				cyMax = centroRojosY;
// 				//qDebug() << centroRojosX << centroRojosY << rojos;
// 			}
// 		}
// 	}
// 
// }
// 
// void Worker::iniCanny(int sx, int sy )
// {
// 
// 	srcSize.width = sx;
// 	srcSize.height = sy;
// 	img16 = ippsMalloc_16s(sx*sy);
// 	SobelHorz = ippsMalloc_16s(sx*sy);
//  	SobelVert = ippsMalloc_16s(sx*sy);
// 	int tam;
// 	IppiSize size = { sx,sy};
// 	ippiCannyGetSize(size, &tam);
// 	tmpbuf = ippsMalloc_8u(tam);
// 	width16 = sx * sizeof(Ipp16s);
// }
// 
// void Worker::cannyFilterRegion(uchar *imgIn, uchar *imgOut, int cx, int cy, int sx, int sy)
// {
// 
//  	ippiConvert_8u16s_C1R(imgIn+(params.width*(cy-sy/2)+(cx-sx/2)), params.width, img16, width16 , srcSize);
// 	ippiFilterSobelVert_16s_C1R(img16, width16, SobelHorz, width16, srcSize);
//  	ippiFilterSobelHoriz_16s_C1R(img16, width16, SobelVert, width16, srcSize);
//  	ippiCanny_16s8u_C1R(SobelHorz, width16, SobelVert, width16, imgOut, sx, srcSize, MINCANNY, MAXCANNY, tmpbuf);
// }
// 
// 
// void Worker::drawRegions( RoboCompVision::ROIPyrVectors regionList , QColor c, int levmin, int levmax)
// {
// 	uint j;
// 
// 	if(levmin < 0) levmin = 0;
// 	if((uint)levmax > regionList.size()-1) levmax=regionList.size()-1;
// 	if((uint)levmin > regionList.size()-1) levmax=regionList.size()-1;
// 	if(levmax < 0) levmax=regionList.size()-1;
// 
// 	qreal r,g,b,a;
// 	c.getRgbF(&r,&g,&b,&a);
// 	visor->setColor( (double)r,(double)g,(double)b);
// 	for(int level=levmin; level<=levmax; level++)
// 	{
// 		for(j=0; j<regionList[level].size(); j++)
// 			visor->renderPlainSquare(regionList[level][j].xBase, regionList[level][j].yBase, regionList[level][j].size, 1.);
// 	}
// }
// 
// void Worker::drawRegions( RoboCompVision::TRoi region, const QColor c )
// {
// 	qreal r,g,b,a;
// 	c.getRgbF(&r,&g,&b,&a);
// 	visor->setColor( (double)r,(double)g,(double)b);
// 	visor->renderPlainSquare(region.xBase, region.yBase, region.size, 1.);
// }
// 
// void Worker::saliencyOfRegions( RoboCompVision::ROIPyrVectors &regionList,  RoboCompVision::TRoi &roi , int &sal )
// {
// 	int s, size, xBase, yBase, cRes, maxR = -9999;
// 	IppiSize localSize;
// 
// 	s = regionList.size();
// 	if ( s >0)
// 	{
//     	for(uint i=0; i<regionList[s-1].size();i++)
// 		{
// 			size = regionList[s-1][i].size;
// 			xBase = regionList[s-1][i].xBase;
// 			yBase = regionList[s-1][i].yBase;
// 			localSize.width = params.width;
// 			localSize.height = params.height;
// 			ippiCountInRange_8u_C1R( imgSeg+params.width*(yBase-size/2)+(xBase-size/2), params.width, localSize, &cRes, 1, 255);
// 			if( cRes > maxR)
// 			{
// 				maxR = cRes;
// 				roi = regionList[s-1][i];
// 			}
// 		}
// 		qDebug() << "max" << maxR;
// 		sal = maxR;
// 	}
// }
// 
// 
// bool Worker::guess( QString s, uchar *planosRGB[],  RoboCompVision::ROIPyrVectors &regionList, RoboCompVision::TRoi & roi, QColor color )
// {
// 	s=s;
// 	segmentateColor(planosRGB, imgSeg, color);
// 	roiProcess->procesa(imgSeg, regionList);
// 	int sal;
// 	saliencyOfRegions(regionList, roi, sal);
// 	if (sal < 800) return false;
// 	else return true;
// 
// }

///Slots

void Worker::on_sbRhoX_valueChanged(int val)
{
	RHOX = (float)(val/10000.);
	lcdRX->display(RHOX);
	ippiCreateMapCameraUndistort_32f_C1R (img_vector[imagenChange].xMap,img_vector[imagenChange].xStep,img_vector[imagenChange].yMap,img_vector[imagenChange].yStep,roiSize,FOCAL, FOCAL, params.width/2., params.height/2., RHOX, RHOY, KHAX,  KHAY, bufUndist);

	qDebug()<<"imagen 0"<<img_vector[0].xMap<<img_vector[0].xStep<<imagenChange; 
qDebug()<<"imagen 2"<<img_vector[2].xMap<<img_vector[2].xStep;

}

void Worker::on_sbRhoY_valueChanged(int val)
{
	RHOY = (float)(val/10000.);
	lcdRY->display(RHOY);
	ippiCreateMapCameraUndistort_32f_C1R (img_vector[imagenChange].xMap,img_vector[imagenChange].xStep,img_vector[imagenChange].yMap,img_vector[imagenChange].yStep,roiSize,FOCAL, FOCAL, params.width/2., params.height/2., RHOX, RHOY, KHAX,  KHAY, bufUndist);
}

void Worker::on_sbKhaX_valueChanged(int val)
{
	KHAX = (float)(val/1000.);
	lcdKX->display(KHAX);
	ippiCreateMapCameraUndistort_32f_C1R (img_vector[imagenChange].xMap,img_vector[imagenChange].xStep,img_vector[imagenChange].yMap,img_vector[imagenChange].yStep,roiSize,FOCAL, FOCAL, params.width/2., params.height/2., RHOX, RHOY, KHAX,  KHAY, bufUndist);
	
}

void Worker::on_sbKhaY_valueChanged(int val)
{
	KHAY = (float)(val/1000.);
	lcdKY->display(KHAY);
	ippiCreateMapCameraUndistort_32f_C1R (img_vector[imagenChange].xMap,img_vector[imagenChange].xStep,img_vector[imagenChange].yMap,img_vector[imagenChange].yStep,roiSize,FOCAL, FOCAL, params.width/2., params.height/2., RHOX, RHOY, KHAX,  KHAY, bufUndist);
}

void Worker::mouseClickSignal(QPoint p)
{
	mousePressed = true;
	iniPoint = p;
	currentPoint = p;
	int i = p.x()*params.height + p.y();
	qDebug() << auxPlanos[0][i] << auxPlanos[1][i] << auxPlanos[2][i];

	newRoi.xBase = p.x();
	newRoi.yBase = p.y();
	newRoi.xBaseCenter = (int)s2cX(p.x());
	newRoi.yBaseCenter = (int)s2cY(p.y());

	newRoiBool=true;
//	timer.start(100);
}

void Worker::mouseReleaseSignal(QPoint p)
{
	p=p;
	mousePressed = false;

}

void Worker::mouseMovementSignal(QPoint p)
{
	currentPoint = p;
	lcdX->display(p.x());
	lcdY->display(p.y());
	lcdLength->display(sqrt(pow(p.x()-iniPoint.x(), 2) + pow(p.y()-iniPoint.y(), 2)));
}

void Worker::crossToggled(bool )
{
	visor1->setMouseTracking(pbCross->isChecked());
}

void Worker::on_pbResetK_clicked( )
{
	
}

// void Worker::widgetExtras()
// {
// 	if(mousePressed)
// 		visor->renderSegment(iniPoint.x(), iniPoint.y(), currentPoint.x(), currentPoint.y(), 2.);
// 	if(pbCross->isChecked())
// 	{
// 		visor->renderSegment(0, currentPoint.y(), params.width, currentPoint.y(), 1.);
// 		visor->renderSegment(currentPoint.x(), 0, currentPoint.x(), params.height, 1.);
// 	}
// 
// }

void Worker::on_hsQ_valueChanged( int val )
{
// 	float v = val*(5./500.);
// 	lcdQ->display(v);
// 	QMat q(Q);
// 	q &= v;
// 	kalman->setRQ(R,q);
}

void Worker::on_hsR_valueChanged( int val )
{
// 	float v=val*(5./500.);
// 	lcdR->display(v);
// 	QMat r(R);
// 	r &= v;
// 	kalman->setRQ(r,Q);

}

int Worker::c2sX( float n )
{
	return (int)(n + params.width/2);
}

int Worker::c2sY( float n )
{
	return (int)(params.height/2 - n);
}
float Worker::s2cX( int n )
{
	return (int)(n - params.width/2);
}

float Worker::s2cY( int n )
{
	return (int)(-n + params.height/2 );
}


//Aux for extracting regions
// void Worker::extractRoi( uchar *orig, int x, int y, int width, uchar *dest, IppiSize & cs)
// {
// 	ippiCopy_8u_C1R(img8 + ((y-cs.height/2)*width + x-cs.width/2), width, dest, cs.width ,cs);
// 
// }

// void Worker::on_hsMinCanny_valueChanged( int v )
// {
// 	MINCANNY = v;
// }
// 
// void Worker::on_hsMaxCanny_valueChanged( int v )
// {
// 	MAXCANNY = v;
// }
// 


// void Worker::compSimilarity(uchar *bordesImg, uchar *projImg)
// {
// 	//Invertimos para la DistTrans. 0's son las buenas
// 	ippiNot_8u_C1IR(proyImg, LADO_ROBOT*2, srcSize);
// 	//DistTrans
// 	ippiDistanceTransform_3x3_8u_C1IR( proyImg, LADO_ROBOT*2 , srcSize, pMetrics);
// // void Worker::compSimilarity(uchar *bordesImg, uchar *projImg)
// {
// 	//Invertimos para la DistTrans. 0's son las buenas
// 	ippiNot_8u_C1IR(proyImg, LADO_ROBOT*2, srcSize);
// 	//DistTrans
// 	ippiDistanceTransform_3x3_8u_C1IR( proyImg, LADO_ROBOT*2 , srcSize, pMetrics);
//
// 	//Invertimos de nuevo
// 	//ippiNot_8u_C1IR(imgAux, camParams.width, srcSize);
//
// 	//Umbralizamos para quedarnos con una franja
// 	//ippiThreshold_LTVal_8u_C1IR(imgAux,camParams.width, srcSize, 250, 0);
// 	//Contamos cuántos on tiene el modelo
// /*	int counts;
// 	ippiCountInRange_8u_C1R( img, camParams.width, srcSize, &counts, 1, 255);*/
// 	//Multiplicamos y normalizamos
// 	ippiMulScale_8u_C1IR(proyImg, LADO_ROBOT*2, bordesImg, LADO_ROBOT*2, srcSize);
// 	//Contamos cuántos on tiene el resultado
// /*	int cRes;
// 	ippiCountInRange_8u_C1R( imgAux, camParams.width, srcSize, &cRes, 1, 255);*/
// 	/*if (counts>0)
// 	{
// 	//	qDebug() << (float)cRes/(float)counts;
// 		similarity = (float)cRes/(float)counts;
// 	}
// 	else similarity = 0.;*/
// 	//sumamos el resultado y dividimos por el nª de pixels del modelo
// 	ippiSum();
// 	qDebug() << counts;
// }


			//kalmanLinear->predict();
	/*		QVec m2d; QMat v2d;
			k2d->predict(m2d,v2d);
			for(uint k=0; k < measuredList[3].size(); k++)
			{
				float ex = (measuredList[3][k].xBaseCenter - m2d(0));
				float ey = (measuredList[3][k].yBaseCenter - m2d(1));
				float d = ::sqrt(ex*ex + ey*ey);
				if( d < 30)
				{
					QVec meas(2); meas(0)= ex; meas(1) = ey;
					k2d->update(meas);
					visor->setColor(0.,255.,128.);
					visor->renderPlainSquare(c2sX(m2d(0)), c2sY(m2d(1)), 10, 1.);
					visor->renderSegment(c2sX(m2d(0)), c2sY(m2d(1)),c2sX(m2d(0)+(m2d(2))),c2sY(m2d(1))+(m2d(3)),1.);
					break;
				}

			}
	*/

void Worker::on_pbRecord_clicked()
{
	if(pbRecord->isChecked())
		trecord.start(1000/15);
	else
		trecord.stop();

	qDebug()<<"trecord";
}

void Worker::record()
{
	int recorteW = 15;
	int recorteH = 5;
	static IppiSize roi = { params.width-recorteW,params.height-recorteH};
	ippiCopy_8u_C3R(img1->bits(),params.width*3,imgG->bits(),params.width*6,roi); //0.0
	ippiCopy_8u_C3R(img2->bits()+recorteW,params.width*3,imgG->bits()+(params.width-recorteW)*3,params.width*6,roi); //0.1

	ippiCopy_8u_C3R(img3->bits(),params.width*3,imgG->bits()+params.width*6*(params.height-recorteH),params.width*6,roi); //1.0
	ippiCopy_8u_C3R(img4->bits()+recorteW,params.width*3,imgG->bits()+params.width*6*(params.height-recorteH)+(params.width-recorteW)*3,params.width*6,roi); //1.1
	
	
	
	
	

	img = imgG->scaled(QSize(imgR->width(),imgR->height()));
	
	img.save("/home/robolab/grabacion/"+QDateTime::currentDateTime().toString("hh:mm:ss:zzz")+".jpeg","JPEG");
}

void Worker::changeImagen()
{
	if(rbC1->isChecked())
		imagenChange = 0;
	if(rbC2->isChecked())
		imagenChange = 1;
	if(rbC3->isChecked())
		imagenChange = 2;
	if(rbC4->isChecked())
		imagenChange = 3;
}