// -*-c++-*-

#ifndef OSGDTE_UTILS_H
#define OSGDTE_UTILS_H

#include <osg/Vec3>
#include <osg/TexGen>
#include <osg/Texture1D>
#include <osg/Texture2D>

#include <osgDB/ReadFile>

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Material>
#include <osg/Geometry>
#include <osg/Quat>

#include <iostream>

using namespace osg;
using namespace std;

namespace osgDTE {

  const Vec4 ROJO (1.0f, 0.5f, 0.5f,1.0f);
  const Vec4 ROJO_T (1.0f, 0.5f, 0.5f,0.5f);
  const Vec4 ROJO2(1.0f, 0.2f, 0.2f,1.0f);
  const Vec4 ROJOOSCURO(0.5f, 0.2f, 0.2f,1.0f);
  const Vec4 BLANCO (1.0f, 1.0f, 1.0f,1.0f);
  const Vec4 AZUL (0.5f, 0.5f, 1.0f,1.0f);
  const Vec4 VERDE (0.5f, 1.0f, 0.5f,1.0f);
  const Vec4 AMARILLO (1.0f,1.0f,0.0f,1.0f);
  const Vec4 AMARILLOOSCURO (0.5f,0.5f,0.0f,1.0f);
  const Vec4 NEGRO (0.1f, 0.1f, 0.1f, 1.0f);
  const Vec4 GRIS (0.8f, 0.8f, 0.8f, 1.0f);



  class DameTessellationHints {
  public:
    
    DameTessellationHints()
      {
	_th = new TessellationHints;
	_th->setDetailRatio(0.5f);
      }
    
    ref_ptr<TessellationHints> _th;
  };
  static DameTessellationHints dth;

  
  
  // Calcular el determinante de la matriz
  inline double Det(const osg::Matrixf& mat)
    {
      double r00, r01, r02,
	r10, r11, r12,
	r20, r21, r22,
	temp1, temp2, temp3; 
      
      r00 = mat(0,0); r01 = mat(0,1); r02 = mat(0,2);
      r10 = mat(1,0); r11 = mat(1,1); r12 = mat(1,2);
      r20 = mat(2,0); r21 = mat(2,1); r22 = mat(2,2);
      
      temp1 = r11*r22 - r12*r21;
      temp2 = r02*r21 - r01*r22;
      temp3 = r01*r12 - r02*r11;
      
      return (r00*temp1 + r10*temp2 + r20*temp3);
    }
  
  
  
  //  Devolver un vector de la misma direccion al de entrada
  //  pero de modulo unidad 
  inline Vec3 Unit(const Vec3& in)
    {
      float length = in.length();
      return Vec3(in._v[0] / length, in._v[1] / length, in._v[2] / length);
    }
  
  
  inline Vec3 Project(Vec3& punto, Vec3& linea1, Vec3& linea2)
    {
      Vec3 vecdirector = linea2 - linea1;
      vecdirector.normalize();
      return Vec3(linea1 + (vecdirector * ((punto - linea1) * vecdirector)));
    }
  
  
  
  inline osg::Drawable* caja(const Vec3& center, float longX, float longY, float longZ, 
					 const Vec4& color)
    {
      osg::Box* box = new osg::Box(center, longX, longY, longZ);
      ShapeDrawable* caja = new ShapeDrawable(box,dth._th.get());
      caja->setColor(color);
      return caja;
    }
  
  inline osg::Group* cajaGrupo(const Vec3& center, float longX, float longY, float longZ, 
					 const Vec4& color)
    {
      Group* group = new Group();

      Geode* geo_0 = new Geode();
      osg::Box* box = new osg::Box(center, longX, longY, longZ);
      ShapeDrawable* caja = new ShapeDrawable(box,dth._th.get());
      caja->setColor(color);

      geo_0->addDrawable(caja);
      group->addChild(geo_0);
      return group;
    }

  
  inline osg::Drawable* esfera(const Vec3& center, float radio,  const Vec4& color )
    {
      ShapeDrawable* esfera = new ShapeDrawable(new Sphere(center,radio),dth._th.get());
      esfera->setColor(color);
      return esfera;
    }
  
