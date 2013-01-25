#include "learning.h"
#include <math.h>
#include <fstream>

// Constructor
Learning::Learning()
{
  entrada = new Patron();
  entrada->clear();
  secuencia_entrada.clear();
  maximos_entrada.clear();
  minimos_entrada.clear();
  patrones.clear(); 
  pesos.clear();
  manceldas = new manejadorceldas();
  dist2D = new DistanciaParCeldas(*manceldas);
  distCoord = new DistanciaCoordenadas(*manceldas);
  num_ganadores = -1; // en principio, representa las distancias a todos los patrones
}

// Función para cambiar los valores de los pesos
void Learning::cargaPesos(vector<float> nuevos_pesos)
{
  pesos.clear();
  pesos = nuevos_pesos;
}

// Cambia los valores del grid de celdas
void Learning::configuraCeldas(int delta_x, int delta_y, int delta_z, int x, int y, int z)
{
  manceldas->cambiarDeltas(delta_x, delta_y, delta_z);
  manceldas->cambiarEspacio(x, y, z);
}

// Función para leer los ficheros de patrón
// Devuelve true si todo va bien o false si hay algún problema que impida leer los patrones
bool Learning::cargaPatrones(string fichero_config, string fichero_nombres)
{

  ifstream ifs_config(fichero_config.c_str());
  
  // Leer el fichero de configuracion que indica quién integra la base de datos de patrones
  vector<string> ficheros_patron;
  copy(istream_iterator<string>(ifs_config), istream_iterator<string>(), back_inserter(ficheros_patron));

  // Lee el fichero de nombres
  ifstream ifs_nombres(fichero_nombres.c_str()); 
  vector<string> nombres_patron;
  copy(istream_iterator<string>(ifs_nombres), istream_iterator<string>(), back_inserter(nombres_patron));

  //cout << "PATRONES A LEER: " << endl;
  //copy(ficheros_patron.begin(), ficheros_patron.end(), ostream_iterator<string>(cout, "\n"));

  // Si no hay fichero de patrón, se avisa que no se ha leído nada
  if ( ficheros_patron.size()== 0 )
    {
      cout << "AVISO (learning): No se ha leído ningún fichero de patrón!!!" << endl;
      return false;
    }

  // Leer cada fichero de patron 
  patrones.resize(ficheros_patron.size());
  vector<Patron>::iterator patron = patrones.begin();
  vector<string>::iterator nombre_patron;
  if (nombres_patron.size()!=0)
    nombre_patron = nombres_patron.begin();
  for (vector<string>::iterator fichero = ficheros_patron.begin();
       fichero != ficheros_patron.end();
       fichero++, patron++) 
    {
      // Lo primero limpiarlo
      patron->clear();

      int cuenta;

      FILE *f_nombres;
      FILE *f_datos;
      FILE *f_amplitud;
      FILE *f_key;
      FILE *f_cal;
      string aux = (*fichero);
      if (nombres_patron.size()!=0) {
	patron->nombre = (*nombre_patron);
	nombre_patron++;
      }
      else 
	patron->nombre = aux; // carga el nombre del patron
      
      // Carga los nombres de las trayectorias
      aux = (*fichero)+".nom";
      if (!(f_nombres = fopen(aux.c_str(),"r")) ) { 
	cout<< "ERROR (learning): No se ha podido abrir el fichero "<<aux<<" de la base de patrones"<<endl;
	exit(-1);
      }      
      ifstream ifs_nombres(aux.c_str());
      copy(istream_iterator<string>(ifs_nombres), istream_iterator<string>(), back_inserter(patron->nombres));
      
      // Carga las posiciones de celdas
      aux = (*fichero)+".cel";
      if (!(f_datos = fopen(aux.c_str(),"r")) ) { 
	cout<< "ERROR (learning): No se ha podido abrir el fichero "<<aux<<" de la base de patrones"<<endl;
	exit(-1);
      }      
      fscanf(f_datos,"%d\n",&cuenta);
      for (int i = 0; i<cuenta; i++) {
	vector<CELDA> celdasleidas;
	celdasleidas.clear();
	for (unsigned int j=0; j<patron->nombres.size(); j++) {
	    long int aim;
	    fscanf(f_datos,"%ld ",&aim);
	    celdasleidas.push_back(aim);
	  }    
	fscanf(f_datos,"\n");
	patron->celdas.push_back(celdasleidas);
      }
      
      // Carga los valores de amplitud
      aux = (*fichero)+".amp";
      if (!(f_amplitud = fopen(aux.c_str(),"r")) ) { 
	cout<< "ERROR (learning): No se ha podido abrir el fichero "<<aux<<" de la base de patrones"<<endl;
	exit(-1);
      }      
      for (unsigned int j=0; j<patron->nombres.size(); j++) {
	COORDENADA aim;
	fscanf(f_amplitud,"%d %d %d\n", &aim.x, &aim.y, &aim.z);
	patron->mov.push_back(aim);
      }

      // Carga los keypoints XYZ
      aux = (*fichero)+".key";
      if (!(f_key = fopen(aux.c_str(),"r")) ) { 
	cout<< "ERROR (learning): No se ha podido abrir el fichero "<<aux<<" de la base de patrones"<<endl;
	exit(-1);
      } 
      patron->secs_key_XZ.resize(patron->nombres.size());
      patron->secs_key_YZ.resize(patron->nombres.size());
      for (unsigned int j=0; j<patron->nombres.size(); j++) {
	fscanf(f_key,"%d\n",&cuenta);
	for (int i = 0; i<cuenta; i++) {
	  COORDENADA keyXZ;
	  fscanf(f_key,"%d %d %d\n", &keyXZ.x, &keyXZ.y, &keyXZ.z);
	  patron->secs_key_XZ.at(j).push_back(keyXZ);
	}
	fscanf(f_key,"%d\n",&cuenta);
	for (int i = 0; i<cuenta; i++) {
	  COORDENADA keyYZ;
	  fscanf(f_key,"%d %d %d\n", &keyYZ.x, &keyYZ.y, &keyYZ.z);
	  patron->secs_key_YZ.at(j).push_back(keyYZ);
	}
      }

      // Carga los keypoints CAL
      patron->cal_key_XZ.resize(patron->nombres.size());
      patron->cal_key_YZ.resize(patron->nombres.size());
      aux = (*fichero)+".cal";
      if (!(f_cal = fopen(aux.c_str(),"r")) ) { 
	cout<< "WARNING (learning): No se ha podido abrir el fichero "<<aux<<" de la base de patrones"<<endl;
      }
      else { 
	for (unsigned int j=0; j<patron->nombres.size(); j++) {
	  fscanf(f_cal,"%d\n",&cuenta);
	  for (int i = 0; i<cuenta; i++) {
	    CAL keyXZ;
	    fscanf(f_cal,"%f %f\n", &keyXZ.curva, &keyXZ.arcodistancia);
	    patron->cal_key_XZ.at(j).push_back(keyXZ);
	  }
	  fscanf(f_cal,"%d\n",&cuenta);
	  for (int i = 0; i<cuenta; i++) {
	    CAL keyYZ;
	    fscanf(f_cal,"%f %f\n", &keyYZ.curva, &keyYZ.arcodistancia);
	    patron->cal_key_YZ.at(j).push_back(keyYZ);
	  }
	}
      }
      // (con tanto cargar, acabaremos cargandonos algo)

      fclose(f_nombres);
      fclose(f_datos);
      fclose(f_amplitud);
      fclose(f_key);
      if (f_cal) fclose(f_cal);
    }

  cout << "-----" << endl << "INFORMACION DE LEARNING.CPP. PATRONES DE MOVIMIENTO" << endl;
  cout << "He leido " << patrones.size() << " ficheros de patron" << endl;

  // Indica cuantas posiciones hay en cada patron
  for (vector<Patron>::iterator it = patrones.begin();
       it != patrones.end();
       it++) 
    {
      cout << " Patron: "<< it->nombre << " con " << it->celdas.size()<< " posiciones"<< endl;
    }
  cout << "-----" << endl;

  return true;
}

