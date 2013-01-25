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
Worker::Worker(RoboCompCamera::CameraPrx cameraprx, RoboCompKinect::KinectPrx kinectprx, QWidget *parent) : QWidget(parent) 
{
  //Proxies: To connect to others components.
	  camera = cameraprx;
	  kinect = kinectprx;

  //Mutex and GUI
	  mutex = new QMutex();//Mutex: with lock() and unlock() you can to block and release the memory.
	  setupUi(this);//GUI: Starting the GUI.
	  
  //Image sizes and depth values.
	  imgHeight=480;	imgWidth=640;	imgHeightIR=488; imgChannelsRGB=3;
	  minDepth=0.7;		maxDepth=10;
	  depthValue=2; 	depthRangeValue=1;

  //Bool values to set several working modes.
	  calibration=false; //1=From RGB Cam; 0=From IR Cam
	  irORrgb=false;//false=RGB; true=IR
	  
  //Several variables to work with QT4: qImage, qPoint, qRGB...
	  qImageRGB  = new QImage (imgWidth,imgHeight,QImage::Format_RGB888);
	  qImageTest = new QImage (imgWidth,imgHeight,QImage::Format_RGB888);
	  qImageIR 	 = new QImage (imgWidth, imgHeightIR, QImage::Format_Indexed8);//Image from IR CAM is 8 pixels heighter and B&W.
	  r_QTvalue = 120;	g_QTvalue=20;	b_QTvalue=30;	a_QTvalue=0;
	  valueRGB = qRgb(r_QTvalue,g_QTvalue,b_QTvalue);
	  
  //Starting Mat (matrix-image OpenCV) and other OpenCV objects.
	  //http://opencv.willowgarage.com/documentation/cpp/core_basic_structures.html?highlight=mat#Mat
	  imageCVrgb.create(imgHeight,imgWidth,CV_8UC3);
// 	  imageCVrgb.create(imgHeight,imgWidth,CV_BGR2RGB);
	  imageCVrgbINV.create(imgHeight,imgWidth,CV_8UC3);
	  imageCVrgbAUX.create(imgHeight,imgWidth,CV_8UC3);
	  Mat maskCV3x3(3, 3, CV_32F, Scalar(5));

		//Starting OpenCV windows.
			cv::namedWindow("windowCV1", CV_GUI_EXPANDED | CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO );
			cv::namedWindow("windowCV2", CV_GUI_EXPANDED | CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO );
			cv::namedWindow("windowCV3", CV_GUI_EXPANDED | CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO );
	  
	  circle=initCircleCV(100,100,10,200,50,50);
	  ksizeCV=Size2i(5,5);
	  counterCV=0;
	  
		//AUX variables to Tracking tests.
	  optionsCV[0]=0; //options to show histogram
	  optionsCV[1]=0; //options to show plane
	  optionsCV[2]=0; //options to show ...
	  
		//OpenCV Luiky Tracking.
	  hsize=16;
	  hranges[0]=0;
	  hranges[1]=180;
	  phranges = hranges;

	  
  //Paint frames
	  rcd2 = new RCDraw  (imgWidth,imgHeight,qImageTest, this->frameTest);
	  show();
	  
  //Connetions between QT4 GUI and several methods.
	  connect(&timer, SIGNAL(timeout())						,this, SLOT(compute()) );
	  connect(slider_Depth,SIGNAL(valueChanged(int))		,this, SLOT(depth_Slider(int)) );
	  connect(slider_DepthRange,SIGNAL(valueChanged(int))	,this, SLOT(depthRange_Slider(int)) );
	  connect(slider_Lux,SIGNAL(valueChanged(int))			,this, SLOT(lux_Slider(int)) );
	  connect(slider_LuxRange,SIGNAL(valueChanged(int))		,this, SLOT(luxRange_Slider(int)) );
	  connect(pb_RGBwithZinIR,SIGNAL(pressed())				,this, SLOT(pbPaintZFromIRCam()) );
	  connect(pb_RGBwithZinRGB,SIGNAL(pressed())			,this, SLOT(pbPaintZFromRGBCam()) );
	  connect(pb_VideoIR, SIGNAL(pressed())					,this, SLOT(pbPaintIR()) );
	  connect(pb_VideoRGB, SIGNAL(pressed())				,this, SLOT(pbPaintRGB()) );
	  connect(pb_StartKinect, SIGNAL(pressed())				,this, SLOT(pbStartKinect()) );
	  connect(pb_StopKinect, SIGNAL(pressed())				,this, SLOT(pbStopKinect()) );
	  connect(pb_ocv1, SIGNAL(pressed())					,this, SLOT(pbOpenCV1()) );
	  connect(pb_ocv2, SIGNAL(pressed())					,this, SLOT(pbOpenCV2()) );
	  connect(pb_ocv3, SIGNAL(pressed())					,this, SLOT(pbOpenCV3()) );
	  connect(pb_ocv4, SIGNAL(pressed())					,this, SLOT(pbOpenCV4()) );
  //Set the period loopback to compute().
	  Period = BASIC_PERIOD;
	  timer.start(Period);
	  
  //Once running functions:
	  pbStopKinect();
	  pbStartKinect();
	  
}
void Worker::compute()
{
  try 
  { 
	paintVideo();
// 	trackCamShift();
	
	
	cv::cvtColor(imageCVrgb,imageCVrgb,CV_BGR2HLS);
	cv::split(imageCVrgb,threeCVplanes);

	
	histogram=calcHistogramCV(threeCVplanes[optionsCV[0]]);
	histogram=getHistogramCV(histogram);
	cv::erode(backProjImageCV,backProjImageCV,Mat());	
	cv::dilate(backProjImageCV,backProjImageCV,Mat());	
	paintCV3images(histogram,threeCVplanes[optionsCV[0]],backProjImageCV);
	

// 	trackBox = CamShift(backProjImageCV, trackWindow,TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));


  }//end try
  catch(Ice::Exception e) 
  {
	cout<<e;
	qDebug()<<"Error talking to kinect: You need to connect the kinect and to run kinectComp."<<e.what();
  }
}
void Worker::pbStartKinect()
{
  //system("cd ../../kinectComp/bin");
  robocompPath = getenv("ROBOCOMP");
  cout<<"Starting KinectComp..."<<endl;
  kinectPathBin = robocompPath + "/Components/RoboLab/Experimental/kinectComp/bin/kinectComp --Ice.Config=" + robocompPath + "/Components/RoboLab/Experimental/kinectComp/etc/config";
 
  kinectProcessStart.start(kinectPathBin.data());
  qImageRGB = new QImage (imgWidth,imgHeight,QImage::Format_RGB888);
  rcd2 = new RCDraw  (imgWidth,imgHeight,qImageRGB, this->frameTest);
  rcd2->update();
}
void Worker::pbStopKinect()
{
  cout<<"Stoping KinectComp..."<<endl;
  kinectProcessStop.start("killall -9 kinectComp");
  paintWelcomeMSG();
//   compute();
}
void Worker::myTracking()
{
	cv::cvtColor(imageCVrgb,imageCVgray,CV_BGR2GRAY);
// 	cv::adaptiveThreshold(imageCVgray, imageCVgray, 255, 0, 1, 7, 0);
// 	cv::blur(imageCVrgb,imageCVrgb, ksize, Point(-1,-1), BORDER_DEFAULT);

	invertCVrgbColor(imageCVrgb,imageCVrgbINV);
	cv::cvtColor(imageCVrgb,imageCVrgb,CV_BGR2HLS);
	cv::split(imageCVrgb,threeCVplanes);
	
	cv::cvtColor(imageCVrgbINV,imageCVrgbINV,CV_BGR2HLS);
	cv::split(imageCVrgbINV,threeCVplanesINV);

  for(int itCVy=8;itCVy<imgHeight-8;itCVy++)
	 {
	   for(int itCVx=8;itCVx<imgWidth-8;itCVx++)
	   {
		  if(threeCVplanes[0].at<uchar>(itCVy,itCVx)>=340*(256/360) && threeCVplanes[0].at<uchar>(itCVy,itCVx)<=360*(256/360))
		  {
			  if(threeCVplanesINV[0].at<uchar>(itCVy+2,itCVx+2)>=(luxRangeValue+20) && threeCVplanesINV[0].at<uchar>(itCVy-2,itCVx-2)<=(luxRangeValue+50))
			  {
			  if ((depthValue <=  depth[itCVx+itCVy*imgWidth]) && (depthValue+depthRangeValue >= depth[itCVx+itCVy*imgWidth]))
				  {
					cv::circle(imageCVrgbINV, Point(itCVx,itCVy), circle.radius, Scalar(luxRangeValue,225,235), -1, 8, 0);
				  }
				}
		  }
		}//End for itCVx
	  }//End for itCVy

	cv::merge(threeCVplanes,imageCVrgb);
	cv::cvtColor(imageCVrgb,imageCVrgb,CV_HLS2RGB);
	cv::cvtColor(imageCVrgbINV,imageCVrgbINV,CV_HLS2RGB);

// 	cv::cvtColor(imageCVrgb, imageCVrgb, CV_BGR2RGB);
// 	cv::cvtColor(imageCVrgbINV, imageCVrgbINV, CV_BGR2RGB);
// 	cv::cvtColor(imageCVrgbAUX, imageCVrgbAUX, CV_BGR2RGB);
	cv::rectangle(imageCVrgb, pointCVul, pointCVdr, Scalar(100,20,20), 1, 8, 0); 
	paintCV3images(imageCVrgb,imageCVgray,imageCVrgbINV);
}
void Worker::trackCamShift()
{
  v.clear();
	Target aux;	
	for (int i=0; i<2;i++)
	{
		aux.visible=false;	
		aux.trackWindow.x=0;
		aux.trackWindow.y=0;
		aux.trackWindow.height=imgHeight;
		aux.trackWindow.width=imgWidth;
		aux.coor3D=QVec::vec3();
		aux.contPerdido=0;
		v.append(aux);
	}

	v[1].patron=cv::Mat ( 200,200,CV_8UC3,Scalar(115,130,150));
	v[1].hist=calcHistogramCV( v[1].patron);
// 	v[1].hist=calcularHistograma( *patronAzul);
	v[1].ventanaHist="histAzul";
	v[1].ventana="Azul";
	v[1].ventanaQString="Azul";
	v[1].color=Scalar(0,0,255);
	v[1].colorQT.setRgb(0,0,255);
	v[1].despla=QVec::vec3(-0.5,-0.2,0);
	v[1].wH=QVec::vec3(0.3,-0.7,0);
	
// 	mostrarHistograma(v[1].hist,v[1].ventanaHist);	
	
	// 	//magenta
// 	cvtColor(*patronMagenta,v[0].patron,CV_BGR2HSV);
	v[0].patron=cv::Mat ( 200,200,CV_8UC3,Scalar(174,100,120));
	v[0].hist=calcHistogramCV(v[0].patron);
	v[0].ventanaHist="histRojo";
	v[0].ventana="Rojo";
	v[0].ventanaQString="Rojo";
	v[0].color=Scalar(255,0,0);
	v[0].despla=QVec::vec3(-0.35,-0.25,0);
	v[0].wH=QVec::vec3(0.1,-0.1,0);
	v[0].colorQT.setRgb(255,0,0);
	
// 	//amarillo
// 	cvtColor(*patronAmarillo,v[1].patron,CV_BGR2HSV);
// 	v[1].hist=calcularHistograma(v[1].patron);
// 	v[1].ventanaHist="histAmarillo";
// 	v[1].ventana="Amarillo";
// 	v[1].ventanaQString="Amarillo";
// 	v[1].color=Scalar(230,180,10);
// // 	mostrarHistograma(v[2].hist,v[2].ventanaHist);
// 
// 	mostrarHistograma(v[1].hist,v[1].ventanaHist);	

  
}
Mat Worker::calcHistogramCV (const cv::Mat imageCV1)
{
int ch[] = {0, 0};
Mat histogram;
cv::calcHist(&imageCV1, 1, ch ,Mat(), histogram, 1, &hsize, &phranges);
cv::calcBackProject(&imageCV1, 1, ch,histogram, backProjImageCV, &phranges);
cv::normalize(histogram, histogram, 0, 255, CV_MINMAX);
return histogram;
}
Mat Worker::getHistogramCV(const cv::Mat histogram)
{
	Mat histimg = cv::Mat::zeros(200, 320, CV_8UC3);	
	histimg = Scalar::all(0);
	int binW = histimg.cols / hsize;
	Mat buffer(1, hsize, CV_8UC3);
	for( int i = 0; i < hsize; i++ )
	buffer.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
	cvtColor(buffer, buffer, CV_HSV2BGR);

//pinta
	for( int i = 0; i < hsize; i++ )
	{
		int val = saturate_cast<int>(histogram.at<float>(i)*histimg.rows/255);
		rectangle( histimg, Point(i*binW,histimg.rows), Point((i+1)*binW,histimg.rows - val), Scalar(buffer.at<Vec3b>(i)), -1, 8 );
	}
	return histimg;
}
void Worker::openCVwork()
{
  
}
void Worker::getPreviousImageCV(Mat imageCVprevious, Mat imageCVcurrent)
{
	imageCVprevious=imageCVcurrent.clone();
}
void Worker::invertCVrgbColor(Mat imageCVnormal, Mat imageCVinverted)
{	
	vector<Mat> planesCV3rgb;
	cv::split(imageCVnormal,planesCV3rgb);
	planesCV3rgb[0].setTo(255, maskCV3x3);
	planesCV3rgb[1].setTo(255, maskCV3x3);
	planesCV3rgb[2].setTo(255, maskCV3x3);
	cv::merge(planesCV3rgb, imageCVinverted);
	cv::absdiff(imageCVnormal, imageCVinverted, imageCVinverted);
}
void Worker::paintCV3images(Mat imageCV1, Mat imageCV2 , Mat imageCV3)
{
	imshow("windowCV1",imageCV1);
	imshow("windowCV2",imageCV2);
	imshow("windowCV3",imageCV3);
// 	imshow("newWindow2",imageCVrgbAUX);
	waitKey(2);
}
void Worker::paintVideo()
{
  	if (irORrgb==0)
	{
	  if (calibration){ kinect->getDataRGBZinRGB(imageRGB, depth, hState, bState );}
	  else{ kinect->getDataRGBZinIR(imageRGB, depth, hState, bState);}
	  memcpy(qImageRGB->bits(), &imageRGB[0],imgHeight*imgWidth*imgChannelsRGB);
 	  memcpy(qImageTest->bits(), &imageRGB[0],imgHeight*imgWidth*imgChannelsRGB);
	  
	  memcpy(imageCVrgb.data, &imageRGB[0], imgHeight*imgWidth*imgChannelsRGB);

	  paintRB_Depth();//Pinta la profundidad en tonos rojos y de azul los objetos que estén a cierta distancia y margen.	
	}
 	else 
	{ 
	  kinect->getDataIR(imageIR, hState, bState);
	  memcpy(qImageIR->bits(), &imageIR[0],imgHeightIR*imgWidth);
	}
	rcd2->update();
	paintAxies();
// 	irORrgb=!irORrgb; //Negación unitaria:cambia el valor de irORrgb.¬
}
void Worker::pbPaintIR()
{
	  irORrgb=true;
 	  delete rcd2;
	  rcd2 = new RCDraw  (imgWidth,imgHeightIR,qImageIR, this->frameTest);
	  rcd2->update();
}
void Worker::pbPaintRGB()
{     
	  irORrgb=false;
	  delete rcd2;
	  rcd2 = new RCDraw  (imgWidth,imgHeight,qImageRGB, this->frameTest);
	  rcd2->update();
}
void Worker::paintRB_Depth()
{
  for(int y=0;y<imgHeight;y++)
	 {
	   for(int x=0;x<imgWidth;x++)
	   {
		 if ((minDepth <= depth[x+y*(imgWidth)]) && (maxDepth >= depth[x+y*(imgWidth)]))
		 {
		  qImageTest->setPixel(x,y,valueRGB=qRgb(luxValue+depth[x+y*imgWidth]*(256/maxDepth),luxValue,luxValue));
		 }
		 else{}
		 if ((depthValue <=  depth[x+y*imgWidth]) && (depthValue+depthRangeValue >= depth[x+y*imgWidth]))
		 {
		  qImageTest->setPixel(x,y,valueRGB=qRgb(luxValue,luxValue,luxValue+depth[x+y*imgWidth]*(256/maxDepth)));
		 }
	  }
	}
}
void Worker::paintWelcomeMSG()
{
  cout<<"Camimic: Welcome to the Interactive Blackboard."<<endl;
  delete rcd2;
  delete qImageRGB;
  qImageRGB = new QImage (imgWidth,imgHeight,QImage::Format_RGB888);
  qImageRGB->load("welcomeMSG.jpg");
  rcd2 = new RCDraw  (imgWidth,imgHeight,qImageRGB, this->frameTest);
  rcd2->update();
}
void Worker::pbPaintZFromIRCam()
{	
	calibration=0;	//kinect->getDataRGBZinIR(imageRGB, depth, hState, bState);
	irORrgb=false;
	delete rcd2;
	rcd2 = new RCDraw  (imgWidth,imgHeight,qImageTest, this->frameTest);
	rcd2->update();
}
void Worker::pbPaintZFromRGBCam()
{	
	calibration=1;	//kinect->getDataRGBZinRGB(imageRGB, depth, hState, bState);
	irORrgb=false;
	delete rcd2;
	rcd2 = new RCDraw  (imgWidth,imgHeight,qImageTest, this->frameTest);
	rcd2->update();
}
void Worker::paintAxies()
{	
	rcd2->drawLineOnTop (QLine(imgWidth/2,0,imgWidth/2,imgHeight),Qt::darkRed,1);//xInicio,yInicio,xFinal,yFinal-(Yaxis)
	rcd2->drawLineOnTop (QLine(0,imgHeight/2,imgWidth,imgHeight/2),Qt::darkRed,1);//xInicio,yInicio,xFinal,yFinal-(Xaxis)
}
void Worker::depth_Slider(int depthValueSet)
{	
	depthValue= (float)depthValueSet/100;
	lcd_Depth->display(depthValue);
}
void Worker::depthRange_Slider(int depthRangeValueSet)
{	
	depthRangeValue= (float)depthRangeValueSet/1000;
	lcd_DepthRange->display(depthRangeValue);
}
void Worker::lux_Slider(int luxValueSet)
{	
	luxValue=luxValueSet; 
	lcd_Lux->display(luxValue);
}
void Worker::luxRange_Slider(int luxRangeValueSet)
{	
	luxRangeValue=luxRangeValueSet;
}
/**
* \brief Default destructor
*/
Worker::~Worker()
{
  pbStopKinect();//Funciona esto????
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
CircleCV Worker::initCircleCV(int x, int y, int radius, int r, int g, int b)
{
  CircleCV circle;
  circle.x=x;
  circle.y=y;
  circle.radius=radius;
  circle.color=Scalar(r,g,b);
  return circle;
}
void Worker::pbOpenCV1()
{
  optionsCV[0]=0;
}
void Worker::pbOpenCV2()
{
  optionsCV[0]=1;
}
void Worker::pbOpenCV3()
{
  optionsCV[0]=2;
}
void Worker::pbOpenCV4()
{
  optionsCV[1]=2;
}