  // Crear una cilindro
  inline osg::Drawable* cilindro(const Vec3& center, float radio,float altura,  const Vec4& color )
    {
      ShapeDrawable* cilindro = new ShapeDrawable(new Cylinder(center,radio,altura),dth._th.get());
      cilindro->setColor(color);
      return cilindro;
    }
  
  
  // Crear conos
  
  // Crear una cono apuntando hacia el eje Z
  inline osg::Drawable* conoZ(const Vec3& center, float radio,float altura,  const Vec4& color )
    {
      ShapeDrawable* cono = new ShapeDrawable(new Cone(center,radio,altura),dth._th.get());
      cono->setColor(color);
      return cono;
    }
  
  // Crear una cono apuntando hacia el eje Z
  inline osg::Drawable* conoY(const Vec3& center, float radio,float altura,  const Vec4& color )
    {
      ref_ptr<Cone> conegeometry = new Cone(center,radio,altura);
      conegeometry->setRotation(Quat(osg::inDegrees(-90.0f), Vec3(1.0f, 0.0f, 0.0f)));
      ShapeDrawable* cono = new ShapeDrawable(conegeometry.get(),dth._th.get());
      cono->setColor(color);
      return cono;
    }
  // Crear una cono apuntando hacia el eje Z
  inline osg::Drawable* conoX(const Vec3& center, float radio,float altura,  const Vec4& color )
    {
      ref_ptr<Cone> conegeometry = new Cone(center,radio,altura);
      conegeometry->setRotation(Quat(osg::inDegrees(90.0f), Vec3(0.0f, 1.0f, 0.0f)));
      ShapeDrawable* cono = new ShapeDrawable(conegeometry.get(),dth._th.get());
      cono->setColor(color);
      return cono;
    }
  
  // Crear un cono apuntando hacia una direccion cualquiera
  //inline osg::Drawable* cono(const Vec3& center, float radio, float altura,  const Vec4& color, const Vec3& direccion)
  //  {
  //    ref_ptr<Cone> conegeometry = new Cone(center,radio,altura);
  //    conegeometry->setRotation(Quat(direccion));
  //    ShapeDrawable* cono = new ShapeDrawable(conegeometry.get(),dth._th.get());
  //    cono->setColor(color);
  //    return cono;
  //  }
  
  
  
  // Crear una cilindro apuntando hacia el eje Z
  inline osg::Drawable* cilindroZ(const Vec3& center, float radio,float altura,  const Vec4& color )
    {
      ShapeDrawable* cilindro = new ShapeDrawable(new Cylinder(center,radio,altura),dth._th.get());
      cilindro->setColor(color);
      return cilindro;
    }
  
  // Crear una cilindro apuntando hacia el eje Y
  inline osg::Drawable* cilindroY(const Vec3& center, float radio,float altura,  const Vec4& color )
    {
      ref_ptr<Cylinder> conegeometry = new Cylinder(center,radio,altura);
      conegeometry->setRotation(Quat(osg::inDegrees(-90.0f), Vec3(1.0f, 0.0f, 0.0f)));
      ShapeDrawable* cilindro = new ShapeDrawable(conegeometry.get(),dth._th.get());
      cilindro->setColor(color);
      return cilindro;
    }
  // Crear una cilindro apuntando hacia el eje X 
  inline osg::Drawable* cilindroX(const Vec3& center, float radio,float altura,  const Vec4& color )
    {
      ref_ptr<Cylinder> conegeometry = new Cylinder(center,radio,altura);
      conegeometry->setRotation(Quat(osg::inDegrees(90.0f), Vec3(0.0f, 1.0f, 0.0f)));
      ShapeDrawable* cilindro = new ShapeDrawable(conegeometry.get(),dth._th.get());
      cilindro->setColor(color);
      return cilindro;
    }
  
  
  // Crea unos conillos y unos cilindros para representar un eje de coordenadas 
  inline Group* crearEjeCoordenadas(float size)
    {
      Group* group = new Group();
      
      // Para los tamanios
      float radio = 0.4 *  size;
      float radiofino = 0.02 * size;
      size *= 2;
      
      Geode* geo_0 = new Geode();
      geo_0->addDrawable(esfera(Vec3(0.0f,0.0f,0.0f),radio,BLANCO));
      
      geo_0->addDrawable(conoY(Vec3(0.0f,size*2,0.0f),radio,size,AZUL));
      geo_0->addDrawable(cilindroY(Vec3(0.0f,size,0.0f),radiofino,size*2,AZUL));
      
      geo_0->addDrawable(conoZ(Vec3(0.0f,0.0f,size*2),radio,size,ROJO));
      geo_0->addDrawable(cilindroZ(Vec3(0.0f,0.0f,size),radiofino,size*2,ROJO));
      
      geo_0->addDrawable(conoX(Vec3(size*2,0.0f,0.0f),radio,size,VERDE));  
      geo_0->addDrawable(cilindroX(Vec3(size,0.0f,0.0f),radiofino,size*2,VERDE));  
      
      group->addChild(geo_0);
      
      return group;
    }
  