// Función para guardar la secuencia almacenada como un nuevo patrón
// Si el flag está activo, guarda también el nuevo patrón en un fichero 
// propio. Luego, a mano, el usuario podrá incluirlo en la base de datos de 
// patrones si quiere
void Learning::guardaSecComoPatron(string name, bool guarda_en_fichero)
{
  Patron *patron = new Patron();

  patron = entrada;
  patron->nombre = name;

  filtraPatron(patron);
  patrones.push_back(*patron);

  if (guarda_en_fichero)
    {
      FILE *f_nombres;
      FILE *f_datos;
      FILE *f_amplitud;
      FILE *f_key;
      FILE *f_cal;
      
      // Guarda los nombres de las trayectorias
      string aux = (name)+".nom";
      if (!(f_nombres = fopen(aux.c_str(),"w")) ) { 
	cout<< "ERROR (learning): No se ha podido crear el fichero "<<aux<<" para guardar la secuencia"<<endl;
	exit(-1);
      }      
      for (unsigned int i=0; i<patron->nombres.size(); i++)
	{
	  fprintf(f_nombres,"%s\n",patron->nombres.at(i).c_str());
	}

      // Guarda las posiciones de celdas
      aux = (name)+".cel";
      if (!(f_datos = fopen(aux.c_str(),"w")) ) { 
	cout<< "ERROR (learning): No se ha podido crear el fichero "<<aux<<" para guardar la secuencia"<<endl;
	exit(-1);
      }      
      fprintf(f_datos,"%d\n",patron->celdas.size()); // guarda el número de celdas de la secuencia
      for (unsigned int i = 0; i<patron->celdas.size(); i++) { //guarda el contenido de las celdas
	vector<CELDA> celdaPaEscribir = patron->celdas.at(i);
	for (unsigned int j=0; j<patron->nombres.size(); j++) {
	  fprintf(f_datos,"%ld ",celdaPaEscribir.at(j));
	}
	fprintf(f_datos,"\n");
      }
      
      // Guarda los valores de amplitud
      aux = (name)+".amp";
      if (!(f_amplitud = fopen(aux.c_str(),"w")) ) { 
	cout<< "ERROR (learning): No se ha podido crear el fichero "<<aux<<" para guardar la secuencia"<<endl;
	exit(-1);
      }      
      if (patron->mov.size()>0) {
	for (unsigned int j=0; j<patron->nombres.size(); j++) {
	  COORDENADA aim;
	  aim = patron->mov.at(j);
	  fprintf(f_amplitud,"%d %d %d\n", aim.x, aim.y, aim.z);
	}
      }

      // Guarda los keypoints
      aux = (name)+".key";
      if (!(f_key = fopen(aux.c_str(),"w")) ) { 
	cout<< "ERROR (learning): No se ha podido crear el fichero "<<aux<<" para guardar la secuencia"<<endl;
	exit(-1);
      } 
      if ( (patron->secs_key_XZ.size()>0)&&(patron->secs_key_YZ.size()>0) ) {
	for (unsigned int j=0; j<patron->nombres.size(); j++) {
	  vector<COORDENADA> keysXZ;
	  vector<COORDENADA> keysYZ;
	  keysXZ = patron->secs_key_XZ.at(j);
	  keysYZ = patron->secs_key_YZ.at(j);
	  fprintf(f_key,"%d\n",keysXZ.size());
	  for (unsigned int i = 0; i<keysXZ.size(); i++) {
	    fprintf(f_key,"%d %d %d\n", keysXZ.at(i).x, keysXZ.at(i).y, keysXZ.at(i).z);
	  }
	  fprintf(f_key,"%d\n",keysYZ.size());
	  for (unsigned int i = 0; i<keysYZ.size(); i++) {
	    fprintf(f_key,"%d %d %d\n", keysYZ.at(i).x, keysYZ.at(i).y, keysYZ.at(i).z);
	  }
	}
      }

      // Guarda los valores de CAL
      aux = (name)+".cal";
      if (!(f_cal = fopen(aux.c_str(),"w")) ) { 
	cout<< "ERROR (learning): No se ha podido crear el fichero "<<aux<<" para guardar la secuencia"<<endl;
	exit(-1);
      } 
      if ( (patron->cal_key_XZ.size()>0)&&(patron->cal_key_YZ.size()>0) ) {
	for (unsigned int j=0; j<patron->nombres.size(); j++) {
	  vector<CAL> keysXZ;
	  vector<CAL> keysYZ;
	  keysXZ = patron->cal_key_XZ.at(j);
	  keysYZ = patron->cal_key_YZ.at(j);
	  fprintf(f_cal,"%d\n",keysXZ.size());
	  for (unsigned int i = 0; i<keysXZ.size(); i++) {
	    fprintf(f_cal,"%06f %06f\n", keysXZ.at(i).curva, keysXZ.at(i).arcodistancia);
	  }
	  fprintf(f_cal,"%d\n",keysYZ.size());
	  for (unsigned int i = 0; i<keysYZ.size(); i++) {
	    fprintf(f_cal,"%06f %06f\n", keysYZ.at(i).curva, keysYZ.at(i).arcodistancia);
	  }
	}
      }

      fclose(f_nombres);
      fclose(f_datos);
      fclose(f_amplitud);
      fclose(f_key);
      if (f_cal) fclose(f_cal);

    }
}

// Función para comenzar a leer una nueva secuencia
void Learning::nuevaEntrada(string nombre, vector<string> nombres)
{
  entrada->clear();
  maximos_entrada.clear();
  minimos_entrada.clear();
  secuencia_entrada.clear();
  entrada->nombre = nombre;
  entrada->nombres = nombres;
  unsigned int tam = entrada->nombres.size();

  entrada->celdas.clear();
  entrada->mov.resize(tam);
  entrada->secs_key_XZ.resize(tam);
  entrada->secs_key_YZ.resize(tam);

  secuencia_entrada.clear();
  maximos_entrada.resize(tam);
  minimos_entrada.resize(tam);
  for (unsigned int i=0; i<tam; i++) {
    maximos_entrada.at(i).x = -10;
    minimos_entrada.at(i).x = 10;
  }
}

// Función para añadir una posición (en mm.) al final de la secuencia, actualizando todos los campos de 'entrada'
bool Learning::incluirEnSecuencia( vector<osg::Vec3f> in )
{
  // Lo primero, comprobar que se están pasando un número correcto de posiciones
  if ( in.size()!=entrada->nombres.size() ) {
    cout << "AVISO (learning): Se ha intentado incluir un vector con un número incorrecto de posiciones"<<endl;
    return false;
  }
  
  // Carga las coordenadas y celdas en la secuencia de entrada
  vector<COORDENADA> vec_coord;
  vector<CELDA> vec_celda;
  COORDENADA coord;
  CELDA celda;
  bool celda_igual = true;

  for (unsigned int i = 0; i<in.size(); i++) {
    coord.x = (int)(in.at(i).x());
    coord.y = (int)(in.at(i).y());
    coord.z = (int)(in.at(i).z());
    vec_coord.push_back(coord); // va rellenando el vector de coordenadas...
    celda = manceldas->calculaCelda(coord);
    vec_celda.push_back(celda); // ... y el de celdas, para guardarlos luego
    if ( entrada->celdas.size()==0 )
      celda_igual = false;
    else {
      if ( celda != entrada->celdas.back().at(i) )
	celda_igual = false; // las celdas se filtran al vuelo... sólo se insertarán las distintas
    }

    // Actualiza los valores de amplitud
    // Al inicio el mínimo es mayor que el máximo
    if ( minimos_entrada.at(i).x>maximos_entrada.at(i).x ) {
      minimos_entrada.at(i).x = maximos_entrada.at(i).x = coord.x;
      minimos_entrada.at(i).y = maximos_entrada.at(i).y = coord.y;
      minimos_entrada.at(i).z = maximos_entrada.at(i).z = coord.z;
    }
    else {
      if ( coord.x<minimos_entrada.at(i).x ) minimos_entrada.at(i).x = coord.x;
      if ( coord.y<minimos_entrada.at(i).y ) minimos_entrada.at(i).y = coord.y;
      if ( coord.z<minimos_entrada.at(i).z ) minimos_entrada.at(i).z = coord.z;

      if ( coord.x>maximos_entrada.at(i).x ) maximos_entrada.at(i).x = coord.x;
      if ( coord.y>maximos_entrada.at(i).y ) maximos_entrada.at(i).y = coord.y;
      if ( coord.z>maximos_entrada.at(i).z ) maximos_entrada.at(i).z = coord.z;
    }
    entrada->mov.at(i).x = maximos_entrada.at(i).x - minimos_entrada.at(i).x;
    entrada->mov.at(i).y = maximos_entrada.at(i).y - minimos_entrada.at(i).y;
    entrada->mov.at(i).z = maximos_entrada.at(i).z - minimos_entrada.at(i).z;
  }
  secuencia_entrada.push_back(vec_coord); 
  if (!celda_igual) entrada->celdas.push_back(vec_celda);

  return true; // si no ha habido problemas, se devuelve true
}

// Función para calcular las funciones de curvatura que definen el gesto
// (K = -1 para curv. adaptativa)
void Learning::calculaCurvaturas(int K, int factor_diezmado)
{

  // Obtiene el tamaño de las trayectorias
  int Tamano = secuencia_entrada.size() / factor_diezmado + 1;

  // Inicia tamaño de contenedores
  Keys_XZ.clear(); Keys_XZ.resize(entrada->nombres.size());
  Keys_YZ.clear(); Keys_YZ.resize(entrada->nombres.size());
  entrada->secs_key_XZ.clear(); entrada->secs_key_XZ.resize(entrada->nombres.size());
  entrada->secs_key_YZ.clear(); entrada->secs_key_YZ.resize(entrada->nombres.size());
  entrada->cal_key_XZ.clear(); entrada->cal_key_XZ.resize(entrada->nombres.size());
  entrada->cal_key_YZ.clear(); entrada->cal_key_YZ.resize(entrada->nombres.size());
  Curvas_XZ.clear(); Curvas_XZ.resize(entrada->nombres.size());
  Curvas_YZ.clear(); Curvas_YZ.resize(entrada->nombres.size());

  for (unsigned int i = 0; i<entrada->nombres.size(); i++)
    {
      // Inicializa los objetos que se utilizarán para calcular las curvaturas
      curvaturaXZ = new CURVATURALEARNER(Tamano,K,2000,1);//10,10,1);  
      curvaturaYZ = new CURVATURALEARNER(Tamano,K,2000,1);//10,10,1);  
      
      // Carga los datos de las curvas en los objetos
      int k = 0;
      for (unsigned int j = 0; j< secuencia_entrada.size(); j++)
	{
	  if ( (j%factor_diezmado) == 0 ) {

	    COORDENADA aux;
	    aux = secuencia_entrada.at(j).at(i); // acceso a la coordenada en la posicion j de la trayectoria i

	    curvaturaXZ->ArrayIN[k].f = (float)(aux.x*100);
	    curvaturaXZ->ArrayIN[k].c = (float)(aux.z*100);
	    
	    curvaturaYZ->ArrayIN[k].f = (float)(aux.y*100);
	    curvaturaYZ->ArrayIN[k].c = (float)(aux.z*100);
	    	    
	    k++;
	  }
	}      

      // Calcula curvaturas
      curvaturaXZ->CalculoCurvatura();
      curvaturaYZ->CalculoCurvatura();
    
      // Salva las curvaturas obtenidas en los vectores correspondientes
      for (k=0; k<Tamano; k++)
	{
	  Curvas_XZ.at(i).push_back(curvaturaXZ->CurvDef[k]);
	  Curvas_YZ.at(i).push_back(curvaturaYZ->CurvDef[k]);
	}

      // Salva los keypoints
      Keys_XZ.at(i) = curvaturaXZ->Keypoints;
      Keys_YZ.at(i) = curvaturaYZ->Keypoints;

      // Y rellena el vector que contiene las posiciones XYZ de esos keypoints
      entrada->secs_key_XZ.at(i).clear();
      entrada->secs_key_YZ.at(i).clear();
      for (unsigned int it = 0; it<Keys_XZ.at(i).size(); it++) {
	if (Keys_XZ.at(i).at(it)<=(int)secuencia_entrada.size()) {
	  // keypoint anterior (válido)
	  COORDENADA punto_ant = entrada->secs_key_XZ.at(i).back();
	  // Guarda los valores XYZ
	  COORDENADA punto = secuencia_entrada.at( Keys_XZ.at(i).at(it) ).at(i);
	  // Si el keypoint es (0,0,0) es que el marcador no se ha leído... por tanto, NO se incluye
	  if ( (punto.x>=0.1)||(punto.y>=0.1)||(punto.z>=0.1) ) {
	    entrada->secs_key_XZ.at(i).push_back( punto ); //secuencia_entrada.at( Keys_XZ.at(i).at(it) ).at(i) );
	    // Guarda los valores AAL
	    if (it!=0) {
	      CAL cal;
	      cal.curva = Curvas_XZ.at(i).at( Keys_XZ.at(i).at(it-1) );
	      cal.arcodistancia = manceldas->distanciaEntreCoordenadas(punto, punto_ant);
	      entrada->cal_key_XZ.at(i).push_back( cal );
	      //cout << "Key:"<<Keys_XZ.at(i).at(it-1)<<" Curva: "<<aal.curva;
	      //cout << " Distancia: "<<aal.arcodistancia<<endl;
	    }
	  }
	}
      }
      for (unsigned int it = 0; it<Keys_YZ.at(i).size(); it++) {
	if (Keys_YZ.at(i).at(it)<=(int)secuencia_entrada.size()) {
	  // keypoint anterior (válido)
	  COORDENADA punto_ant = entrada->secs_key_YZ.at(i).back();
	  // Guarda los valores XYZ
	  COORDENADA punto = secuencia_entrada.at( Keys_YZ.at(i).at(it) ).at(i);
	  // Si el keypoint es (0,0,0) es que el marcador no se ha leído... por tanto, NO se incluye
	  if ( (punto.x>=0.1)||(punto.y>=0.1)||(punto.z>=0.1) ) {
	    entrada->secs_key_YZ.at(i).push_back( punto );
	    // Guarda los valores AAL
	    if (it!=0) {
	      CAL cal;
	      cal.curva = Curvas_YZ.at(i).at( Keys_YZ.at(i).at(it-1) );
	      cal.arcodistancia = manceldas->distanciaEntreCoordenadas(punto, punto_ant);
	      entrada->cal_key_YZ.at(i).push_back( cal );
	    }
	  }
	}
      }
      
      // Obtenidas las curvaturas, se cierran los objetos
      delete(curvaturaXZ);
      delete(curvaturaYZ);
    }

}

