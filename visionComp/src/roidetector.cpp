#include "roidetector.h"

roiDetector::roiDetector(const CameraPrx& camera , RoboCompVision::TCamParams params_, int nCam_, QObject *parent):
  QObject(parent), _camera(camera), nCam(nCam_), params(params_)
{
	qDebug() << "roiDetector::roiDetector() -> Initiating roiDetector number " << nCam;
	Umbral_Harris=UMBRAL_HARRIS;
	Umbral_Harris_Min=UMBRAL_HARRIS_MIN;
	Umbral_Hess=UMBRAL_HESS;
	Umbral_Laplacian=(uchar)UMBRAL_LAPLACIAN;

	sizePir=(IppiSize *) malloc(params.pyrLevels*sizeof(IppiSize));
	infoPrisma=(PrismaROI *) malloc(params.pyrLevels*sizeof(PrismaROI));

	imagen = ippsMalloc_8u(params.size);
	imagenV.resize(params.size);

	iniPiramide();

	IppiSize sizeImage;
	sizeImage.width=PAN_RANK;
	sizeImage.height=TILT_RANK;

	Mapa_Umbral_Harris1=(Ipp32f *) ippsMalloc_32f(PAN_RANK*TILT_RANK*sizeof(Ipp32f));
	Mapa_Umbral_Harris2=(Ipp32f *) ippsMalloc_32f(PAN_RANK*TILT_RANK*sizeof(Ipp32f));
	ippiSet_32f_C1R(-1.,Mapa_Umbral_Harris1,PAN_RANK*sizeof(Ipp32f),sizeImage);
	ippiSet_32f_C1R(-1.,Mapa_Umbral_Harris2,PAN_RANK*sizeof(Ipp32f),sizeImage);
	Mapa_Umbral_Harris_Act=Mapa_Umbral_Harris1;
	Mapa_Umbral_Harris_Ant=Mapa_Umbral_Harris2;

	//LogPolar
	//	lp = new LPolar(64,200,height,height);
}

roiDetector::~roiDetector()
{
}

void roiDetector::procesa(uchar *img, const RoboCompVision::TState & state)
{
	stateNew = state;
	totalRois = 0;
	getPiramide(img);
	extraerROIHarrisPrisma();
	//extraerROIHarris();
}

void roiDetector::swapBuffers()
{
	state=stateNew;
	RListE.swap(RListL);
	if (A) {
		LecpiramideG=BpiramideG;
		EscpiramideG=ApiramideG;
	} else {
		LecpiramideG=ApiramideG;
		EscpiramideG=BpiramideG;
	}
	A=!A;
}

inline void roiDetector::printFPS( )
{
	static int fps=0;
	static QTime ti;
	if ((++fps % 50) == 0) {
		qDebug() << " VisionComp Cam fps: " << (50000 / ti.restart())/params.numCams;
	}
}

