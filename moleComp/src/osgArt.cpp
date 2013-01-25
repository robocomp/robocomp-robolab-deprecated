#include "osgArt.h"
// You need to add //#include <osgART/GeometryUtils> & #include <osg/io_utils>// in this .cpp file, if you add in .h file you can get problem in linking process.
#include <osgART/GeometryUtils>
#include <osg/io_utils>

osg::Group* createImageBackground(osg::Image* video) 
{
    osgART::VideoLayer* _layer = new osgART::VideoLayer();
    _layer->setSize(video->s(),video->t());
    osgART::VideoGeode* _geode = new osgART::VideoGeode(osgART::VideoGeode::USE_TEXTURE_2D, video);
    osgART::addTexturedQuad(*_geode,video->s(),video->t());
    _layer->addChild(_geode);
    return _layer;
}
OsgArt::OsgArt( QWidget* parent, const QGLWidget* shareWidget, WindowFlags f): QGLWidget(parent, shareWidget, f) , osgViewer::Viewer()
{
    imgAlto=480;
    imgAncho=640;
	
    binarizar =false;
    _gw = new osgViewer::GraphicsWindowEmbedded(0,0,width(),height());
    setParent( parent);
    resize(parent->width(), parent->height());
    getCamera()->setViewport(new osg::Viewport(0,0,width(),height()));
    getCamera()->setGraphicsContext(getGraphicsWindow());

    // create a root node
    root = new osg::Group;

    // attach root node to the viewer
    this->setSceneData(root.get());

    int _tracker_id = osgART::PluginManager::instance()->load("osgart_tracker_artoolkit2");
    tracker = dynamic_cast<osgART::Tracker*>(osgART::PluginManager::instance()->get(_tracker_id));

    if (!tracker.valid()){qFatal("Could not initialize tracker plugin! Without tracker an AR application can not work. Quit if none found.");}

    // get the tracker calibration object
    calibration = tracker->getOrCreateCalibration();

    // load a calibration file
    if (!calibration->load(std::string("data/ar/camera_para.dat"))){qFatal("Non existing or incompatible calibration file, the calibration file was non-existing or couldnt be loaded.");}

    imgSize = imgAncho * 3;

    // create an image that same size (packing set to 1)
    osgImage = new osg::Image();
    bufferImgSize.width = 1600;
    bufferImgSize.height = 1200;
    bufferImgResize = ippsMalloc_8u(bufferImgSize.width * bufferImgSize.height * 3 );
    bufferImgResizeGray = ippsMalloc_8u(bufferImgSize.width * bufferImgSize.height );
    iv=ippsMalloc_8u(imgAncho*imgAlto*3);
    
    osgImage.get()->allocateImage(bufferImgSize.width,bufferImgSize.height, 1, GL_RGB, GL_UNSIGNED_BYTE, 1);
    // set the image source for the tracker le enchufamos nuestra imagen
    tracker->setImage(osgImage.get());

    osgART::TrackerCallback::addOrSet(root.get(), tracker.get());

    //enchufarle nuestra imagen
    videoBackground = createImageBackground(osgImage.get());
    videoBackground->getOrCreateStateSet()->setRenderBinDetails(0, "RenderBin");

    //enchufarle nuestra imagen
    cam = calibration->createCamera();

    ///nueva lineas averiguar coorenadas de pantalla
    cam->setViewport(0,0,imgAncho,imgAlto);
    cameraMatrix = cam->getViewMatrix()  * cam->getProjectionMatrix() * cam->getViewport()->computeWindowMatrix();

    cam->addChild(videoBackground.get());
    cam->getChild(0)->setName("video");

    root->addChild(cam.get());
    this->setSceneData(root.get());

    //marcadores utilizados
    marker = tracker->addMarker("single;data/ar/patt.hiro;80;0;0");
    marker2 = tracker->addMarker("single;data/ar/patt.kanji;80;0;0");

    if (!marker.valid() or !marker2.valid())
        qFatal("Could not add marker!");

	marker->setActive(true);
	marker2->setActive(true);

	arTransform = new osg::MatrixTransform();
	osgART::attachDefaultEventCallbacks(arTransform.get(),marker.get());
	arTransform2 = new osg::MatrixTransform();
	osgART::attachDefaultEventCallbacks(arTransform2.get(),marker2.get());

	arTransform->getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");
	arTransform2->getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");

    //camera
	cam->addChild(arTransform.get());
	cam->addChild(arTransform2.get());
    markerSet.clear();

    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(100);
}
void OsgArt::paintGL()
{
    frame();
}
QVec OsgArt::updateART(std::vector< uchar > imgV)
{
    IppiSize srcSize;
    srcSize.width = imgAncho;// camParams.width;
    srcSize.height = imgAlto;// camParams.height;
    
    IppiRect srcRoi;
    srcRoi.height = imgAlto;// camParams.height;
    srcRoi.width = imgAncho;// camParams.width;
    srcRoi.x = 0;
    srcRoi.y =0;
    
    // osgImage.get()->allocateImage(imgAncho,imgAlto, 1, GL_RGB, GL_UNSIGNED_BYTE, 1);
//     IppStatus st= ippiResize_8u_C3R (&imgV[0],srcSize, srcSize.width*3,srcRoi, bufferImgResize, bufferImgSize.width*3, bufferImgSize,  (double ) (bufferImgSize.width) / (double) (imgAncho), (double) (bufferImgSize.height) / (double) (imgAlto), IPPI_INTER_LINEAR  );    
    IppStatus st= ippiResize_8u_C3R (&imgV[0],srcSize, srcSize.width*3,srcRoi, bufferImgResize, bufferImgSize.width*3, bufferImgSize,  2.5, 2.5, IPPI_INTER_LINEAR  );
    //qDebug()<<parentWidget()->width()<<"....."<<osgImage.get()->s();
        
    if (binarizar )
    {
      //convertir a gray
        ippiRGBToGray_8u_C3C1R(bufferImgResize,bufferImgSize.width*3, bufferImgResizeGray, bufferImgSize.width, bufferImgSize );
	for (int i=0; i<bufferImgSize.width*bufferImgSize.height; i++)
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
    QVec vec =QVec::vec2();
    return vec;
}
void OsgArt::binarize()
{
    binarizar= not binarizar;
    qDebug()<<binarizar<<binarizar;
}
void OsgArt::resizeOsgArt ()
{
    imgAlto = parentWidget()->height();
    imgAncho = parentWidget()->width();
//     bufferImgSize.width=imgAncho;
//     bufferImgSize.height=imgAlto;
//     _gw = new osgViewer::GraphicsWindowEmbedded(0,0,imgAncho,imgAlto);
    //setParent( parent);
    _gw->resized(0,0,parentWidget()->width(),parentWidget()->height());
    resize(parentWidget()->width(),parentWidget()->height());
}
void OsgArt::closeOsgArt()
{
// 	delete juego;
}
bool OsgArt::addMarker(string markerPath, string markerName, bool single, int size)
{
  std::cout<<"ADDMARKER"<<std::endl;
  
  ARMarker ARM;
  osg::ref_ptr<osgART::Marker> marker;
  osg::ref_ptr<osg::MatrixTransform> MT = new osg::MatrixTransform();
  string patron;
  char markerSize [5];
  
  if(single)
  {
    patron = "single;";
    sprintf(markerSize,"%d",size);
    patron=patron+markerPath+markerName+";"+markerSize+";0;0";
  }
  else
  {
    patron = "multi;"+markerPath+markerName;
  }


  printf("IN\n");
  marker = tracker->addMarker(patron);
  printf("OUT\n");
  if (!marker.valid())
  {
    // Without marker an AR application can not work. Quit if none found.
    osg::notify(osg::FATAL) << "Could not add marker!" << std::endl;
    exit(-1);
  }
  
  marker->setActive(true);
  
  
  osgART::attachDefaultEventCallbacks(MT.get(),marker.get());
  MT->getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");
  MT->setName("marker");
  
  cam->addChild(MT.get());

  ARM.setMarker(marker);
  ARM.setMT(MT);

  markerSet.insert(QString::fromStdString(markerName),ARM);
  
  //QString key=markerSet.key(ARM);
  //std::cout<<key.toStdString()<<std::endl;
  
  std::cout<<"END ADDMARKER"<<std::endl;
  return true;
}
bool OsgArt::removeMarker(string id)
{
  std::cout<<"REMOVEMARKER"<<std::endl;
  
  ARMarker ARM;
  osg::ref_ptr<osgART::Marker> marker;
  osg::ref_ptr<osg::MatrixTransform> MT;
  
  ARM = markerSet.value(QString::fromStdString(id));

  MT=ARM.getMT();
  marker=ARM.getMarker();
    
  marker->setActive(false);  
  tracker->removeMarker(marker);

  cam->removeChild(MT.get());

  markerSet.remove(QString::fromStdString(id));
  
  std::cout<<"END REMOVEMARKER"<<std::endl;
  return true;
}
osg::MatrixTransform* OsgArt::getMarkerNode(string id)
{
    ARMarker ARM = markerSet.value(QString::fromStdString(id));
    
    return ARM.getMT();
}
QString OsgArt::getBaseMarker(osg::MatrixTransform* MT)
{
  bool encontrado = false;
  osg::MatrixTransform* MTAux;
  osgART::Marker* markerAux=NULL;
  ARMarker ARM;
  
  MTAux=MT;
  while(!encontrado)
  {
    if(MTAux->getName() == "marker"){
      encontrado = true;
    }
    else
    {
      MTAux = (osg::MatrixTransform*)MTAux->getParent(0);
    }  
  }

  ARM.setMarker(markerAux);
  ARM.setMT(MTAux);
  
  QString key;
  key=markerSet.key(ARM);

  return key;
  
}
osg::MatrixTransform* OsgArt::addBall(osg::Vec3 position,int radius,osg::Vec4 color, bool transparent,osg::ref_ptr<osg::MatrixTransform> arMT)
{
    osg::Sphere *myBall = new osg::Sphere(position,radius);
    osg::ShapeDrawable* sphereDrawable1 = new osg::ShapeDrawable(myBall);
    osg::Geode* shapeGeode1 = new osg::Geode();
    sphereDrawable1->setColor(color);
    
    if(transparent==true)
      sphereDrawable1->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);     
    
    shapeGeode1->addDrawable(sphereDrawable1);

    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    osg::Matrix m;
    mt->setMatrix( m );
    mt->addChild(shapeGeode1);

    arMT->addChild(mt);

    return mt;
}
osg::MatrixTransform* OsgArt::addCone(osg::Vec3 position,osg::Vec3 rotation,int coneRadius,int coneHeight,osg::Vec4 color, bool transparent,osg::ref_ptr<osg::MatrixTransform> arMT)
{
    osg::Cone *myCone = new osg::Cone(position,coneRadius, coneHeight);

    osg::ShapeDrawable* coneDrawableY = new osg::ShapeDrawable( myCone );
    osg::Geode *shapeGY = new osg::Geode();
    coneDrawableY->setColor(color);
    
    if(transparent==true)
      coneDrawableY->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON); 
    
    shapeGY->addDrawable(coneDrawableY);

    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    osg::Matrix m;
    mt->setMatrix( m );
    mt->addChild(shapeGY);

    arMT->addChild(mt);
	rotate(mt,rotation);
    return mt;
}
osg::MatrixTransform* OsgArt::addBox(osg::Vec3 position,osg::Vec3 rotation,osg::Vec3 dimensions,osg::Vec4 color, bool transparent, osg::ref_ptr<osg::MatrixTransform> arMT)
{
    osg::Box *myBox = new osg::Box(position,dimensions.x(),dimensions.y(),dimensions.z());
    
    osg::ShapeDrawable* boxDrawableY = new osg::ShapeDrawable( myBox );
    osg::Geode *shapeGY = new osg::Geode();
    boxDrawableY->setColor(color);
    
    if(transparent==true)
      boxDrawableY->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON); 
    
    shapeGY->addDrawable(boxDrawableY);

    shapeGY->setName("target");
    
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    osg::Matrix m;
    mt->setMatrix( m );
    mt->addChild(shapeGY);

    arMT->addChild(mt);
	rotate(mt,rotation);
    return mt;
}