// Función para calcular las distancias en amplitud de movimientos
double Learning::parecidoAmplitudes(vector<COORDENADA> a, vector<COORDENADA> b)
{
  if ( (a.size())!=(b.size()) ) {
    cout<<"ERROR (learning. parecidoAmplitudes): Se han intentado comparar vectores de amplitudes de distinto tamaño"<<endl;
    exit(-1);
  }
  
  double parecido_mov_abs = 0.0;
  double parecido_mov_rel = 0.0;
  
  vector<osg::Vec3f> a_rel;
  vector<osg::Vec3f> b_rel;

  COORDENADA a_total; // creo que se crean inicializadas a 0
  COORDENADA b_total;

  for (unsigned int i = 0; i<a.size(); i++) {
    a_total.x += a.at(i).x;
    a_total.y += a.at(i).y;
    a_total.z += a.at(i).z;

    b_total.x += b.at(i).x;
    b_total.y += b.at(i).y;
    b_total.z += b.at(i).z;
  }

  // Obtiene los valores de amplitudes relativas
  for (unsigned int i = 1; i<a.size(); i++) {
    osg::Vec3f aux;
    
    if (a_total.x!=0) aux.x() = (float)(a.at(i).x) / (float)(a_total.x);
    else aux.x() = 0.5;

    if (a_total.y!=0) aux.y() = (float)(a.at(i).y) / (float)(a_total.y);
    else aux.y() = 0.5;

    if (a_total.z!=0) aux.z() = (float)(a.at(i).z) / (float)(a_total.z);
    else aux.z() = 0.5;

    a_rel.push_back(aux);

    if (b_total.x!=0) aux.x() = (float)(b.at(i).x) / (float)(b_total.x);
    else aux.x() = 0.5;

    if (b_total.y!=0) aux.y() = (float)(b.at(i).y) / (float)(b_total.y);
    else aux.y() = 0.5;

    if (b_total.z!=0) aux.z() = (float)(b.at(i).z) / (float)(b_total.z);
    else aux.z() = 0.5;

    b_rel.push_back(aux);
  }

  // Cálculo del parecido según el movimiento relativo
  vector<float> parecidos_rel;
  float parecido_rel_aux;
  for (unsigned int i = 0; i<a_rel.size(); i++) {
    parecido_rel_aux = (fabs(a_rel.at(i).x()-b_rel.at(i).x()) + fabs(a_rel.at(i).y()-b_rel.at(i).y()) + fabs(a_rel.at(i).z()-b_rel.at(i).z())) / 3;
    parecido_rel_aux = (1 - parecido_rel_aux);
    parecidos_rel.push_back(parecido_rel_aux);
  }
  for (unsigned int i = 0; i<parecidos_rel.size(); i++) {
    parecido_mov_rel += pesos.at(i+1)*parecidos_rel.at(i);
  }  
  parecido_mov_rel = 100*(parecido_mov_rel / parecidos_rel.size());

  // Cálculo del parecido según movimientos absolutos
  vector<float> parecidos_abs;
  double parecido_x, parecido_y, parecido_z;
  for (unsigned int i = 0; i<a.size(); i++) {
    if ( (a.at(i).x==0)&&(b.at(i).x==0) )
      parecido_x = 1.0;
    else {
      if (a.at(i).x > b.at(i).x) parecido_x = ((double)b.at(i).x)/((double)a.at(i).x);
      else parecido_x = ((double)a.at(i).x)/((double)b.at(i).x);
    }

    if ( (a.at(i).y==0)&&(b.at(i).y==0) )
      parecido_y = 1.0;
    else {
      if (a.at(i).y > b.at(i).y) parecido_y = ((double)b.at(i).y)/((double)a.at(i).y);
      else parecido_y = ((double)a.at(i).y)/((double)b.at(i).y);
    }

    if ( (a.at(i).z==0)&&(b.at(i).z==0) )
      parecido_z = 1.0;
    else {
      if (a.at(i).z > b.at(i).z) parecido_z = ((double)b.at(i).z)/((double)a.at(i).z);
      else parecido_z = ((double)a.at(i).z)/((double)b.at(i).z);
    }

    parecidos_abs.push_back( (parecido_x + parecido_y + parecido_z)/3 );
  }
  for (unsigned int i = 0; i<parecidos_abs.size(); i++) {
    parecido_mov_abs += pesos.at(i)*parecidos_abs.at(i);
  }  
  parecido_mov_abs = 100*(parecido_mov_abs / parecidos_abs.size());

  return ((parecido_mov_abs+parecido_mov_rel)/2);

}

// Función para calcular las distancias usando DTW sobre los valores XYZ de los Keypoints
double Learning::parecidoDTWxyzKeypoints(vector< vector<COORDENADA> > a_XZ, vector< vector<COORDENADA> > a_YZ,
					     vector< vector<COORDENADA> > b_XZ, vector< vector<COORDENADA> > b_YZ,
					     int marcador_offset)
{
  if ( ((a_XZ.size())!=(b_XZ.size()))||((a_YZ.size())!=(b_YZ.size()))||((a_XZ.size())!=(a_YZ.size())) ) {
    cout<<"ERROR (learning. parecidoDTWxyzKeypoints): Se han intentado comparar vectores de valores XYZ de distinto tamaño"<<endl;
    exit(-1);
  }

  double distancia_key = 0.0;
  int validos = 0;

  int ventana;
  double peso_horizontal, peso_vertical, peso_diagonal;

  double parcXZ, parcYZ;

  COORDENADA offset;
  if (marcador_offset!=-1) {
    // El primer keypoint es el mismo para XZ e YZ, así que da igual de donde se tome
    if ( (a_XZ.at(marcador_offset).size()!=0)&&(b_XZ.at(marcador_offset).size()!=0) ) {
      offset.x = a_XZ.at(marcador_offset).at(0).x - b_XZ.at(marcador_offset).at(0).x;
      offset.y = a_XZ.at(marcador_offset).at(0).y - b_XZ.at(marcador_offset).at(0).y;
      offset.z = a_XZ.at(marcador_offset).at(0).z - b_XZ.at(marcador_offset).at(0).z;
    }
  }
  
  // PRUEBA. QUITAR CUANDO ACABE
  double simXZ, simYZ;
  double distancia_key_sim = 0.0;

  for (unsigned int i = 0; i<a_XZ.size(); i++) {
    parcXZ = 0.0;
    parcYZ = 0.0;

    simXZ = 0.0;
    simYZ = 0.0;

    // Actualiza la distancia 
    if ( (a_XZ.at(i).size()!=0)&&(a_YZ.at(i).size()!=0)&&(b_XZ.at(i).size()!=0)&&(b_YZ.at(i).size()!=0) ) {
      
      vector<COORDENADA> b_XZ_i = b_XZ.at(i);
      vector<COORDENADA> b_YZ_i = b_YZ.at(i);

      // Si hay offset, se aplica a todos los keypoints de la secuencia patrón.
      for (unsigned int k = 0; k<b_XZ_i.size(); k++ ) {
	b_XZ_i.at(k).x += offset.x;   
	b_XZ_i.at(k).y += offset.y;   
	b_XZ_i.at(k).z += offset.z;   
      }
      for (unsigned int k = 0; k<b_YZ_i.size(); k++ ) {
	b_YZ_i.at(k).x += offset.x;
	b_YZ_i.at(k).y += offset.y;
	b_YZ_i.at(k).z += offset.z;      
      }

      // Calcular distancias simetricas
      peso_horizontal = 1; peso_diagonal = 1; peso_vertical = 1;
      ventana = std::max(a_XZ.at(i).size(), b_XZ.at(i).size() );       
      simXZ += DistanciaSecuencias(a_XZ.at(i), 
				    b_XZ_i,//b_XZ.at(i),
				    peso_horizontal,
				    peso_vertical, 
				    peso_diagonal,
				    *distCoord,
				    ventana);

      // Calcular distancias simetricas
      peso_horizontal = 1; peso_diagonal = 1; peso_vertical = 1;
      ventana = std::max(a_YZ.at(i).size(), b_YZ.at(i).size() ); 
      simYZ += DistanciaSecuencias(a_YZ.at(i), 
				    b_YZ_i,//b_YZ.at(i),
				    peso_horizontal,
				    peso_vertical, 
				    peso_diagonal,
				    *distCoord,
				    ventana);

//// Calcular distancias asimetricas
//peso_horizontal = 1; peso_diagonal = 1; peso_vertical = 0;
//ventana = std::max(a_XZ.at(i).size(), b_XZ.at(i).size() );       
//parcXZ += DistanciaSecuencias(a_XZ.at(i), 
//				    b_XZ_i,//b_XZ.at(i),
//				    peso_horizontal,
//				    peso_vertical, 
//				    peso_diagonal,
//				    *distCoord,
//				    ventana);
//
//// Calcular distancias asimetricas nuestras
//peso_horizontal = 0; peso_diagonal = 1; peso_vertical = 1;
//ventana = std::max(a_XZ.at(i).size(), b_XZ.at(i).size() ); 
//parcXZ += DistanciaSecuencias(a_XZ.at(i), 
//				    b_XZ_i,//b_XZ.at(i),
//				    peso_horizontal,
//				    peso_vertical, 
//				    peso_diagonal,
//				    *distCoord,
//				    ventana);
//
//// Calcular distancias asimetricas
//peso_horizontal = 1; peso_diagonal = 1; peso_vertical = 0;
//ventana = std::max(a_YZ.at(i).size(), b_YZ.at(i).size() ); 
//parcYZ += DistanciaSecuencias(a_YZ.at(i), 
//				    b_YZ_i,//b_YZ.at(i),
//				    peso_horizontal,
//				    peso_vertical, 
//				    peso_diagonal,
//				    *distCoord,
//				    ventana);
//
//// Calcular distancias asimetricas nuestras
//peso_horizontal = 0; peso_diagonal = 1; peso_vertical = 1;
//ventana = std::max(a_YZ.at(i).size(), b_YZ.at(i).size() ); 
//parcYZ += DistanciaSecuencias(a_YZ.at(i), 
//				    b_YZ_i,//b_YZ.at(i),
//				    peso_horizontal,
//				    peso_vertical, 
//				    peso_diagonal,
//				    *distCoord,
//				    ventana);

      //cout<<"En: "<<entrada->nombres.at(i)<<" DTW Key.adapt.XYZ. Parec. XZ: "<<parcXZ<<" Parec. YZ: "<<parcYZ<<endl;
      distancia_key_sim += (double)(pesos.at(i))*(simXZ + simYZ)/2;  
      
      //      distancia_key += (double)(pesos.at(i))*(parcXZ + parcYZ)/2;  
      validos++;
    }
  }

  // Normaliza la distancia
  //distancia_key  = distancia_key / validos;

  // PRUEBA
  distancia_key_sim = distancia_key_sim / validos;
  return distancia_key_sim;
  //return distancia_key;

}