void roiDetector::iniPiramide()
{
	int i;
	int tamBuftmpHarris;
	
	ippiPyrDownGetBufSize_Gauss5x5(params.width,ipp8u,1,&tamBuftmpPirG);
	buftmpPirG = (Ipp8u *) ippsMalloc_8u(tamBuftmpPirG);
	sizePir[0].width=params.width;
	sizePir[0].height=params.height;

	sizeWholePir=params.size;
	ApiramideG=(TipoPiramide *) malloc(sizeof(TipoPiramide));
	BpiramideG=(TipoPiramide *) malloc(sizeof(TipoPiramide));
	LecpiramideG=ApiramideG;
	EscpiramideG=BpiramideG;
	(* ApiramideG)[0] = (Ipp8u *) ippsMalloc_8u(sizePir[0].width*sizePir[0].height);
	(* BpiramideG)[0] = (Ipp8u *) ippsMalloc_8u(sizePir[0].width*sizePir[0].height);
	respHarris[0] = (Ipp32f *) ippsMalloc_32f(sizePir[0].width*sizePir[0].height*sizeof(Ipp32f));
	mapaHarris[0] = (Ipp8u *) ippsMalloc_8u(sizePir[0].width*sizePir[0].height);

	for (i=1;i<params.pyrLevels;i++) {
		sizePir[i].width=(sizePir[i-1].width)/2;
		sizePir[i].height=(sizePir[i-1].height)/2;
		sizeWholePir+=sizePir[i].width*sizePir[i].height;
		(* ApiramideG)[i] = (Ipp8u *) ippsMalloc_8u(sizePir[i].width*sizePir[i].height);
		(* BpiramideG)[i] = (Ipp8u *) ippsMalloc_8u(sizePir[i].width*sizePir[i].height);
		respHarris[i] = (Ipp32f *) ippsMalloc_32f(sizePir[i].width*sizePir[i].height*sizeof(Ipp32f));
		mapaHarris[i] = (Ipp8u *) ippsMalloc_8u(sizePir[i].width*sizePir[i].height);
	}

	ippiPyrUpGetBufSize_Gauss5x5(sizePir[0].width,ipp8u,1,&tamBuftmpPirL);
	buftmpPirL = (Ipp8u *) ippsMalloc_8u(tamBuftmpPirL);

	for (i=0;i<params.pyrLevels;i++)
		piramideL[i] = (Ipp8u *) ippsMalloc_8u(sizePir[i].width*sizePir[i].height);

	//Inicializaci� datos para esquinas

	AROIList=new TipoROIList;
	BROIList=new TipoROIList;
	LecROIList=AROIList;
	EscROIList=BROIList;
	A=true;
	for (i=0;i<params.pyrLevels;i++) {
		AROIList->tambufROIs[i]=10*sizeof(int)*NFEAT;
		AROIList->numROIs[i]=0;
		AROIList->ROIList[i]=(int *) calloc(AROIList->tambufROIs[i],1);

		BROIList->tambufROIs[i]=10*sizeof(int)*NFEAT;
		BROIList->numROIs[i]=0;
		BROIList->ROIList[i]=(int *) calloc(BROIList->tambufROIs[i],1);

		tambufesqHarris[i]=10*sizeof(int)*2;
		numesqHarris[i]=0;
		esqHarris[i]=(int *) malloc(tambufesqHarris[i]);

		ippiMinEigenValGetBufferSize_32f_C1R(sizePir[i], WHARRIS, WHARRIS, &tamBuftmpHarris);
		buftmpHarris[i] = (Ipp8u *) ippsMalloc_8u(tamBuftmpHarris);
	}

	//Inicializaci� informaci� para el prisma (el ltimo nivel no se utiliza)

	for (i=params.pyrLevels-1;i>=0;i--) {
		infoPrisma[i].sizeImg.width=sizePir[i].width;
		infoPrisma[i].sizeImg.height=sizePir[i].height;
		if (i>=params.pyrLevels-2) {
			infoPrisma[i].sizeROI.width=sizePir[i].width;  //tama� del prisma=tama� del penltimo nivel
			infoPrisma[i].sizeROI.height=sizePir[i].height;
		} else {
			infoPrisma[i].sizeROI.width=sizePir[params.pyrLevels-3].width;  //tama� del prisma=tama� del penltimo nivel
			infoPrisma[i].sizeROI.height=sizePir[params.pyrLevels-3].height;
		}
		infoPrisma[i].xIni=(infoPrisma[i].sizeImg.width-infoPrisma[i].sizeROI.width)/2;
		infoPrisma[i].yIni=(infoPrisma[i].sizeImg.height-infoPrisma[i].sizeROI.height)/2;
		infoPrisma[i].despIni=infoPrisma[i].yIni*infoPrisma[i].sizeImg.width+infoPrisma[i].xIni;
	}
}


