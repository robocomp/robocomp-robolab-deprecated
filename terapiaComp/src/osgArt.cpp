#include "osgArt.h"
//mirar pq no compila en el otro lao?
#include <osgART/GeometryUtils>
#include <ipps.h>
#include <ippdefs.h>
#include <ippcc.h>

osg::Group* createImageBackground(osg::Image* video) {
	osgART::VideoLayer* _layer = new osgART::VideoLayer();
	_layer->setSize(video->s(),video->t());
	osgART::VideoGeode* _geode = new osgART::VideoGeode(osgART::VideoGeode::USE_TEXTURE_2D, video);	
	osgART::addTexturedQuad(*_geode,video->s(),video->t());
	_layer->addChild(_geode);
	return _layer;
}


OsgArt::OsgArt( RoboCompCamera::CameraPrx _camera, QWidget* parent, const QGLWidget* shareWidget, WindowFlags f): QGLWidget(parent, shareWidget, f) , osgViewer::Viewer()
{
    camera = _camera;
	binarizar =false;
	_gw = new osgViewer::GraphicsWindowEmbedded(0,0,width(),height());
	setParent( parent);
	resize(parent->width(), parent->height());
	getCamera()->setViewport(new osg::Viewport(0,0,width(),height()));
	getCamera()->setGraphicsContext(getGraphicsWindow());	
	
	
// 	// create a root node
 	root = new osg::Group;
	
	// 	attach root node to the viewer	
	this->setSceneData(root.get());
		
	int _tracker_id = osgART::PluginManager::instance()->load("osgart_tracker_artoolkit2");	
	tracker = dynamic_cast<osgART::Tracker*>(osgART::PluginManager::instance()->get(_tracker_id));

	if (!tracker.valid()) 
	{
		// Without tracker an AR application can not work. Quit if none found.		
		qFatal("Could not initialize tracker plugin!");
	}
    
	// get the tracker calibration object
	calibration = tracker->getOrCreateCalibration();

	// load a calibration file
	if (!calibration->load(std::string("data/camera_para.dat"))) 
	{
		// the calibration file was non-existing or couldnt be loaded
		qFatal("Non existing or incompatible calibration file");		
	}
		
	try
	{
	  camParams = camera->getCamParams();	
	  imgSize = camParams.size * 3;
	}
	catch(Ice::Exception e)
	{
	  qDebug()<<"Error reading camera params: "<<e.what();
	}
	
	// create an image that same size (packing set to 1)
	osgImage = new osg::Image(); 
// 	osgImage.get()->allocateImage(camParams.width,camParams.height, 1, GL_RGB, GL_UNSIGNED_BYTE, 1);
	bufferImgSize.width = 1600;
	bufferImgSize.height = 1200;
	bufferImgResize = ippsMalloc_8u(bufferImgSize.width * bufferImgSize.height * 3 );
	bufferImgResizeGray = ippsMalloc_8u(bufferImgSize.width * bufferImgSize.height );
	
	osgImage.get()->allocateImage(bufferImgSize.width,bufferImgSize.height, 1, GL_RGB, GL_UNSIGNED_BYTE, 1);
	
 // set the image source for the tracker le enchufamos nuestra imagen
	tracker->setImage(osgImage.get()); 	
		
	osgART::TrackerCallback::addOrSet(root.get(), tracker.get());

	//enchufarle nuestra imagen
	videoBackground = createImageBackground(osgImage.get());
	videoBackground->getOrCreateStateSet()->setRenderBinDetails(0, "RenderBin");
	//enchufarle nuestra imagen
   
	cam = calibration->createCamera();	  
	cam->addChild(videoBackground.get());
	
	juego = NULL;
	
	root->addChild(cam.get());	    	
    this->setSceneData(root.get());
	
 	connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
 	timer.start(90);		
}