// Función para calcular las distancias usando EDR sobre los valores XYZ de los Keypoints
double Learning::parecidoEDRxyzKeypoints(vector< vector<COORDENADA> > a_XZ, vector< vector<COORDENADA> > a_YZ,
					     vector< vector<COORDENADA> > b_XZ, vector< vector<COORDENADA> > b_YZ,
					     double umbralmatchEDR, int marcador_offset)
{

  if ( ((a_XZ.size())!=(b_XZ.size()))||((a_YZ.size())!=(b_YZ.size()))||((a_XZ.size())!=(a_YZ.size())) ) {
    cout<<"ERROR (learning. parecidoEDRxyzKeypoints): Se han intentado comparar vectores de valores XYZ de distinto tamaño"<<endl;
    exit(-1);
  }

  double distancia_key = 0.0;
  int validos = 0;

  double parcXZ, parcYZ;

  COORDENADA offset;
  // El primer keypoint es el mismo para XZ e YZ, así que da igual de donde se tome
  if ( (a_XZ.at(marcador_offset).size()!=0)&&(b_XZ.at(marcador_offset).size()!=0) ) {
    offset.x = a_XZ.at(marcador_offset).at(0).x - b_XZ.at(marcador_offset).at(0).x;
    offset.y = a_XZ.at(marcador_offset).at(0).y - b_XZ.at(marcador_offset).at(0).y;
    offset.z = a_XZ.at(marcador_offset).at(0).z - b_XZ.at(marcador_offset).at(0).z;
  }
  
  for (unsigned int i = 0; i<a_XZ.size(); i++) {
    parcXZ = 0.0;
    parcYZ = 0.0;

    // Actualiza la distancia 
    if ( (a_XZ.at(i).size()!=0)&&(a_YZ.at(i).size()!=0)&&(b_XZ.at(i).size()!=0)&&(b_YZ.at(i).size()!=0) ) {
      
      vector<COORDENADA> b_XZ_i = b_XZ.at(i);
      vector<COORDENADA> b_YZ_i = b_YZ.at(i);

      // Si hay offset, se aplica a todos los keypoints de la secuencia patrón.
      for (unsigned int k = 0; k<b_XZ_i.size(); k++ ) {
	b_XZ_i.at(k).x += offset.x;   
	b_XZ_i.at(k).y += offset.y;   
	b_XZ_i.at(k).z += offset.z;   
      }
      for (unsigned int k = 0; k<b_YZ_i.size(); k++ ) {
	b_YZ_i.at(k).x += offset.x;
	b_YZ_i.at(k).y += offset.y;
	b_YZ_i.at(k).z += offset.z;      
      }

      // Calcular distancias EDR
      parcXZ += DistanciaSecuenciasEDR(a_XZ.at(i), 
				       b_XZ_i,//b_XZ.at(i),
				       umbralmatchEDR,
				       *distCoord);
      
      // Calcular distancias EDR
      parcYZ += DistanciaSecuenciasEDR(a_YZ.at(i), 
				       b_YZ_i,//b_YZ.at(i),
				       umbralmatchEDR,
				       *distCoord);
            
      //cout<<"En: "<<entrada->nombres.at(i)<<" EDR Key.adapt.XYZ. Parec. XZ: "<<parcXZ<<" Parec. YZ: "<<parcYZ<<endl;
      
      distancia_key += (double)(pesos.at(i))*(parcXZ + parcYZ)/2;  
      validos++;
    }
  }

  // Normaliza la distancia
  distancia_key  = distancia_key / validos;
  
  return distancia_key;

}

// Función para calcular las distancias usando ERP sobre los valores XYZ de los Keypoints
double Learning::parecidoERPxyzKeypoints(vector< vector<COORDENADA> > a_XZ, vector< vector<COORDENADA> > a_YZ,
					     vector< vector<COORDENADA> > b_XZ, vector< vector<COORDENADA> > b_YZ,
					     COORDENADA hueco, int marcador_offset)
{

  if ( ((a_XZ.size())!=(b_XZ.size()))||((a_YZ.size())!=(b_YZ.size()))||((a_XZ.size())!=(a_YZ.size())) ) {
    cout<<"ERROR (learning. parecidoERPxyzKeypoints): Se han intentado comparar vectores de valores XYZ de distinto tamaño"<<endl;
    exit(-1);
  }

  double distancia_key = 0.0;
  int validos = 0;

  double parcXZ, parcYZ;

  COORDENADA offset;
  // El primer keypoint es el mismo para XZ e YZ, así que da igual de donde se tome
  if ( (a_XZ.at(marcador_offset).size()!=0)&&(b_XZ.at(marcador_offset).size()!=0) ) {
    offset.x = a_XZ.at(marcador_offset).at(0).x - b_XZ.at(marcador_offset).at(0).x;
    offset.y = a_XZ.at(marcador_offset).at(0).y - b_XZ.at(marcador_offset).at(0).y;
    offset.z = a_XZ.at(marcador_offset).at(0).z - b_XZ.at(marcador_offset).at(0).z;
  }
  
  for (unsigned int i = 0; i<a_XZ.size(); i++) {
    parcXZ = 0.0;
    parcYZ = 0.0;

    // Actualiza la distancia 
    if ( (a_XZ.at(i).size()!=0)&&(a_YZ.at(i).size()!=0)&&(b_XZ.at(i).size()!=0)&&(b_YZ.at(i).size()!=0) ) {
      
      vector<COORDENADA> b_XZ_i = b_XZ.at(i);
      vector<COORDENADA> b_YZ_i = b_YZ.at(i);

      // Si hay offset, se aplica a todos los keypoints de la secuencia patrón.
      for (unsigned int k = 0; k<b_XZ_i.size(); k++ ) {
	b_XZ_i.at(k).x += offset.x;   
	b_XZ_i.at(k).y += offset.y;   
	b_XZ_i.at(k).z += offset.z;   
      }
      for (unsigned int k = 0; k<b_YZ_i.size(); k++ ) {
	b_YZ_i.at(k).x += offset.x;
	b_YZ_i.at(k).y += offset.y;
	b_YZ_i.at(k).z += offset.z;      
      }

      // Calcular distancias ERP
      parcXZ += DistanciaSecuenciasERP(a_XZ.at(i), 
				       b_XZ_i,//b_XZ.at(i),
				       hueco,
				       *distCoord);
      
      // Calcular distancias ERP
      parcYZ += DistanciaSecuenciasERP(a_YZ.at(i), 
				       b_YZ_i,//b_YZ.at(i),
				       hueco,
				       *distCoord);
            
      //cout<<"En: "<<entrada->nombres.at(i)<<" EDR Key.adapt.XYZ. Parec. XZ: "<<parcXZ<<" Parec. YZ: "<<parcYZ<<endl;
      
      distancia_key += (double)(pesos.at(i))*(parcXZ + parcYZ)/2;  
      validos++;
    }
  }

  // Normaliza la distancia
  distancia_key  = distancia_key / validos;
  
  return distancia_key;

}