void roiDetector::getPiramide(Ipp8u * img)
{
	int i;

	// XXX
	// Safe gauss code
	gaussSize.width = sizePir[0].width-2;
	gaussSize.height = sizePir[0].height-2;
	ippiFilterGauss_8u_C1R(img+sizePir[0].width+1, sizePir[0].width, (* EscpiramideG)[0]+sizePir[0].width+1, sizePir[0].width, gaussSize, ippMskSize3x3);
	// Memory fault prone gauss code?
	//ippiFilterGauss_8u_C1R(img,sizePir[0].width,(* EscpiramideG)[0],sizePir[0].width,sizePir[0],ippMskSize3x3);

	//ippiCopy_8u_C1R(img,sizePir[0].width,(* EscpiramideG[0]),sizePir[0].width,sizePir[0]);

	for (i=1;i<params.pyrLevels;i++)
		ippiPyrDown_Gauss5x5_8u_C1R((* EscpiramideG)[i-1],sizePir[i-1].width, (* EscpiramideG)[i],sizePir[i].width,sizePir[i-1],buftmpPirG);

	for (i=params.pyrLevels-1;i>0;i--) {
		ippiPyrUp_Gauss5x5_8u_C1R((* EscpiramideG)[i],sizePir[i].width,piramideL[i-1],sizePir[i-1].width,sizePir[i],buftmpPirL);
		//ippiSub_8u_C1RSfs((* EscpiramideG)[i-1],sizePir[i-1].width,piramideL[i-1],sizePir[i-1].width,piramideL[i-1],sizePir[i-1].width, sizePir[i-1],-1);
		ippiAbsDiff_8u_C1R((* EscpiramideG)[i-1],sizePir[i-1].width,piramideL[i-1],sizePir[i-1].width,piramideL[i-1],sizePir[i-1].width, sizePir[i-1]);
	}
}

void roiDetector::MaxLaplacianHarris(int x, int y, int scale, int &level, Ipp8u &Laplacian)
{
	int i;
	int xl, yl;
	Ipp8u max,maxl;
	int levelAct;

	xl=x*scale;
	xl=(int)(xl/pow(2,params.pyrLevels-2));
	yl=y*scale;
	yl=(int)(yl/pow(2,params.pyrLevels-2));  
	max=(piramideL[level])[y*sizePir[level].width+x];
	levelAct=level;
	//if(level<params.pyrLevels-2)
	{
		for (i=params.pyrLevels-3;i>=0;i--) {
			xl=xl*2;
			yl=yl*2;
			if (mapaHarris[i][yl*sizePir[i].width+xl]==1) {
				maxl=(piramideL[i])[yl*sizePir[i].width+xl];
				if (maxl>max) {
					level=i;
					max=maxl;
				}
			} else break;
		}//for
	}//if
	Laplacian=max;
}

void roiDetector::MaxLaplacianHarrisPrisma(int x, int y, int scale, int &level, Ipp8u &Laplacian)
{
	int i;
	int xl, yl;
	Ipp8u max,maxl;
	int levelAct;

	xl=x*scale;
	xl=(int)(xl/pow(2,params.pyrLevels-2));
	yl=y*scale;
	yl=(int)(yl/pow(2,params.pyrLevels-2));
	max=(piramideL[level])[y*sizePir[level].width+x];
	levelAct=level;
	if (level<params.pyrLevels-2) {
		for (i=params.pyrLevels-3;i>=0;i--) {
			xl=xl*2;
			yl=yl*2;
			if (i>levelAct && xl>=infoPrisma[i].xIni && xl<infoPrisma[i].xIni+infoPrisma[i].sizeROI.width && yl>=infoPrisma[i].yIni && yl<infoPrisma[i].yIni+infoPrisma[i].sizeROI.height) {
				if (mapaHarris[i][yl*sizePir[i].width+xl]==1) {
					maxl=(piramideL[i])[yl*sizePir[i].width+xl];
					if (maxl>max) {
						level=i;
						max=maxl;
					}
				}
			} else break;
		}//for
	}//for
	Laplacian=max;
}