osg::MatrixTransform* OsgArt::addCylinder(osg::Vec3 position,osg::Vec3 rotation,float radius, float height,osg::Vec4 color, bool transparent,osg::ref_ptr<osg::MatrixTransform> arMT)
{
    osg::Cylinder *myCylinder = new osg::Cylinder(position,radius,height);
    myCylinder->setDataVariance(DYNAMIC);
    osg::ShapeDrawable* CylinderDrawableY = new osg::ShapeDrawable( myCylinder );
    osg::Geode *shapeGY = new osg::Geode();
    CylinderDrawableY->setColor(color);
    
    if(transparent==true)
      CylinderDrawableY->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);     
    
    shapeGY->addDrawable(CylinderDrawableY);

    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    osg::Matrix m;
    mt->setMatrix( m );
    mt->addChild(shapeGY);

    arMT->addChild(mt);
	rotate(mt,rotation);
    return mt;
}

osg::MatrixTransform* OsgArt::addCapsule(osg::Vec3 position,osg::Vec3 rotation,float radius, float height,osg::Vec4 color, bool transparent,osg::ref_ptr<osg::MatrixTransform> arMT)
{
    osg::Capsule *myCapsule = new osg::Capsule(position,radius,height);
    osg::ShapeDrawable* CapsuleDrawableY = new osg::ShapeDrawable( myCapsule );
    osg::Geode *shapeGY = new osg::Geode();
    if(transparent==true)
    {  
      CapsuleDrawableY->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);  
      CapsuleDrawableY->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
    }
    CapsuleDrawableY->setColor(color);
    shapeGY->addDrawable(CapsuleDrawableY);
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    osg::Matrix m;
    mt->setMatrix( m );
    mt->addChild(shapeGY);
    arMT->addChild(mt);
    rotate(mt,rotation);
    return mt;
}
void OsgArt::setColorRGBA(osg::MatrixTransform* mt, osg::Vec4 color)
{
    osg::Geode* myGeode;
    myGeode=(osg::Geode*) mt->getChild(0);
    osg::ShapeDrawable* myDrawable;
    myDrawable= (osg::ShapeDrawable*)myGeode->getDrawable(0);
    myDrawable->setColor(color);
// osg::Cylinder* myCylinder;
// myCylinder=(osg::Cylinder*)myDrawable->getShape();
// myCylinder->setRadius(60);
}
osg::MatrixTransform* OsgArt::addText(osg::Vec3 position, osg::Vec4 color, float characterSize, std::string texto, osgText::Font* font, osg::ref_ptr<osg::MatrixTransform> arMT)
{  
    osgText::String aux(texto,osgText::String::ENCODING_UTF8);
    osg::Geode* geode = new osg::Geode;
    osgText::Text* text = new osgText::Text;
    //Propiedades de text pasadas por parámetros
	text->setColor(color);
	text->setCharacterSize(characterSize);
	text->setPosition(position);
	text->setText(aux);
	text->setFont(font);
    //Propiedades fijas
	text->setAxisAlignment(osgText::Text::XY_PLANE);  //aliniación de los ejes. Por defecto plano XY
	text->setLayout(osgText::Text::LEFT_TO_RIGHT); //escritura de izquierda a derecha
    geode->addDrawable(text);
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    osg::Matrix m;
    mt->setMatrix( m );
    mt->addChild(geode); //colgamos la geode del MatrixTransform
    arMT->addChild(mt);
    return mt;
}
void OsgArt::changePositionText(osg::MatrixTransform* arMT, osg::Vec3 position)
{
    osg::Geode* geode;
    geode=(osg::Geode*)arMT->getChild(0); //obtenemos geode a partir de la matriz
    osgText::Text* text;
    text=(osgText::Text*) geode->getDrawable(0);
    text->setPosition(position);
}
void OsgArt::changeColorText(osg::MatrixTransform* arMT, osg::Vec4 color)
{
    osg::Geode* geode;
    geode=(osg::Geode*)arMT->getChild(0); //obtenemos geode a partir de la matriz
    osgText::Text* text;
    text=(osgText::Text*) geode->getDrawable(0);
    text->setColor(color);
}
void OsgArt::changeCharacterSizeText(osg::MatrixTransform* arMT, float characterSize)
{
    osg::Geode* geode;
    geode=(osg::Geode*)arMT->getChild(0); //obtenemos geode a partir de la matriz
    osgText::Text* text;
    text=(osgText::Text*) geode->getDrawable(0);
    text->setCharacterSize(characterSize);
}
void OsgArt::changeFontText(osg::MatrixTransform* arMT, osgText::Font* font)
{
    osg::Geode* geode;
    geode=(osg::Geode*)arMT->getChild(0); //obtenemos geode a partir de la matriz
    osgText::Text* text;
    text=(osgText::Text*) geode->getDrawable(0);
    text->setFont(font);
}
void OsgArt::changeText(osg::MatrixTransform* arMT, osgText::String texto)
{
    osg::Geode* geode;
    geode=(osg::Geode*)arMT->getChild(0); //obtenemos geode a partir de la matriz
    osgText::Text* text;
    text=(osgText::Text*) geode->getDrawable(0);
    text->setText(texto);
}
void OsgArt::addTexture2D(osg::MatrixTransform* arMT, std::string imagenEntrada, osg::Vec4 valoresMaterial, float transparencia)
{
      osg::Image* image = osgDB::readImageFile(imagenEntrada);
      osg::Geode* pGeode;
      pGeode=(osg::Geode*)arMT->getChild(0);
      
      if (!image) {std::cout << "Couldn't load texture." << std::endl;}
      osg::Texture2D* texture = new osg::Texture2D;
      texture->setDataVariance(Object::DYNAMIC);
      texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
      texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
      texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP);
      texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP);
      texture->setImage(image);      
      osg::Material *material = new osg::Material();
      material->setTransparency( osg::Material::FRONT_AND_BACK, transparencia);
      material->setEmission(osg::Material::FRONT, osg::Vec4(0.8, 0.8, 0.8, 0.5));
      // assign the material and texture to the sphere
      osg::StateSet *sphereStateSet = pGeode->getOrCreateStateSet();
      sphereStateSet->ref();
      sphereStateSet->setAttribute(material);
      sphereStateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