// Función para calcular las distancias usando LCSS sobre los valores XYZ de los Keypoints
double Learning::parecidoLCSSxyzKeypoints(vector< vector<COORDENADA> > a_XZ, vector< vector<COORDENADA> > a_YZ,
					      vector< vector<COORDENADA> > b_XZ, vector< vector<COORDENADA> > b_YZ,
					      double umbralmatchLCSS, int marcador_offset)
{
  if ( ((a_XZ.size())!=(b_XZ.size()))||((a_YZ.size())!=(b_YZ.size()))||((a_XZ.size())!=(a_YZ.size())) ) {
    cout<<"ERROR (learning. parecidoLCSSxyzKeypoints): Se han intentado comparar vectores de valores XYZ de distinto tamaño"<<endl;
    exit(-1);
  }

  double distancia_LCSS = 0.0;
  int validos = 0;

  double parcXZ, parcYZ;

  COORDENADA offset;
  // El primer keypoint es el mismo para XZ e YZ, así que da igual de donde se tome
  if ( (a_XZ.at(marcador_offset).size()!=0)&&(b_XZ.at(marcador_offset).size()!=0) ) {
    offset.x = a_XZ.at(marcador_offset).at(0).x - b_XZ.at(marcador_offset).at(0).x;
    offset.y = a_XZ.at(marcador_offset).at(0).y - b_XZ.at(marcador_offset).at(0).y;
    offset.z = a_XZ.at(marcador_offset).at(0).z - b_XZ.at(marcador_offset).at(0).z;
  }
  
  for (unsigned int i = 0; i<a_XZ.size(); i++) {
    parcXZ = 0.0;
    parcYZ = 0.0;

    // Actualiza la distancia 
    if ( (a_XZ.at(i).size()!=0)&&(a_YZ.at(i).size()!=0)&&(b_XZ.at(i).size()!=0)&&(b_YZ.at(i).size()!=0) ) {
      
      vector<COORDENADA> b_XZ_i = b_XZ.at(i);
      vector<COORDENADA> b_YZ_i = b_YZ.at(i);

      // Si hay offset, se aplica a todos los keypoints de la secuencia patrón.
      for (unsigned int k = 0; k<b_XZ_i.size(); k++ ) {
	b_XZ_i.at(k).x += offset.x;   
	b_XZ_i.at(k).y += offset.y;   
	b_XZ_i.at(k).z += offset.z;   
      }
      for (unsigned int k = 0; k<b_YZ_i.size(); k++ ) {
	b_YZ_i.at(k).x += offset.x;
	b_YZ_i.at(k).y += offset.y;
	b_YZ_i.at(k).z += offset.z;      
      }

      // Calcular distancias LCSS
      parcXZ += DistanciaSecuenciasLCSS(a_XZ.at(i), 
					b_XZ_i,//b_XZ.at(i),
					umbralmatchLCSS,
					*distCoord);
      
      // Calcular distancias LCSS
      parcYZ += DistanciaSecuenciasLCSS(a_YZ.at(i), 
					b_YZ_i,//b_YZ.at(i),
					umbralmatchLCSS,
					*distCoord);
            
      //cout<<"En: "<<entrada->nombres.at(i)<<" LCSS Key.adapt.XYZ. Parec. XZ: "<<parcXZ<<" Parec. YZ: "<<parcYZ<<endl;
      
      distancia_LCSS += (double)(pesos.at(i))*(parcXZ + parcYZ)/2;  
      validos++;
    }
  }

  // Normaliza la distancia
  distancia_LCSS  = distancia_LCSS / validos;

  return distancia_LCSS;
}

// Función para calcular las distancias usando DTW sobre los valores XYZ de los Keypoints
double Learning::parecidoEuclideoKeypoints(vector< vector<COORDENADA> > a_XZ, vector< vector<COORDENADA> > a_YZ,
					       vector< vector<COORDENADA> > b_XZ, vector< vector<COORDENADA> > b_YZ,
					       int marcador_offset)
{
  if ( ((a_XZ.size())!=(b_XZ.size()))||((a_YZ.size())!=(b_YZ.size()))||((a_XZ.size())!=(a_YZ.size())) ) {
    cout<<"ERROR (learning. parecidoEuclideoKeypoints): Se han intentado comparar vectores de valores XYZ de distinto tamaño"<<endl;
    exit(-1);
  }

  double distancia_euclidea = 0.0;
  int validos = 0;

  double parcXZ, parcYZ;

  COORDENADA offset;
  // El primer keypoint es el mismo para XZ e YZ, así que da igual de donde se tome
  if ( (a_XZ.at(marcador_offset).size()!=0)&&(b_XZ.at(marcador_offset).size()!=0) ) {
    offset.x = a_XZ.at(marcador_offset).at(0).x - b_XZ.at(marcador_offset).at(0).x;
    offset.y = a_XZ.at(marcador_offset).at(0).y - b_XZ.at(marcador_offset).at(0).y;
    offset.z = a_XZ.at(marcador_offset).at(0).z - b_XZ.at(marcador_offset).at(0).z;
  }
  
  for (unsigned int i = 0; i<a_XZ.size(); i++) {
    parcXZ = 0.0;
    parcYZ = 0.0;

    // Actualiza la distancia 
    if ( (a_XZ.at(i).size()!=0)&&(a_YZ.at(i).size()!=0)&&(b_XZ.at(i).size()!=0)&&(b_YZ.at(i).size()!=0) ) {
      
      if ( a_XZ.at(i).size()>=b_XZ.at(i).size() ) {
	for (unsigned int j = 0; j<b_XZ.at(i).size(); j++) {
	  parcXZ += manceldas->distanciaEntreCoordenadasAlCuadrado( a_XZ.at(i).at(j*b_XZ.at(i).size()/a_XZ.at(i).size()), b_XZ.at(i).at(j) );
	}
      }
      else {
	for (unsigned int j = 0; j<a_XZ.at(i).size(); j++) {
	  parcXZ += manceldas->distanciaEntreCoordenadasAlCuadrado( a_XZ.at(i).at(j), b_XZ.at(i).at(j*a_XZ.at(i).size()/b_XZ.at(i).size()) );
	}
      }

      if ( a_YZ.at(i).size()>=b_YZ.at(i).size() ) {
	for (unsigned int j = 0; j<b_YZ.at(i).size(); j++) {
	  parcYZ += manceldas->distanciaEntreCoordenadasAlCuadrado( a_YZ.at(i).at(j*b_YZ.at(i).size()/a_YZ.at(i).size()), b_YZ.at(i).at(j) );
	}
      }
      else {
	for (unsigned int j = 0; j<a_YZ.at(i).size(); j++) {
	  parcYZ += manceldas->distanciaEntreCoordenadasAlCuadrado( a_YZ.at(i).at(j), b_YZ.at(i).at(j*a_YZ.at(i).size()/b_YZ.at(i).size()) );
	}
      }
      
      parcXZ = sqrt(parcXZ);
      parcYZ = sqrt(parcYZ);
      distancia_euclidea += (double)(pesos.at(i))*(parcXZ + parcYZ)/2;  
      validos++;
    }
  }

  // Normaliza la distancia
  distancia_euclidea  = distancia_euclidea / validos; 
  
  return distancia_euclidea;
}

// Calcula distancias entre la secuencia y los patrones
// PARÁMETROS: 
//   int marcador_offset -> marcador cuya primer valor indica el offset XYZ entre la secuencia y los patrones
//                          (a -1 indica que no se ajusta el offset XYZ)
void Learning::calculaDistancias(int marcador_offset)
{
  ptime time_inicio;
  ptime time_fin;
  time_duration intervalo;

  ptime timea;
  ptime timeafin;
  ptime timeb;
  ptime timebfin;

  // Si no hay ningún gesto almacenado en la base de datos, no se ejecuta nada
  if (patrones.size() == 0)
    {
      cout<<"AVISO Learning: No hay ningún gesto almacenado en la base de datos"<<endl;
      return;
    }

  // Si no hay vector de pesos, se construye uno por defecto (todo a 1)
  if (pesos.size()!=entrada->nombres.size()) {
    pesos.clear();
    pesos.resize( entrada->nombres.size(), 1.0 );
  }

  // Captura inicial del tiempo
  time_inicio = microsec_clock::local_time();

  vector<double> distancias_key;
  vector<double> distancias_amp;

  for( vector<Patron>::iterator patron = patrones.begin();
       patron != patrones.end();
       patron++)
    {
      // ------------------------------------------------------------------------
      // PRIMERA MEDIDA DE PARECIDO (LOCAL FEATURE): DISTANCIA ENTRE KEYPOINTS CON EL DTW
      distancias_key.push_back(parecidoDTWxyzKeypoints(entrada->secs_key_XZ,
						       entrada->secs_key_YZ,
						       patron->secs_key_XZ,
						       patron->secs_key_YZ,
						       marcador_offset) );
      // ------------------------------------------------------------------------

      // ------------------------------------------------------------------------
      // SEGUNDA MEDIDA DE PARECIDO (GLOBAL FEATURE): AMPLITUD DE MOVIMIENTOS
      distancias_amp.push_back(parecidoAmplitudes(entrada->mov,
						   patron->mov) );
      // ------------------------------------------------------------------------
      // ------------------------------------------------------------------------
      // REPRESENTACIÓN DE RESULTADOS
      if (num_ganadores==-1) {
	cout << "Distancias al patrón: " << patron->nombre << endl;
	cout << " Parecido según DTW adaptativo (XYZ Keypoints): " << distancias_key.back() << endl;
	cout << " Parecido en amplitud de movimientos: "<< distancias_amp.back() << endl;
	cout << " --- " << endl;
      }
      // ------------------------------------------------------------------------
    }       

  // Obtiene el máximo de las distancias globales y la suma de las distancias locales, necesarias para calcular la 
  // confianza
  double maximoglob = distancias_amp.at(0);
  for (unsigned int i = 1; i<distancias_amp.size(); i++) {
    if (distancias_amp.at(i)>maximoglob) maximoglob = distancias_amp.at(i);
  }
  double sumalocales = 0.0;
  for (unsigned int i = 0; i<distancias_key.size(); i++) {
    sumalocales += distancias_key.at(i);
  }
  
  // Obtiene los valores de confianza
  vector<double> confidences_sin_globales; 
  vector<double> confidences;
  for (unsigned int i = 0; i<distancias_key.size(); i++) {
    double confidence_sin_globales = ( 1 - ((distancias_key.size())*(distancias_key.at(i))/sumalocales) );
    double confidence = (distancias_amp.at(i)/maximoglob)*confidence_sin_globales;
    confidences_sin_globales.push_back( confidence_sin_globales );
    confidences.push_back( confidence );  
  }

  // Captura final del tiempo para calcular el tiempo de procesado por cada frame
  time_fin = microsec_clock::local_time();
  intervalo = time_fin - time_inicio;

  // Representación de resultados
  if (num_ganadores!=-1) {
    vector<string> ganadores;
    vector<double> conf_ganadores;
    float minim;
    int iminim;
    for (int k=0; k<num_ganadores; k++)
      {
	unsigned int j = 0;
	while ( (j<(distancias_key.size()-1))&&(distancias_key.at(j)<0.0) )
	  j++;
	minim = distancias_key.at(j);
	iminim = j;
	for (unsigned int i=j+1; i<distancias_key.size(); i++)
	  {
	    if ( (distancias_key.at(i)<minim)&&(distancias_key.at(i)>=0.0) ) {
	      minim = distancias_key.at(i);
	      iminim = i;
	    }	      
	  }
	ganadores.push_back(patrones.at(iminim).nombre);
	conf_ganadores.push_back(confidences_sin_globales.at(iminim));
	distancias_key.at(iminim) = -1.0;
      }
    cout<<"Winners (DTW + Adaptive Curvature & Keypoints): "<<endl;
    //cout<<"Ganadores según DTW adaptativo (XYZ Keypoints)"<<endl;
    for (unsigned int i=0; i<ganadores.size(); i++)
      cout<<ganadores.at(i)<<" "<<conf_ganadores.at(i)<<endl;

    ganadores.clear();
    conf_ganadores.clear();
    float maxim;
    int imaxim;
    for (int k=0; k<num_ganadores; k++)
      {
	unsigned int j = 0;
	while ( (j<(confidences.size()-1))&&(confidences.at(j)<0.0) )
	  j++;
	maxim = confidences.at(j);
	imaxim = j;
	for (unsigned int i=j+1; i<confidences.size(); i++)
	  {
	    if ( (confidences.at(i)>maxim)&&(confidences.at(i)>=0.0) ) {
	      maxim = confidences.at(i);
	      imaxim = i;
	    }	      
	  }
	ganadores.push_back(patrones.at(imaxim).nombre);
	conf_ganadores.push_back(confidences.at(imaxim));
	confidences.at(imaxim) = -1.0;
      }
    cout<<"Confidence values (DTW + Adaptive Curvatures & Keypoints + Global features)"<<endl;
    //cout<<"Valores de confianza según DTW adaptativo (XYZ Keypoints) + Global features"<<endl;
    for (unsigned int i=0; i<ganadores.size(); i++) {
      if (i==0) {
	ganador = ganadores.at(i);
	conf_ganador = conf_ganadores.at(i);
      }
      else if (i==1) {
	if (ganadores.at(i)==ganador)
	  dos_primeros_ganadores = true;	
	else
	  dos_primeros_ganadores = false;		  
      }
      cout<<ganadores.at(i)<<" "<<conf_ganadores.at(i)<<endl;
    }

  }

  cout<<" Total time computing distances: "<<((double)(intervalo.total_microseconds())/1000000)<<endl;
  cout<<"-------"<<endl;

}