void roiDetector::extraerROIHarrisPrisma()
{
	int i,j,x,y;
	int xmax,ymax;
	Ipp32f * resp,max;
	int hNMS=RADIUS,wNMS=RADIUS;
	IppiSize sizeNeigh;
	int level,scale;
	Ipp8u Laplacian;
	QPainterPath cola;
	float pan, tilt;
	int pan_i, tilt_i;
	IppiSize sizeImage;
	
	sizeImage.width=PAN_RANK;
	sizeImage.height=TILT_RANK;

	/*Mapa_Aux=Mapa_Umbral_Harris_Ant;
	Mapa_Umbral_Harris_Ant=Mapa_Umbral_Harris_Act;
	Mapa_Umbral_Harris_Act=Mapa_Aux;*/

	//ippiSet_32f_C1R(-1.,Mapa_Umbral_Harris_Act,PAN_RANK*sizeof(Ipp32f),sizeImage);

        sizeNeigh.height=hNMS;
        sizeNeigh.width=wNMS;
        scale=1;

        RListE.resize(params.pyrLevels);   ///////////&&&&

        for (i=0;i<params.pyrLevels;i++) { ///////////&&&&

                ippiSet_8u_C1R(0,mapaHarris[i],sizePir[i].width,sizePir[i]);
                ippiMinEigenVal_8u32f_C1R(&((* EscpiramideG)[i][infoPrisma[i].despIni]),sizePir[i].width,
                                          &(respHarris[i][infoPrisma[i].despIni]),sizePir[i].width*sizeof(Ipp32f),
                                          infoPrisma[i].sizeROI,ippKernelSobel,WHARRIS,WHARRIS,buftmpHarris[i]);
                numesqHarris[i]=0;
                EscROIList->numROIs[i]=0;
                resp=respHarris[i];
                RListE[i].resize(0);
                for (y=infoPrisma[i].yIni+hNMS/2;y<infoPrisma[i].yIni+infoPrisma[i].sizeROI.height-hNMS/2;y++)
                        for (x=infoPrisma[i].xIni+wNMS/2;x<infoPrisma[i].xIni+infoPrisma[i].sizeROI.width-wNMS/2;x++) {
                                ippiMaxIndx_32f_C1R(&(resp[(y-hNMS/2)*sizePir[i].width+(x-wNMS/2)]),
                                                    sizePir[i].width*sizeof(Ipp32f),sizeNeigh,&max,&xmax,&ymax);

								imgToAng(x*scale, y*scale, pan, tilt);
								angToMapIndex(pan, tilt, pan_i, tilt_i);
								tilt_i=y*scale;
								pan_i=x*scale;
                                if (xmax==wNMS/2 && ymax==hNMS/2){

										//if(max>Umbral_Harris_Min && max<Umbral_Harris){
											/*if(Mapa_Umbral_Harris_Ant[tilt_i*PAN_RANK+pan_i]>max){
												qDebug()<<"Banda crítica de Harris"<<"("<<x*scale<<","<<y*scale<<")"<<Mapa_Umbral_Harris_Ant[tilt_i*PAN_RANK+pan_i];	
												max=Mapa_Umbral_Harris_Ant[tilt_i*PAN_RANK+pan_i];	
											}
											else
												qDebug()<<"No estabilizada"<<"("<<x*scale<<","<<y*scale<<")"<<Mapa_Umbral_Harris_Ant[tilt_i*PAN_RANK+pan_i]<<max;*/
										//}
										if(max>=Umbral_Harris) {
                                        	if (tambufesqHarris[i]<=(int)(numesqHarris[i]*sizeof(int)*2)) {
                                                tambufesqHarris[i]+=10*sizeof(int)*2;
                                                esqHarris[i]=(int *) realloc(esqHarris[i],tambufesqHarris[i]);
                                        	}
                                        	esqHarris[i][numesqHarris[i]*2]=x;
                                        	esqHarris[i][numesqHarris[i]*2+1]=y;
                                        	numesqHarris[i]++;
                                        	mapaHarris[i][y*sizePir[i].width+x]=1;
											//Mapa_Umbral_Harris_Ant[tilt_i*PAN_RANK+pan_i]=max;
									}
                                }
                        }//for
                scale=scale*2;
        }

        scale=1;
		int totalEsq=0;
        for (i=0;i<params.pyrLevels;i++) { ///////////&&&&
				totalEsq+=numesqHarris[i];


				
                for (j=0;j<numesqHarris[i];j++) {
                        x=esqHarris[i][j*2];
                        y=esqHarris[i][j*2+1];
                        level=i;
                        MaxLaplacianHarrisPrisma(x,y,scale,level,Laplacian);
                        if ((level>=params.pyrLevels-2) || ((level==i) && (Laplacian>Umbral_Laplacian)))
                        //if(level==i)
                        {
                                RoboCompVision::TRoi roi;
                                roi.x = x;
                                roi.y = y;
                                roi.level = i;
                                roi.scale = scale;
                                roi.xBase = x*scale;
                                roi.yBase = y*scale;
								roi.casado = false;
                                roi.size = 2*params.radius*scale;
                                roi.xBaseCenter = roi.xBase-params.width/2;
                                roi.yBaseCenter = roi.yBase-params.height/2;
                                roi.anillo = (int)rint(sqrt(pow(roi.xBaseCenter,2)+pow(roi.yBaseCenter,2)));
                                RListE[i].push_back(roi);

                        }//level
                }//j
                scale=scale*2;
        }

}

