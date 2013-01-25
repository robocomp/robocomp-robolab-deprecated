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
Worker::Worker()
{

	Period = BASIC_PERIOD*1000*100;
	mutex = new QMutex();
	
	//Libqrdecode
	printf("libdecodeqr version %s\n",qr_decoder_version());
// 	qrdecoder = qr_decoder_open();

}

/**
* \brief Default destructor
*/
Worker::~Worker()
{

}
///Common Behavior

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
	Period = p*1000;
}
/**
* \brief
* @param params_ Parameter list received from monitor thread
*/
bool Worker::setParams(RoboCompCommonBehavior::ParameterList params_)
{
//	active = false;
		//CAMBIAR PARAMETROS Y RE-ARRANQUE DEL COMPONENTE SI ES NECESARIO

//	active = true;
	this->start();
	return true;
}


/**
* \brief Thread method
*/
void Worker::run()
{
	forever
	{
		rDebug("worker running");
		usleep(Period);
	}
}




void Worker::decode(const RoboCompQrdecode::imgType& img, int width, int height, string& message)
{
	qDebug()<<"decode";
	//load image
	IplImage *src =  cvCreateImage( cvSize( width, height ), 8, 3 );
	memcpy (src->imageData,&img[0],width*height*3 );

	qDebug()<<"image"<<width<<height;
	
	QImage m(width,height,QImage::Format_RGB888);
	memcpy(m.bits(),&img[0],width*height*3);
	m.save("hola.png");
	
	
	qrdecoder = qr_decoder_open();
	
	//
    // do decode using default parameter
    //
    short stat=qr_decoder_decode_image(qrdecoder,src);
    printf("STATUS=%04x\n",stat);

	
	message.resize(20);
	message = "Error decoding\n";
    //
    // get QR code header
    //
    QrCodeHeader header;

    if(qr_decoder_get_header(qrdecoder,&header)!= 0)
	{
        //
        // get QR code text
        // To null terminate, a buffer size is larger than body size.
        //
        qDebug()<<"size"<<header.byte_size;
		if(header.byte_size > 0)
		{
			message.resize(header.byte_size+1);
			uchar a[header.byte_size+1];
			int size = qr_decoder_get_body(qrdecoder,&a[0],header.byte_size+1);
			memcpy(&message[0],a,size);
			qDebug()<<"Message:"<<message.c_str();
		}
	}
}