// Calcula las distancias euclídeas entre keypoints
//   int marcador_offset -> marcador cuya primer valor indica el offset XYZ entre la secuencia y los patrones
//                          (a -1 indica que no se ajusta el offset XYZ)
void Learning::calculaDistanciasEuclideas(int marcador_offset)
{
  ptime time_inicio;
  ptime time_fin;
  time_duration intervalo;

  ptime timea;
  ptime timeafin;
  ptime timeb;
  ptime timebfin;

  // Si no hay ningún gesto almacenado en la base de datos, no se ejecuta nada
  if (patrones.size() == 0)
    {
      cout<<"AVISO Learning: No hay ningún gesto almacenado en la base de datos"<<endl;
      return;
    }

  // Si no hay vector de pesos, se construye uno por defecto (todo a 1)
  if (pesos.size()!=entrada->nombres.size()) {
    pesos.clear();
    pesos.resize( entrada->nombres.size(), 1.0 );
  }

  // Captura inicial del tiempo
  time_inicio = microsec_clock::local_time();

  vector<double> distancias_euc;
  vector<double> distancias_amp;

  for( vector<Patron>::iterator patron = patrones.begin();
       patron != patrones.end();
       patron++)
    {
      // ------------------------------------------------------------------------
      // DISTANCIA EUCLÍDEAS ENTRE KEYPOINTS
      distancias_euc.push_back(parecidoEuclideoKeypoints(entrada->secs_key_XZ,
							 entrada->secs_key_YZ,
							 patron->secs_key_XZ,
							 patron->secs_key_YZ,
							 marcador_offset) );
      // ------------------------------------------------------------------------

      // ------------------------------------------------------------------------
      // SEGUNDA MEDIDA DE PARECIDO (GLOBAL FEATURE): AMPLITUD DE MOVIMIENTOS
      distancias_amp.push_back(parecidoAmplitudes(entrada->mov,
						   patron->mov) );
      // ------------------------------------------------------------------------
      
      // ------------------------------------------------------------------------
      // REPRESENTACIÓN DE RESULTADOS
      if (num_ganadores==-1) {
	cout << "Distancias al patrón: " << patron->nombre << endl;
	cout << " Parecido según distancias euclídeas aplicadas a keypoints: " << distancias_euc.back() << endl;
	cout << " Parecido en amplitud de movimientos: "<< distancias_amp.back() << endl;
	cout << " --- " << endl;
      }
      // ------------------------------------------------------------------------

    }
  
  // Obtiene el máximo de las distancias globales y la suma de las distancias locales, necesarias para calcular la 
  // confianza
  double maximoglob = distancias_amp.at(0);
  for (unsigned int i = 1; i<distancias_amp.size(); i++) {
    if (distancias_amp.at(i)>maximoglob) maximoglob = distancias_amp.at(i);
  }
  double sumalocales = 0.0;
  for (unsigned int i = 0; i<distancias_euc.size(); i++) {
    sumalocales += distancias_euc.at(i);
  }
  
  // Obtiene los valores de confianza
  vector<double> confidences_sin_globales; 
  vector<double> confidences;
  for (unsigned int i = 0; i<distancias_euc.size(); i++) {
    double confidence_sin_globales = ( 1 - ((distancias_euc.size())*(distancias_euc.at(i))/sumalocales) );
    double confidence = (distancias_amp.at(i)/maximoglob)*confidence_sin_globales;
    confidences_sin_globales.push_back( confidence_sin_globales );
    confidences.push_back( confidence );  
  }

  // Captura final del tiempo para calcular el tiempo de procesado por cada frame
  time_fin = microsec_clock::local_time();
  intervalo = time_fin - time_inicio;

  // Representación de resultados
  if (num_ganadores!=-1) {
    vector<string> ganadores;
    vector<double> conf_ganadores;
    float minim;
    int iminim;
    for (int k=0; k<num_ganadores; k++)
      {
	unsigned int j = 0;
	while ( (distancias_euc.at(j)<0.0)&&(j<(distancias_euc.size()-1)) )
	  j++;
	minim = distancias_euc.at(j);
	iminim = j;
	for (unsigned int i=j+1; i<distancias_euc.size(); i++)
	  {
	    if ( (distancias_euc.at(i)<minim)&&(distancias_euc.at(i)>=0.0) ) {
	      minim = distancias_euc.at(i);
	      iminim = i;
	    }	      
	  }
	ganadores.push_back(patrones.at(iminim).nombre);
	conf_ganadores.push_back(confidences_sin_globales.at(iminim));
	distancias_euc.at(iminim) = -1.0;
      }
    cout<<"Winners (Euclidean distance + Adaptive Curvatures & Keypoints)"<<endl;
    //cout<<"Ganadores según distancias euclídeas adaptativas (XYZ Keypoints)"<<endl;
    for (unsigned int i=0; i<ganadores.size(); i++)
      cout<<ganadores.at(i)<<" "<<conf_ganadores.at(i)<<endl;

    ganadores.clear();
    conf_ganadores.clear();
    float maxim;
    int imaxim;
    for (int k=0; k<num_ganadores; k++)
      {
	unsigned int j = 0;
	while ( (confidences.at(j)<0.0)&&(j<(confidences.size()-1)) )
	  j++;
	maxim = confidences.at(j);
	imaxim = j;
	for (unsigned int i=j+1; i<confidences.size(); i++)
	  {
	    if ( (confidences.at(i)>maxim)&&(confidences.at(i)>=0.0) ) {
	      maxim = confidences.at(i);
	      imaxim = i;
	    }	      
	  }
	ganadores.push_back(patrones.at(imaxim).nombre);
	conf_ganadores.push_back(confidences.at(imaxim));
	confidences.at(imaxim) = -1.0;
      }
    cout<<"Confidence values (Euclidean distance + Adaptive Curvatures & Keypoints + Global features)"<<endl; 
    //cout<<"Valores de confianza según distancias euclídeas adaptativas (XYZ Keypoints) + Global features"<<endl;
    for (unsigned int i=0; i<ganadores.size(); i++) {
      if (i==0) {
	ganador = ganadores.at(i);
	conf_ganador = conf_ganadores.at(i);
      }
      cout<<ganadores.at(i)<<" "<<conf_ganadores.at(i)<<endl;
    }
  }

  cout<<" Total time computing distances: "<<((double)(intervalo.total_microseconds())/1000000)<<endl;
  cout<<"-------"<<endl;

}