void roiDetector::extraerROIHarris()
{
        long i,j, x,y;
        int xmax,ymax;
        Ipp32f * resp,max;
        int hNMS=RADIUS,wNMS=RADIUS;
        IppiSize sizeNeigh;
        int level,scale;
        Ipp8u Laplacian;

        sizeNeigh.height=hNMS;
        sizeNeigh.width=wNMS;
        scale=1;
        RListE.resize(params.pyrLevels-1);
        for (i=0;i<params.pyrLevels-1;i++) {
                ippiSet_8u_C1R(0,mapaHarris[i],sizePir[i].width,sizePir[i]);
                ippiMinEigenVal_8u32f_C1R((* EscpiramideG)[i],sizePir[i].width,respHarris[i],sizePir[i].width*sizeof(Ipp32f),sizePir[i], ippKernelSobel, WHARRIS, WHARRIS, buftmpHarris[i]);
                numesqHarris[i]=0;
                EscROIList->numROIs[i]=0;
                resp=respHarris[i];
                RListE[i].resize(0);
                for (y=hNMS/2;y<sizePir[i].height-hNMS/2;y++)
                        for (x=wNMS/2;x<sizePir[i].width-wNMS/2;x++) {
                                ippiMaxIndx_32f_C1R(&(resp[(y-hNMS/2)*sizePir[i].width+(x-wNMS/2)]),sizePir[i].width*sizeof(Ipp32f),sizeNeigh,&max,&xmax,&ymax);
                                if (xmax==wNMS/2 && ymax==hNMS/2 && max>Umbral_Harris) {
///////////////////////////      S�O HARRIS       ////////////////////////////////////////
                                        if (tambufesqHarris[i]<=(int)(numesqHarris[i]*sizeof(int)*2)) {
                                                tambufesqHarris[i]+=10*sizeof(int)*2;
                                                esqHarris[i]=(int *) realloc(esqHarris[i],tambufesqHarris[i]);
                                        }
                                        esqHarris[i][numesqHarris[i]*2]=x;
                                        esqHarris[i][numesqHarris[i]*2+1]=y;
                                        numesqHarris[i]++;
                                        mapaHarris[i][y*sizePir[i].width+x]=1;
                                }//for

                        }
                scale=scale*2;
        }

///////////////////////////      HARRIS+LAPLACE       ////////////////////////////////////////
        scale=1;
        for (i=0;i<params.pyrLevels-1;i++) {
                //qDebug()<<"Esquinas del nivel"<<i<<numesqHarris[i];
                for (j=0;j<numesqHarris[i];j++) {
                        x=esqHarris[i][j*2];
                        y=esqHarris[i][j*2+1];
                        level=i;
                        MaxLaplacianHarris(x,y,scale,level,Laplacian);
                        if ((level==i) && (Laplacian>Umbral_Laplacian))
                         //if(level==i)
                        {
							RoboCompVision::TRoi roi;
							roi.x = x;
							roi.y = y;
							roi.level = i;
							roi.scale = scale;
							roi.xBase = x*scale;
							roi.yBase = y*scale;
							roi.casado = false;
							roi.size = 2*params.radius*scale;
							roi.xBaseCenter = roi.xBase-params.width/2;
							roi.yBaseCenter = roi.yBase-params.height/2;
							roi.anillo = (int)rint(sqrt(pow(roi.xBaseCenter,2)+pow(roi.yBaseCenter,2)));
							RListE[i].push_back(roi);

                        }//level
                }
                scale=scale*2;
        }

}