void OsgArt::paintGL()
{	
	frame();	
}
void OsgArt::update()
{
	try
	{
	  camera->getRGBPackedImage(0, imgV, hState,bState);
	}
	catch(Ice::Exception e)
	{
	  qDebug()<<"Error talking to camera: "<<e.what();
	}
	IppiSize srcSize;
	srcSize.width = camParams.width;
	srcSize.height = camParams.height;
	
	IppiRect srcRoi;
	srcRoi.height = camParams.height;
	srcRoi.width = camParams.width;
	srcRoi.x = 0;
	srcRoi.y =0;
	
	
	ippiResize_8u_C3R (&imgV[0],srcSize, srcSize.width*3,srcRoi, bufferImgResize, bufferImgSize.width*3, bufferImgSize,  (double ) (bufferImgSize.width) / (double) (camParams.width), (double) (bufferImgSize.height) / (double) (camParams.height), IPPI_INTER_LINEAR  );
	
	if (binarizar )
	{					
		//convertir a gray
		ippiRGBToGray_8u_C3C1R(bufferImgResize,bufferImgSize.width*3, bufferImgResizeGray, bufferImgSize.width, bufferImgSize );		
		for(int i=0; i<bufferImgSize.width*bufferImgSize.height; i++)
		{
			if ( bufferImgResizeGray[i] >= 100 )
				bufferImgResizeGray[i]=255;
			else
				bufferImgResizeGray[i]=0;
				
			bufferImgResize[i*3]=bufferImgResizeGray[i];
			bufferImgResize[i*3+1]=bufferImgResizeGray[i];
			bufferImgResize[i*3+2]=bufferImgResizeGray[i];
		}
		
	}
	
	//ponenmos la imagen para que osg dibuje sus cosas
	osgImage.get()->setImage(bufferImgSize.width,bufferImgSize.height,1,GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, bufferImgResize, osg::Image::NO_DELETE, 1);

	if (juego != NULL )
	  juego->computeGame();
}

void OsgArt::setJuego(QString name, QFrame* parentFrame, RoboCompArmrocio::ArmrocioPrx arm, RoboCompSpeech::SpeechPrx speech)
{    
           
  if ( name == QString::fromUtf8("Flexión hombro.")  )
  {
	if (juego!=NULL)  
		delete juego;	
	juego = new Juego1 (cam, tracker,arm, speech, parentFrame);
 	connect(dynamic_cast<Juego1 *> (juego)->pbBinarize, SIGNAL (clicked()),this,SLOT (binarize()) );	
 	connect(dynamic_cast<Juego1 *> (juego)->pbConseguido, SIGNAL (clicked()) ,this,SLOT (speechConseguido()));		
	binarizar= false;
  }
  
  if ( name == QString::fromUtf8("Flexión y extensión de codo." ))
  {	
	if (juego!=NULL)  
		delete juego;
	juego=NULL;
	juego = new Juego2 (cam, tracker,arm, speech, parentFrame);
	connect(dynamic_cast<Juego2 *> (juego)->pbBinarize, SIGNAL (clicked() ),this,SLOT (binarize()) );	
	connect(dynamic_cast<Juego2 *> (juego)->pbConseguido, SIGNAL (clicked()) ,this,SLOT (speechConseguido()));		  
	binarizar= false;
  }
  
  if ( name == QString::fromUtf8 ("Bienvenida / Despedida.") )
  {	
	if (juego!=NULL)  
		delete juego;	
	juego = NULL;	
	binarizar= false;
  }
}


void OsgArt::binarize()
{
 binarizar= not binarizar;	
 qDebug()<<binarizar<<binarizar;
}

void OsgArt::resizeOsgArt ()
{    
    _gw->resized(0,0,parentWidget()->width(),parentWidget()->height());
	resize(parentWidget()->width(), parentWidget()->height());
}

void OsgArt::closeOsgArt()
{
	delete juego;
}

void OsgArt::speechConseguido()
{
	
	emit signalSpeechConseguido();
}


OsgArt::~OsgArt()
{

}