// Calcula las distancias entre keypoints usando el algoritmo EDR para el cálculo de distancias
// Ver artículo: Lei Chen, Tamer Ozsu, Vincent Oria; "Robust and Fast 
//               Similarity Search for Moving Object Trajectories".
//
//   double umbral_match_EDR -> si la distancia entre dos puntos está por debajo de este umbral (en mm.), el EDR
//                              los considerará el mismo punto (coste 0 al hacer el matching)
//   int marcador_offset -> marcador cuya primer valor indica el offset XYZ entre la secuencia y los patrones
//                          (a -1 indica que no se ajusta el offset XYZ)
void Learning::calculaDistanciasEDRxyzKey(double umbral_match_EDR, int marcador_offset)
{
  ptime time_inicio;
  ptime time_fin;
  time_duration intervalo;

  ptime timea;
  ptime timeafin;
  ptime timeb;
  ptime timebfin;

  // Si no hay ningún gesto almacenado en la base de datos, no se ejecuta nada
  if (patrones.size() == 0)
    {
      cout<<"AVISO Learning: No hay ningún gesto almacenado en la base de datos"<<endl;
      return;
    }

  // Si no hay vector de pesos, se construye uno por defecto (todo a 1)
  if (pesos.size()!=entrada->nombres.size()) {
    pesos.clear();
    pesos.resize( entrada->nombres.size(), 1.0 );
  }

  // Captura inicial del tiempo
  time_inicio = microsec_clock::local_time();

  vector<double> distancias_edr;
  vector<double> distancias_amp;

  for( vector<Patron>::iterator patron = patrones.begin();
       patron != patrones.end();
       patron++)
    {
      // ------------------------------------------------------------------------
      // DISTANCIA EDR ENTRE KEYPOINTS
      distancias_edr.push_back(parecidoEDRxyzKeypoints(entrada->secs_key_XZ,
						       entrada->secs_key_YZ,
						       patron->secs_key_XZ,
						       patron->secs_key_YZ,
						       umbral_match_EDR,
						       marcador_offset) );
      // ------------------------------------------------------------------------

      // ------------------------------------------------------------------------
      // SEGUNDA MEDIDA DE PARECIDO (GLOBAL FEATURE): AMPLITUD DE MOVIMIENTOS
      distancias_amp.push_back(parecidoAmplitudes(entrada->mov,
						   patron->mov) );
      // ------------------------------------------------------------------------
            
      // ------------------------------------------------------------------------
      // REPRESENTACIÓN DE RESULTADOS
      if (num_ganadores==-1) {
	cout << "Distancias al patrón: " << patron->nombre << endl;
	cout << " Parecido según EDR adaptativo (XYZ Keypoints): " << distancias_edr.back() << endl;
	cout << " Parecido en amplitud de movimientos: "<< distancias_amp.back() << endl;
	cout << " --- " << endl;
      }
      // ------------------------------------------------------------------------

    }
  
  // Obtiene el máximo de las distancias globales y la suma de las distancias locales, necesarias para calcular la 
  // confianza
  double maximoglob = distancias_amp.at(0);
  for (unsigned int i = 1; i<distancias_amp.size(); i++) {
    if (distancias_amp.at(i)>maximoglob) maximoglob = distancias_amp.at(i);
  }
  double sumalocales = 0.0;
  for (unsigned int i = 0; i<distancias_edr.size(); i++) {
    sumalocales += distancias_edr.at(i);
  }
  
  // Obtiene los valores de confianza
  vector<double> confidences_sin_globales; 
  vector<double> confidences;
  for (unsigned int i = 0; i<distancias_edr.size(); i++) {
    double confidence_sin_globales = ( 1 - ((distancias_edr.size())*(distancias_edr.at(i))/sumalocales) );
    double confidence = (distancias_amp.at(i)/maximoglob)*confidence_sin_globales;
    confidences_sin_globales.push_back( confidence_sin_globales );
    confidences.push_back( confidence );  
  }

  // Captura final del tiempo para calcular el tiempo de procesado por cada frame
  time_fin = microsec_clock::local_time();
  intervalo = time_fin - time_inicio;

  // Representación de resultados
  if (num_ganadores!=-1) {
    vector<string> ganadores;
    vector<double> conf_ganadores;
    float minim;
    int iminim;
    for (int k=0; k<num_ganadores; k++)
      {
	unsigned int j = 0;
	while ( (distancias_edr.at(j)<0.0)&&(j<(distancias_edr.size()-1)) )
	  j++;
	minim = distancias_edr.at(j);
	iminim = j;
	for (unsigned int i=j+1; i<distancias_edr.size(); i++)
	  {
	    if ( (distancias_edr.at(i)<minim)&&(distancias_edr.at(i)>=0.0) ) {
	      minim = distancias_edr.at(i);
	      iminim = i;
	    }	      
	  }
	ganadores.push_back(patrones.at(iminim).nombre);
	conf_ganadores.push_back(confidences_sin_globales.at(iminim));
	distancias_edr.at(iminim) = -1.0;
      }
    cout<<"Winners (EDR + Adaptive Curvatures & Keypoints)"<<endl;
    //cout<<"Ganadores según EDR adaptativo (XYZ Keypoints)"<<endl;
    for (unsigned int i=0; i<ganadores.size(); i++)
      cout<<ganadores.at(i)<<" "<<conf_ganadores.at(i)<<endl;

    ganadores.clear();
    conf_ganadores.clear();
    float maxim;
    int imaxim;
    for (int k=0; k<num_ganadores; k++)
      {
	unsigned int j = 0;
	while ( (confidences.at(j)<0.0)&&(j<(confidences.size()-1)) )
	  j++;
	maxim = confidences.at(j);
	imaxim = j;
	for (unsigned int i=j+1; i<confidences.size(); i++)
	  {
	    if ( (confidences.at(i)>maxim)&&(confidences.at(i)>=0.0) ) {
	      maxim = confidences.at(i);
	      imaxim = i;
	    }	      
	  }
	ganadores.push_back(patrones.at(imaxim).nombre);
	conf_ganadores.push_back(confidences.at(imaxim));
	confidences.at(imaxim) = -1.0;
      }
    cout<<"Confidence values (EDR + Adaptive Curvatures & Keypoints + Global features)"<<endl;
    //cout<<"Valores de confianza según EDR adaptativo (XYZ Keypoints) + Global features"<<endl;
    for (unsigned int i=0; i<ganadores.size(); i++) {
      if (i==0) {
	ganador = ganadores.at(i);
	conf_ganador = conf_ganadores.at(i);
      }
      cout<<ganadores.at(i)<<" "<<conf_ganadores.at(i)<<endl;
    }
  }

  cout<<" Total time computing distances: "<<((double)(intervalo.total_microseconds())/1000000)<<endl;
  cout<<"-------"<<endl;

}

// Calcula las distancias entre keypoints usando el algoritmo ERP para el cálculo de distancias
// Ver artículo: Lei Chen, Tamer Ozsu, Vincent Oria; "Robust and Fast 
//               Similarity Search for Moving Object Trajectories".
//
//   COORDENADA hueco -> Llamado 'g' en los artículos. La penalización por insertar o borrar un valor 
//                       es su distancia al hueco.
//   int marcador_offset -> marcador cuya primer valor indica el offset XYZ entre la secuencia y los patrones
//                          (a -1 indica que no se ajusta el offset XYZ)
void Learning::calculaDistanciasERPxyzKey(int huecox, int huecoy, int huecoz, int marcador_offset)
{
  COORDENADA hueco;
  hueco.x = huecox; hueco.y = huecoy; hueco.z = huecoz;
  
  ptime time_inicio;
  ptime time_fin;
  time_duration intervalo;

  ptime timea;
  ptime timeafin;
  ptime timeb;
  ptime timebfin;

  // Si no hay ningún gesto almacenado en la base de datos, no se ejecuta nada
  if (patrones.size() == 0)
    {
      cout<<"AVISO Learning: No hay ningún gesto almacenado en la base de datos"<<endl;
      return;
    }

  // Si no hay vector de pesos, se construye uno por defecto (todo a 1)
  if (pesos.size()!=entrada->nombres.size()) {
    pesos.clear();
    pesos.resize( entrada->nombres.size(), 1.0 );
  }

  // Captura inicial del tiempo
  time_inicio = microsec_clock::local_time();

  vector<double> distancias_erp;
  vector<double> distancias_amp;

  for( vector<Patron>::iterator patron = patrones.begin();
       patron != patrones.end();
       patron++)
    {
      // ------------------------------------------------------------------------
      // DISTANCIA ERP ENTRE KEYPOINTS
      distancias_erp.push_back(parecidoERPxyzKeypoints(entrada->secs_key_XZ,
						       entrada->secs_key_YZ,
						       patron->secs_key_XZ,
						       patron->secs_key_YZ,
						       hueco, 
						       marcador_offset) );
      // ------------------------------------------------------------------------
      
      // ------------------------------------------------------------------------
      // SEGUNDA MEDIDA DE PARECIDO (GLOBAL FEATURE): AMPLITUD DE MOVIMIENTOS
      distancias_amp.push_back(parecidoAmplitudes(entrada->mov,
						   patron->mov) );
      // ------------------------------------------------------------------------

      // ------------------------------------------------------------------------
      // REPRESENTACIÓN DE RESULTADOS
      if (num_ganadores==-1) {
	cout << "Distancias al patrón: " << patron->nombre << endl;
	cout << " Parecido según ERP adaptativo (XYZ Keypoints): " << distancias_erp.back() << endl;
	cout << " Parecido en amplitud de movimientos: "<< distancias_amp.back() << endl;
	cout << " --- " << endl;
      }
      // ------------------------------------------------------------------------

    }
  
  // Obtiene el máximo de las distancias globales y la suma de las distancias locales, necesarias para calcular la 
  // confianza
  double maximoglob = distancias_amp.at(0);
  for (unsigned int i = 1; i<distancias_amp.size(); i++) {
    if (distancias_amp.at(i)>maximoglob) maximoglob = distancias_amp.at(i);
  }
  double sumalocales = 0.0;
  for (unsigned int i = 0; i<distancias_erp.size(); i++) {
    sumalocales += distancias_erp.at(i);
  }
  
  // Obtiene los valores de confianza
  vector<double> confidences_sin_globales; 
  vector<double> confidences;
  for (unsigned int i = 0; i<distancias_erp.size(); i++) {
    double confidence_sin_globales = ( 1 - ((distancias_erp.size())*(distancias_erp.at(i))/sumalocales) );
    double confidence = (distancias_amp.at(i)/maximoglob)*confidence_sin_globales;
    confidences_sin_globales.push_back( confidence_sin_globales );
    confidences.push_back( confidence );  
  }

  // Captura final del tiempo para calcular el tiempo de procesado por cada frame
  time_fin = microsec_clock::local_time();
  intervalo = time_fin - time_inicio;

  // Representación de resultados
  if (num_ganadores!=-1) {
    vector<string> ganadores;
    vector<double> conf_ganadores;
    float minim;
    int iminim;
    for (int k=0; k<num_ganadores; k++)
      {
	unsigned int j = 0;
	while ( (distancias_erp.at(j)<0.0)&&(j<(distancias_erp.size()-1)) )
	  j++;
	minim = distancias_erp.at(j);
	iminim = j;
	for (unsigned int i=j+1; i<distancias_erp.size(); i++)
	  {
	    if ( (distancias_erp.at(i)<minim)&&(distancias_erp.at(i)>=0.0) ) {
	      minim = distancias_erp.at(i);
	      iminim = i;
	    }	      
	  }
	ganadores.push_back(patrones.at(iminim).nombre);
	conf_ganadores.push_back(confidences_sin_globales.at(iminim));
	distancias_erp.at(iminim) = -1.0;
      }
    cout<<"Winners (ERP + Adaptive Curvatures & Keypoints)"<<endl;
    //cout<<"Ganadores según ERP adaptativo (XYZ Keypoints)"<<endl;
    for (unsigned int i=0; i<ganadores.size(); i++)
      cout<<ganadores.at(i)<<" "<<conf_ganadores.at(i)<<endl;

    ganadores.clear();
    conf_ganadores.clear();
    float maxim;
    int imaxim;
    for (int k=0; k<num_ganadores; k++)
      {
	unsigned int j = 0;
	while ( (confidences.at(j)<0.0)&&(j<(confidences.size()-1)) )
	  j++;
	maxim = confidences.at(j);
	imaxim = j;
	for (unsigned int i=j+1; i<confidences.size(); i++)
	  {
	    if ( (confidences.at(i)>maxim)&&(confidences.at(i)>=0.0) ) {
	      maxim = confidences.at(i);
	      imaxim = i;
	    }	      
	  }
	ganadores.push_back(patrones.at(imaxim).nombre);
	conf_ganadores.push_back(confidences.at(imaxim));
	confidences.at(imaxim) = -1.0;
      }
    cout<<"Confidence values (ERP + Adaptive Curvatures & Keypoints + Global features)"<<endl;
    //cout<<"Valores de confianza según ERP adaptativo (XYZ Keypoints) + Global features"<<endl;
    for (unsigned int i=0; i<ganadores.size(); i++) {
      if (i==0) {
	ganador = ganadores.at(i);
	conf_ganador = conf_ganadores.at(i);
      }
      cout<<ganadores.at(i)<<" "<<conf_ganadores.at(i)<<endl;
    }
  }

  cout<<" Total time computing distances: "<<((double)(intervalo.total_microseconds())/1000000)<<endl;
  cout<<"-------"<<endl;

}