//Detección de MSER
void roiDetector::extraerMSER()
{


}

void roiDetector::imgToAng(int x, int y, float& pan, float& tilt)	 
{

/*	pan=atan2(((float) x-params.width/2),params.focal)+ hStateNew.left.pos;
	tilt = atan2(((float)- y+params.height/2),params.focal) + hStateNew.tilt.pos;*/
	pan=atan2(((float) x-params.width/2),params.focal)+ stateNew.hState.left.pos;
	tilt = atan2(((float)- y+params.height/2),params.focal) + stateNew.hState.tilt.pos;

}

void roiDetector::angToMapIndex(float pan, float tilt, int &pan_i, int &tilt_i)
{
	pan_i=(int) rint(((2.*pan/M_PI)*PAN_RANK+PAN_RANK)/2);
	tilt_i=(int) rint(((2.*tilt/M_PI)*TILT_RANK+TILT_RANK)/2);
}


///Servant access methods
void roiDetector::getLevelPyr(int level, imgType &img)
{
	img.resize(sizePir[level].width*sizePir[level].height);
	memcpy(&(img[0]),(*LecpiramideG)[level],sizePir[level].width*sizePir[level].height);
}

void roiDetector::getWholePyr(imgType &pyr )
{
	int i, posPyr=0;

	pyr.resize(sizeWholePir);
	for (i=0;i<params.pyrLevels;i++) {
		memcpy(&(pyr[posPyr]),(*LecpiramideG)[i],sizePir[i].width*sizePir[i].height);
		posPyr+=sizePir[i].width*sizePir[i].height;
	}

}

void roiDetector::getLevelROIList(int level, RoboCompVision::ROIVector &roiList)
{

	roiList = RListL[level];

}

void roiDetector::getWholeROIList(RoboCompVision::ROIPyrVectors &roiList)
{

	roiList = RListL;

}

void roiDetector::getROILevelPyr(int level, int xc, int yc, int w, int h, RoboCompVision::imgType & img)
{
	int xl, yl;
	int scale;
	IppiSize roiSize;
	Ipp8u * iniRoi;

	scale=(int)pow(2,level);
	xl=xc/scale-w/2;
	yl=yc/scale-h/2;
	roiSize.width=w;
	roiSize.height=h;

	img.resize(w*h);
	iniRoi=&((*LecpiramideG)[level][yl*sizePir[level].width+xl]);
	ippiCopy_8u_C1R(iniRoi,sizePir[level].width,&(img[0]),w,roiSize);
}

void roiDetector::getROIWholePyr(int xc, int yc, int w, int h, RoboCompVision::imgType & img)
{
	int i, xl, yl;
	int scale, posPyr=0;
	IppiSize roiSize;
	Ipp8u * iniRoi;

	roiSize.width=w;
	roiSize.height=h;

	img.resize(w*h*(params.pyrLevels));
	for (i=0;i<params.pyrLevels;i++) 
	{
		scale=(int)pow(2,i);
		xl=xc/scale-w/2;
		yl=yc/scale-h/2;

		iniRoi=&((*LecpiramideG)[i][yl*sizePir[i].width+xl]);
		ippiCopy_8u_C1R(iniRoi,sizePir[i].width,&(img[posPyr]),w,roiSize);

		posPyr+=w*h;
	}
}