//Transparencia de textura
//       sphereStateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
//       sphereStateSet->setRenderBinDetails(0, "transparent");
//       sphereStateSet->setMode( osg::StateAttribute::GL_BLEND, osg::StateAttribute::ON);
//       sphereStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
//       sphereStateSet->setAttributeAndModes(material, osg::StateAttribute::OVERRIDE); 
}
void OsgArt::addTexture2D(osg::MatrixTransform* arMT, osg::Image *imagenEntrada, osg::Vec4 valoresMaterial, float transparencia)
{  
      osg::Geode* pGeode;
      pGeode=(osg::Geode*)arMT->getChild(0);
      osg::Texture2D* texture = new osg::Texture2D;
      texture->setDataVariance(Object::DYNAMIC);
      texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
      texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
      texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP);
      texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP);
      texture->setImage(imagenEntrada);
      osg::Material *material = new osg::Material();
      material->setTransparency( osg::Material::FRONT_AND_BACK, transparencia);
      material->setEmission(osg::Material::FRONT, osg::Vec4(0.8, 0.8, 0.8, 0.5));
      // assign the material and texture to the sphere
      osg::StateSet *sphereStateSet = pGeode->getOrCreateStateSet();
      sphereStateSet->ref();
      sphereStateSet->setAttribute(material);
      sphereStateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);      
}
void OsgArt::load3DModel(osg::MatrixTransform* arMT, osg::Node* model, osg::Vec3d position, osg::Vec3d escala)
{
      // Load a model
      if (!model){std::cout << " could not find model " << std::endl;} 
      osg::PositionAttitudeTransform * model_pat = new osg::PositionAttitudeTransform();
      model_pat->setPosition(position);
      model_pat->setScale(escala);
      model_pat->addChild(model);
      arMT->addChild(model_pat);
}
void OsgArt::move(osg::MatrixTransform* mt, osg::Vec3 position)
{
    osg::Matrix m = mt->getMatrix();
    osg::Matrix m2;
    m2.makeTranslate(position);
    m.mult(m, m2);
    mt->setMatrix(m);
}
void OsgArt::rotate(osg::MatrixTransform* mt, osg::Vec3 angles)
{
    osg::Matrix m = mt->getMatrix();
    osg::Matrix mX, mY, mZ;
    mX.makeRotate(angles.x(),1,0,0);
    m.mult(m,mX);
    mY.makeRotate(angles.y(),0,1,0);
    m.mult(m,mY);
    mZ.makeRotate(angles.z(),0,0,1);
    m.mult(m,mZ);
    mt->setMatrix(m);
}
void OsgArt::scale(osg::MatrixTransform* mt, osg::Vec3 axies)
{
    osg::Matrix m = mt->getMatrix();
    m.scale(axies);
    mt->setMatrix(m);
}
void OsgArt::setVisible(osg::Node *node)
{
  node->setNodeMask(VISIBLE);
}
void OsgArt::setInvisible(osg::Node *node)
{
  node->setNodeMask(INVISIBLE);
}
//Devuelve la matriz de transformacion del objeto que hay debajo de unas coordenadas de pantalla (corrds 2D)
osg::MatrixTransform* OsgArt::getObjectBy2DCoords(float x, float y)
{
    y=imgAlto-y;
    //Creamos segmento desde el origen de la camara a el punto 2D de pantalla convertido en coordenadas 3D
    osg::Matrix escene(cam->getViewMatrix() * cam->getProjectionMatrix() * cam->getViewport()->computeWindowMatrix());
    osg::Matrix inverseEscene = osg::Matrix::inverse(escene);
    //Definimos los puntos extremos del segmento
    osg::Vec3 near_point = osg::Vec3(x,y,0.0f) * inverseEscene;
    osg::Vec3 far_point = osg::Vec3(x,y,1.0f) * inverseEscene;
    //Creamos el segmento de interseccion
    osg::ref_ptr<osg::LineSegment> segment = new osg::LineSegment;
    segment->set(near_point,far_point);
    osgUtil::IntersectVisitor visitor;
    visitor.addLineSegment(segment.get());
    cam->accept(visitor);
    //Lista de aciertos
    osg::Geode *geoda;
    osg::MatrixTransform* mTransform = NULL; 
    if(visitor.hits())
    {
      osgUtil::IntersectVisitor::HitList& hitList = visitor.getHitList(segment.get());
      if(!hitList.empty())
      {
	geoda=hitList.front().getGeode();
	mTransform = (osg::MatrixTransform*)geoda->getParent(0);
      }
    }
    return mTransform;
}
osg::MatrixTransform* OsgArt::pick(float x, float y)
{
  
}
OsgArt::~OsgArt()
{
  
}