  inline osg::Drawable* DrawLine(osg::Vec3 start, osg::Vec3 end, osg::Vec4 color)
  {
    
    osg::Geometry* lineGeometry = new osg::Geometry();
    
    osg::Vec3Array* vArray = new osg::Vec3Array(2);
    osg::Vec4Array* colors = new osg::Vec4Array(1);
    
    (*vArray)[0] = start;
    (*vArray)[1] = end;
    (*colors)[0] = color;
    
    lineGeometry->setVertexArray(vArray);
    lineGeometry->setColorArray(colors);
    lineGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
    osg::DrawArrays* drawable = new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2);
    lineGeometry->addPrimitiveSet(drawable);
    return lineGeometry;
  }
  
  // Crea una 'T'. Los vértices están en los puntos que se pasan
  // como parámetros, siendo el punto 'A' el vértice inferior  
  inline Group* crearT( const Vec3& A, const Vec3& B, const Vec3& C, 
				float size, const Vec4& color )
  {
    Group* group = new Group();
    Group* caja = new Group();
    osg::Vec3d aux;
    osg::Quat rot;
    
    // Caja B-C
    aux = B - C;
    rot.makeRotate(osg::Vec3d(0.0f,1.0f,0.0f),aux);
    caja = cajaGrupo( osg::Vec3d(0.0f,0.0f,0.0f), size, aux.length(), size, color ); 
    MatrixTransform* matrot2 = new MatrixTransform;
    matrot2->setMatrix(osg::Matrix::translate(0.0f,aux.length()/2,0.0f)*
		       osg::Matrix::rotate(rot)*
		       osg::Matrix::translate(C));
    matrot2->addChild(caja); 
    group->addChild(matrot2);

    // Caja A-(B+C/2)
    aux = (C+B)/2 - A;
    rot.makeRotate(osg::Vec3d(0.0f,1.0f,0.0f),aux);
    caja = cajaGrupo( osg::Vec3d(0.0f,0.0f,0.0f), size, aux.length(), size, color ); 
    MatrixTransform* matrot3 = new MatrixTransform;
    matrot3->setMatrix(osg::Matrix::translate(0.0f,aux.length()/2,0.0f)*
		       osg::Matrix::rotate(rot)*
		       osg::Matrix::translate(A));
    matrot3->addChild(caja); 
    group->addChild(matrot3);

    return group;
  }

  // Crea un triángulo constituido por tres palotes. Los vértices están en los puntos que se pasan
  // como parámetros
  inline Group* crearTriangulo( const Vec3& A, const Vec3& B, const Vec3& C, 
				float size, const Vec4& color )
  {
    Group* group = new Group();
    Group* caja = new Group();
    osg::Vec3d aux;
    osg::Quat rot;
    
    // Caja A-B
    aux = B - A;
    rot.makeRotate(osg::Vec3d(0.0f,1.0f,0.0f),aux);
    caja = cajaGrupo( osg::Vec3d(0.0f,0.0f,0.0f), size, aux.length(), size, color ); 
    MatrixTransform* matrot = new MatrixTransform;
    matrot->setMatrix(osg::Matrix::translate(0.0f,aux.length()/2,+0.0f)*
		      osg::Matrix::rotate(rot)*
		      osg::Matrix::translate(A));
    matrot->addChild(caja); 
    group->addChild(matrot);
    
    // Caja B-C
    aux = B - C;
    rot.makeRotate(osg::Vec3d(0.0f,1.0f,0.0f),aux);
    caja = cajaGrupo( osg::Vec3d(0.0f,0.0f,0.0f), size, aux.length(), size, color ); 
    MatrixTransform* matrot2 = new MatrixTransform;
    matrot2->setMatrix(osg::Matrix::translate(0.0f,aux.length()/2,0.0f)*
		       osg::Matrix::rotate(rot)*
		       osg::Matrix::translate(C));
    matrot2->addChild(caja); 
    group->addChild(matrot2);

    // Caja A-C
    aux = C - A;
    rot.makeRotate(osg::Vec3d(0.0f,1.0f,0.0f),aux);
    caja = cajaGrupo( osg::Vec3d(0.0f,0.0f,0.0f), size, aux.length(), size, color ); 
    MatrixTransform* matrot3 = new MatrixTransform;
    matrot3->setMatrix(osg::Matrix::translate(0.0f,aux.length()/2,0.0f)*
		       osg::Matrix::rotate(rot)*
		       osg::Matrix::translate(A));
    matrot3->addChild(caja); 
    group->addChild(matrot3);

    return group;
  }
  
  // Crea un triángulo constituido por tres palotes. Los vértices están en los puntos que se pasan
  // como parámetros
  inline Group* crearTrianguloOld( const Vec3& A, const Vec3& B, const Vec3& C, 
				float size, const Vec4& color )
  {
    Group* group = new Group();
    Geode* geo_0 = new Geode();
    Geometry* lineGeometry = new Geometry();
    
    osg::Vec3Array* vArray = new osg::Vec3Array(3);
    osg::Vec4Array* colors = new osg::Vec4Array(1);
    
    (*vArray)[0] = A;
    (*vArray)[1] = B;
    (*vArray)[2] = C;
    (*colors)[0] = color;
    
    lineGeometry->setVertexArray(vArray);
    lineGeometry->setColorArray(colors);
    lineGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
    osg::DrawArrays* drawable = new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 3);
    lineGeometry->addPrimitiveSet(drawable);
    geo_0->addDrawable(lineGeometry);
    group->addChild(geo_0);

    return group;
  }

  // Crea unos conillos y unos cilindros para representar un eje de coordenadas 
  inline Group* crearEjeCoordenadasX(float size)
    {
      Group* group = new Group();
      
      // Para los tamanios
      float radio = 0.4 *  size;
      float altura = 2 * size;
      
      Geode* geo_0 = new Geode();
      geo_0->addDrawable(conoX(Vec3(2.0f * size,0.0f,0.0f),radio,altura,VERDE));  
      group->addChild(geo_0);
      
      return group;
    }

  inline void cambiarColorNodo( Node* node, osg::Vec4 defaultHighlightColor = osg::Vec4(1,0,0,1) )
    {
      osg::Material* mat = new osg::Material;
      
      mat->setDiffuse( osg::Material::FRONT_AND_BACK, defaultHighlightColor );
      node->getOrCreateStateSet()->
	setAttributeAndModes( mat, osg::StateAttribute::OVERRIDE );
      //node->getOrCreateStateSet()->
      //	setAttributeAndModes( mat, osg::StateAttribute::ON );
    }
  
  
  inline void devolverColorNodo(Node* node)
    {
      osg::Material* mat = new osg::Material;
      
      node->getOrCreateStateSet()->
	setAttributeAndModes( mat, osg::StateAttribute::ON );
    }
  
  // Creates a stateset which contains a 1D texture which is populated by contour banded color
  // this is then used in conjunction with TexGen to create contoured models, either in 
  // object linear coords - like contours on a map, or eye linear which contour the distance from
  // the eye. An app callback toggles between the two tex gen modes.
  inline osg::StateSet* create1DTextureStateToDecorate(osg::Node* loadedModel)
  { 
    const osg::BoundingSphere& bs = loadedModel->getBound();
    
    osg::Image* image = osgDB::readImageFile("grano.jpg");
    if (!image)
      cerr<<"couln't find texture, amparo"<<endl;
        
    osg::Texture1D* texture = new osg::Texture1D;
    texture->setWrap(osg::Texture1D::WRAP_S,osg::Texture1D::MIRROR);
    texture->setFilter(osg::Texture1D::MIN_FILTER,osg::Texture1D::LINEAR);
    texture->setImage(image);
    
    float zBase = bs.center().z()-bs.radius();
    float zScale = 2.0f/bs.radius();
    
    osg::TexGen* texgen = new osg::TexGen;
    texgen->setMode(osg::TexGen::SPHERE_MAP);//OBJECT_LINEAR);//NORMAL_MAP);//SPHERE_MAP);
    texgen->setPlane(osg::TexGen::S,osg::Vec4(0.0f,0.0f,zScale,-zBase));
    
    osg::Material* material = new osg::Material;
    
    osg::StateSet* stateset = new osg::StateSet;
    
    stateset->setTextureAttribute(0,texture,osg::StateAttribute::OVERRIDE);
    stateset->setTextureMode(0,GL_TEXTURE_1D,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    stateset->setTextureMode(0,GL_TEXTURE_2D,osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE);
    stateset->setTextureMode(0,GL_TEXTURE_3D,osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE);

    stateset->setTextureAttribute(0,texgen,osg::StateAttribute::OVERRIDE);
    stateset->setTextureMode(0,GL_TEXTURE_GEN_S,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    
    stateset->setAttribute(material,osg::StateAttribute::OVERRIDE);
    
    return stateset;
  }
  
  
  // Crear una matriz de transformacion a partir de los parametros DH2
  // (Modified Denavit-Hartenberg. Ver Craig, Ollero).
  //
  // OJO que las matrices de osg son:
  // 
  // |  R    0 |
  // |         |
  // |   T     |
  // |  t    1 |
  //
  // Luego la matriz generada debe ser:
  //
  // |  cos (\theta_i)  cos (\alpha_{i-1}) * sin(\theta_i)  sin(\alpha_{i-1}) * sin(\theta_i)     0 |
  // | 												  |
  // | -sin (\theta_i)  cos (\alpha_{i-1}) * cos(\theta_i)  sin(\alpha_{i-1}) * cos(\theta_i)     0 |
  // |												  |
  // |       0                   -sin(\alpha_{i-1})                   cos(\alpha_{i-1})           0 |
  // |												  |
  // |     a_{i-1}           -sin(\alpha_{i-1} * d_i             cos(\alpha_{i-1}) * d_i          1 |
  // |
  //
  
  // OJO: alpha_imenos1 y theta_i se deben dar en GRADOS
  
  inline Matrix* MatrizDH(double a_imenos1, double alpha_imenos1, double d_i, double theta_i)
    {
      
      osg::Matrix* mat1 = new osg::Matrix;
      
      // Pasar a radianes
      alpha_imenos1 = osg::inDegrees(alpha_imenos1);
      theta_i = osg::inDegrees(theta_i);
      
      mat1->set(cos (theta_i) ,  cos (alpha_imenos1) * sin(theta_i), sin(alpha_imenos1) * sin(theta_i), 0,
		-sin (theta_i),  cos (alpha_imenos1) * cos(theta_i), sin(alpha_imenos1) * cos(theta_i), 0, 
		0             ,  -sin(alpha_imenos1)               , cos(alpha_imenos1)               , 0, 
		a_imenos1     ,  -sin(alpha_imenos1) * d_i         , cos(alpha_imenos1) * d_i         , 1 );
      
      // Devolver la matriz
      return mat1;
    }
  
}

#endif