// Calcula las distancias entre keypoints usando el algoritmo LCSS para el cálculo de distancias
// Ver artículo: Lei Chen, Tamer Ozsu, Vincent Oria; "Robust and Fast 
//               Similarity Search for Moving Object Trajectories".
//
//   double umbral_match_LCSS -> si la distancia entre dos puntos está por debajo de este umbral (en mm.), el LCSS
//                               los considerará el mismo punto (coste 0 al hacer el matching)
//   int marcador_offset -> marcador cuya primer valor indica el offset XYZ entre la secuencia y los patrones
//                          (a -1 indica que no se ajusta el offset XYZ)
void Learning::calculaDistanciasLCSSxyzKey(double umbral_match_LCSS, int marcador_offset)
{
  ptime time_inicio;
  ptime time_fin;
  time_duration intervalo;

  ptime timea;
  ptime timeafin;
  ptime timeb;
  ptime timebfin;

  // Si no hay ningún gesto almacenado en la base de datos, no se ejecuta nada
  if (patrones.size() == 0)
    {
      cout<<"AVISO Learning: No hay ningún gesto almacenado en la base de datos"<<endl;
      return;
    }

  // Si no hay vector de pesos, se construye uno por defecto (todo a 1)
  if (pesos.size()!=entrada->nombres.size()) {
    pesos.clear();
    pesos.resize( entrada->nombres.size(), 1.0 );
  }

  // Captura inicial del tiempo
  time_inicio = microsec_clock::local_time();

  vector<double> distancias_lcss;
  vector<double> distancias_amp;

  for( vector<Patron>::iterator patron = patrones.begin();
       patron != patrones.end();
       patron++)
    {
      // ------------------------------------------------------------------------
      // DISTANCIA LCSS ENTRE KEYPOINTS
      distancias_lcss.push_back(parecidoLCSSxyzKeypoints(entrada->secs_key_XZ,
							 entrada->secs_key_YZ,
							 patron->secs_key_XZ,
							 patron->secs_key_YZ,
							 umbral_match_LCSS,
							 marcador_offset) );
      // ------------------------------------------------------------------------
      
      // ------------------------------------------------------------------------
      // SEGUNDA MEDIDA DE PARECIDO (GLOBAL FEATURE): AMPLITUD DE MOVIMIENTOS
      distancias_amp.push_back(parecidoAmplitudes(entrada->mov,
						   patron->mov) );
      // ------------------------------------------------------------------------
      
      // ------------------------------------------------------------------------
      // REPRESENTACIÓN DE RESULTADOS
      if (num_ganadores==-1) {
	cout << "Distancias al patrón: " << patron->nombre << endl;
	cout << " Parecido según LCSS adaptativo (XYZ Keypoints): " << distancias_lcss.back() << endl;
	cout << " Parecido en amplitud de movimientos: "<< distancias_amp.back() << endl;
	cout << " --- " << endl;
      }
      // ------------------------------------------------------------------------

    }
  
  // Obtiene el máximo de las distancias globales y la suma de las distancias locales, necesarias para calcular la 
  // confianza
  double maximoglob = distancias_amp.at(0);
  for (unsigned int i = 1; i<distancias_amp.size(); i++) {
    if (distancias_amp.at(i)>maximoglob) maximoglob = distancias_amp.at(i);
  }
  double maximo_parecido_lcss = 0.0;
  for (unsigned int i = 0; i<distancias_lcss.size(); i++) {
    if (distancias_lcss.at(i)>maximo_parecido_lcss) maximo_parecido_lcss = distancias_lcss.at(i);
  }
  
  // Obtiene los valores de confianza
  vector<double> confidences_sin_globales; 
  vector<double> confidences;
  for (unsigned int i = 0; i<distancias_lcss.size(); i++) {
    double confidence_sin_globales = ( (distancias_lcss.at(i))/maximo_parecido_lcss );
    double confidence = (distancias_amp.at(i)/maximoglob)*confidence_sin_globales;
    confidences_sin_globales.push_back( confidence_sin_globales );
    confidences.push_back( confidence );  
  }

  // Captura final del tiempo para calcular el tiempo de procesado por cada frame
  time_fin = microsec_clock::local_time();
  intervalo = time_fin - time_inicio;

  // Representación de resultados
  if (num_ganadores!=-1) {
    vector<string> ganadores;
    vector<double> conf_ganadores;
    for (int k=0; k<num_ganadores; k++)
      {
	unsigned int j = 0;
	while ( (distancias_lcss.at(j)<0.0)&&(j<(distancias_lcss.size()-1)) )
	  j++;
	float maxim = distancias_lcss.at(j);
	int imaxim = j;
	for (unsigned int i=j+1; i<distancias_lcss.size(); i++)
	  {
	    if ( (distancias_lcss.at(i)>maxim)&&(distancias_lcss.at(i)>=0.0) ) {
	      maxim = distancias_lcss.at(i);
	      imaxim = i;
	    }	      
	  }
	ganadores.push_back(patrones.at(imaxim).nombre);
	conf_ganadores.push_back(confidences_sin_globales.at(imaxim));
	distancias_lcss.at(imaxim) = -1.0;
      }
    cout<<"Winners (LCSS + Adaptive Curvatures & Keypoints)"<<endl;
    //cout<<"Ganadores según LCSS adaptativo (XYZ Keypoints)"<<endl;
    for (unsigned int i=0; i<ganadores.size(); i++)
      cout<<ganadores.at(i)<<" "<<conf_ganadores.at(i)<<endl;

    ganadores.clear();
    conf_ganadores.clear();
    float maxim;
    int imaxim;
    for (int k=0; k<num_ganadores; k++)
      {
	unsigned int j = 0;
	while ( (confidences.at(j)<0.0)&&(j<(confidences.size()-1)) )
	  j++;
	maxim = confidences.at(j);
	imaxim = j;
	for (unsigned int i=j+1; i<confidences.size(); i++)
	  {
	    if ( (confidences.at(i)>maxim)&&(confidences.at(i)>=0.0) ) {
	      maxim = confidences.at(i);
	      imaxim = i;
	    }	      
	  }
	ganadores.push_back(patrones.at(imaxim).nombre);
	conf_ganadores.push_back(confidences.at(imaxim));
	confidences.at(imaxim) = -1.0;
      }
    cout<<"Confidence values (LCSS + Adaptive Curvatures & Keypoints + Global features)"<<endl;
    //cout<<"Valores de confianza según LCSS adaptativo (XYZ Keypoints) + Global features"<<endl;
    for (unsigned int i=0; i<ganadores.size(); i++) {
      if (i==0) {
	ganador = ganadores.at(i);
	conf_ganador = conf_ganadores.at(i);
      }
      cout<<ganadores.at(i)<<" "<<conf_ganadores.at(i)<<endl;
    }
  }

  cout<<" Total time computing distances: "<<((double)(intervalo.total_microseconds())/1000000)<<endl;
  cout<<"-------"<<endl;

}

// -------------------------------------------------------------------------------------
// FUNCIONES DE LA CLASE PROCESOGESTOS
// (Thread para detectar automáticamente el comienzo y el final de un gesto)
// -------------------------------------------------------------------------------------

void ProcesoGestos::run()
{
  while(1)
    {
      // TODETÓ: Añade tu super-codigazo aquí... 
      // o, bueno, en su defecto lo que vomite tu teclado...
      if (esta_quieto)
	{
	  if (movido) // ... si estaba quieto y se ha movido, se pasa al estado 'NO esta_quieto'
	    {
	      esta_quieto = false;
	      intervalo_mic = intervalo_mic * 5; // una vez moviéndose, las comprobaciones se espacian más
	      _frameInicio = _frameNumber;
	      cout<<"--------------->Movement detected. Starting frame is: "<<_frameNumber<<endl;
	    }
	}
      else
	{
	  if (!movido) // ... si se ha dejado de mover, se cambia al estado 'esta_quieto'
	    {
	      esta_quieto = true;
	      intervalo_mic = intervalo_mic / 5; // una vez parado, las comprobaciones son más rápidas
	      _frameFin = _frameNumber;
	      cout<<endl<<"-> Movement detected from: "<<_frameInicio<<" to: "<<_frameFin<<endl;
	      semaforo_fin_gesto = true;
	    }
	}

      // Desactiva el flag de movido
      movido =false;

      // Duerme hasta que pase el intervalo programado
      OpenThreads::Thread::microSleep(intervalo_mic);
      if (_para) return;
    }
}
