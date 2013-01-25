#include "learning.h"
#include <fstream>
#include <math.h>

#define BOOKCHAPTER 1

// Funcion para leer de fichero una Tuple de dos celdas de estas.
istream& operator>>(istream& in, Tuple<CELDA>& par)
{
  string patirar;

  // Esto lee una linea del tipo:
  // Izq: 100795136;Der: 117441280 con los valores en Hexadecimal
  in >> patirar >> hex >> par[0] >> patirar >> hex >> par[1];

  return in;
}

// Cambia los valores del grid de celdas
void Learning::configuraCeldas(int delta_x, int delta_y, int delta_z, int x, int y, int z)
{
  manceldas->cambiarDeltas(delta_x, delta_y, delta_z);
  manceldas->cambiarEspacio(x, y, z);
}

// Función para leer los ficheros de patrón
// Devuelve true si todo va bien o false si hay algún problema que impida leer los patrones
bool Learning::cargaPatrones(string fichero_config)
{

  ifstream ifs_config(fichero_config.c_str());
  
  // Leer el fichero de configuracion que indica quién integra la base de datos de patrones
  vector<string> ficheros_patron;
  copy(istream_iterator<string>(ifs_config), istream_iterator<string>(), back_inserter(ficheros_patron));
  //cout << "PATRONES A LEER: " << endl;
  //copy(ficheros_patron.begin(), ficheros_patron.end(), ostream_iterator<string>(cout, "\n"));

  // Si no hay fichero de patrón, se avisa que no se ha leído nada
  if ( ficheros_patron.size()== 0 )
    {
      cout << "AVISO (learning.cpp): No se ha leído ningún fichero de patrón!!!" << endl;
      return false;
    }

  // Leer cada fichero de patron 
  patrones.resize(ficheros_patron.size());
  vector<Patron>::iterator patron = patrones.begin();
  for (vector<string>::iterator fichero = ficheros_patron.begin();
       fichero != ficheros_patron.end();
       fichero++, patron++) 
    {
      string aux = (*fichero)+".dat";
      ifstream ifs(aux.c_str());
      copy(istream_iterator<Tuple<CELDA> >(ifs), istream_iterator<Tuple<CELDA> >(), back_inserter(patron->ristra));
      patron->nombre = aux;
      // Carga los valores de amplitud de movimientos
      aux = (*fichero)+".amp";
      FILE *fileamp;
      fileamp = fopen(aux.c_str(),"r");
      if (fileamp == NULL) 
	cout << "AVISO (learning.cpp): El fichero de amplitud de mov. de patrón " << aux << " no existe!" << endl;
      else {
	fscanf(fileamp,"%d %d %d\n", &patron->mov_izq.x, &patron->mov_izq.y, &patron->mov_izq.z);
	fscanf(fileamp,"%d %d %d\n", &patron->mov_der.x, &patron->mov_der.y, &patron->mov_der.z);
	fclose(fileamp);
      }
      // Carga los ficheros de keypoints
      aux = (*fichero) + ".key";
      FILE *fichkey;
      fichkey = fopen(aux.c_str(),"r");
      if (fichkey == NULL) {
	cout << "AVISO (learning.cpp): El fichero de keypoints " << aux << " no existe!" << endl;
      }
      else {
	int X,Y,Z;
	COORDENADA coord_aux;
	int cuenta;
	fscanf(fichkey,"%d\n",&cuenta);
	for (int i=0;i<cuenta;i++)
	  {
	    fscanf(fichkey, "%d %d %d\n", &X, &Y, &Z);
	    coord_aux.x = X;
	    coord_aux.y = Y;
	    coord_aux.z = Z;
	    patron->Secuencia_KeyXZ_izq.push_back(coord_aux);
	  }
	fscanf(fichkey,"%d\n",&cuenta);
	for (int i=0;i<cuenta;i++)
	  {
	    fscanf(fichkey, "%d %d %d\n", &X, &Y, &Z);
	    coord_aux.x = X;
	    coord_aux.y = Y;
	    coord_aux.z = Z;
	    patron->Secuencia_KeyXZ_der.push_back(coord_aux);
	  }
	fscanf(fichkey,"%d\n",&cuenta);
	for (int i=0;i<cuenta;i++)
	  {
	    fscanf(fichkey, "%d %d %d\n", &X, &Y, &Z);
	    coord_aux.x = X;
	    coord_aux.y = Y;
	    coord_aux.z = Z;
	    patron->Secuencia_KeyYZ_izq.push_back(coord_aux);
	  }
	fscanf(fichkey,"%d\n",&cuenta);
	for (int i=0;i<cuenta;i++)
	  {
	    fscanf(fichkey, "%d %d %d\n", &X, &Y, &Z);
	    coord_aux.x = X;
	    coord_aux.y = Y;
	    coord_aux.z = Z;
	    patron->Secuencia_KeyYZ_der.push_back(coord_aux);
	  }
	fclose(fichkey);  
      }
    }

  cout << "-----" << endl << "INFORMACION DE LEARNING.CPP. PATRONES DE MOVIMIENTO" << endl;
  cout << "He leido " << patrones.size() << " ficheros de patron" << endl;

  // Indica cuantas posiciones hay en cada patron
  for (vector<Patron>::iterator it = patrones.begin();
       it != patrones.end();
       it++) 
    {
      cout << " Patron: "<< it->nombre << " con " << it->ristra.size()<< " posiciones"<< endl;
    }
  cout << "-----" << endl;
  
  // PRUEBA: Filtrado de patrones
  filtraPatrones();
  FILE *fichetiqueta;
  for (vector<Patron>::iterator it = patrones.begin();
       it != patrones.end();
       it++) 
    {
      cout << " Patron: "<< it->nombre << " con " << it->ristra.size()<< " posiciones"<< endl;
  
      // Salva las etiquetas de las posiciones visitadas
      fichetiqueta = fopen(it->nombre.c_str(),"w");
      for ( vector<Tuple<CELDA> >::iterator ag = it->ristra.begin();
  	    ag != it->ristra.end();
  	    ag++)
  	{
  	  if ( fichetiqueta )
  	    {
  	      Tuple<CELDA> urs;
  	      urs = *ag;
  	      // Almacena las celdas en el fichero correspondiente
  	      fprintf(fichetiqueta, "Izq: %lx;", urs[0]);
  	      fprintf(fichetiqueta, "Der: %lx\n", urs[1]);
  	    }
  	}
      fclose(fichetiqueta);
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
  Patron *nuevo_patron = new Patron();

  nuevo_patron->nombre = name;
  nuevo_patron->ristra = secuencia;
  nuevo_patron->mov_izq = secuencia_mov_izq;
  nuevo_patron->mov_der = secuencia_mov_der;
  nuevo_patron->Secuencia_KeyXZ_izq = Secuencia_KeyXZ_izq;
  nuevo_patron->Secuencia_KeyYZ_izq = Secuencia_KeyYZ_izq;
  nuevo_patron->Secuencia_KeyXZ_der = Secuencia_KeyXZ_der;
  nuevo_patron->Secuencia_KeyYZ_der = Secuencia_KeyYZ_der;

  filtraPatron(nuevo_patron);
  patrones.push_back(*nuevo_patron);

  if (guarda_en_fichero)
    {
      FILE *fichetiqueta;
      // Salva las etiquetas de las posiciones visitadas
      string auxname = name + ".dat";
      fichetiqueta = fopen(auxname.c_str(),"w");
      for ( vector<Tuple<CELDA> >::iterator ag = nuevo_patron->ristra.begin();
  	    ag != nuevo_patron->ristra.end();
  	    ag++)
  	{
  	  if ( fichetiqueta )
  	    {
  	      Tuple<CELDA> urs;
  	      urs = *ag;
  	      // Almacena las celdas en el fichero correspondiente
  	      fprintf(fichetiqueta, "Izq: %lx;", urs[0]);
  	      fprintf(fichetiqueta, "Der: %lx\n", urs[1]);
  	    }
  	}
      fclose(fichetiqueta);

      // Salva la amplitud de los movimientos realizados
      auxname = name + ".amp";
      fichetiqueta = fopen(auxname.c_str(),"w");
      fprintf(fichetiqueta, "%d %d %d\n", secuencia_mov_izq.x, secuencia_mov_izq.y, secuencia_mov_izq.z);
      fprintf(fichetiqueta, "%d %d %d\n", secuencia_mov_der.x, secuencia_mov_der.y, secuencia_mov_der.z);
      fclose(fichetiqueta);  

      // Salva las ristras de keypoints
      if ( (!Secuencia_KeyXZ_izq.empty())&&(!Secuencia_KeyYZ_izq.empty())&&
	   (!Secuencia_KeyXZ_der.empty())&&(!Secuencia_KeyYZ_der.empty()) )
	{
	  auxname = name + ".key";
	  fichetiqueta = fopen(auxname.c_str(),"w");
	  int X,Y,Z;
	  fprintf(fichetiqueta, "%d\n",Secuencia_KeyXZ_izq.size());
	  for (vector<COORDENADA>::iterator it = Secuencia_KeyXZ_izq.begin();
	       it != Secuencia_KeyXZ_izq.end();
	       it++)
	    {
	      X = (*it).x; Y = (*it).y; Z = (*it).z;
	      fprintf(fichetiqueta, "%d %d %d\n", X, Y, Z);
	    }
	  fprintf(fichetiqueta, "%d\n",Secuencia_KeyXZ_der.size());
	  for (vector<COORDENADA>::iterator it = Secuencia_KeyXZ_der.begin();
	       it != Secuencia_KeyXZ_der.end();
	       it++)
	    {
	      X = (*it).x; Y = (*it).y; Z = (*it).z;
	      fprintf(fichetiqueta, "%d %d %d\n", X, Y, Z);
	    }
	  fprintf(fichetiqueta, "%d\n",Secuencia_KeyYZ_izq.size());
	  for (vector<COORDENADA>::iterator it = Secuencia_KeyYZ_izq.begin();
	       it != Secuencia_KeyYZ_izq.end();
	       it++)
	    {
	      X = (*it).x; Y = (*it).y; Z = (*it).z;
	      fprintf(fichetiqueta, "%d %d %d\n", X, Y, Z);
	    }
	  fprintf(fichetiqueta, "%d\n",Secuencia_KeyYZ_der.size());
	  for (vector<COORDENADA>::iterator it = Secuencia_KeyYZ_der.begin();
	       it != Secuencia_KeyYZ_der.end();
	       it++)
	    {
	      X = (*it).x; Y = (*it).y; Z = (*it).z;
	      fprintf(fichetiqueta, "%d %d %d\n", X, Y, Z);
	    }
	  fclose(fichetiqueta);
	}
    }
}

// Función para filtrar la secuencia
void Learning::filtraSecuencia()
{
  Tuple<CELDA> aux;
  Tuple<CELDA> aux2;

  for ( vector<Tuple<CELDA> >::iterator tuple = secuencia.begin();
	tuple != secuencia.end();
	)
    {
      aux2 = *tuple;
      if ( (aux2[0] == aux[0])&&(aux2[1] == aux[1]) ) // si son iguales, se elimina la celda
	secuencia.erase(tuple);
      else // si son distintos, 'aux' pasa a tomar los valores de ese tuple
	{
	  aux = *tuple;
	  tuple++;
	}
    }
}

// Función para filtrar un patrón determinado
void Learning::filtraPatron(Patron *patronco)
{
  Tuple<CELDA> aux;
  Tuple<CELDA> aux2;

  for ( vector<Tuple<CELDA> >::iterator tuple = patronco->ristra.begin();
	tuple != patronco->ristra.end();
	)
    {
      aux2 = *tuple;
      if ( (aux2[0] == aux[0])&&(aux2[1] == aux[1]) ) // si son iguales, se elimina la celda
	patronco->ristra.erase(tuple);
      else // si son distintos, 'aux' pasa a tomar los valores de ese tuple
	{
	  aux = *tuple;
	  tuple++;
	}
    }
}

// Función para reducir el tamaño de los ficheros de patrón eliminando lecturas consecutivas
// en la misma celda, pues el algoritmo de Viterbi no tiene en cuenta el tiempo que el efector 
// final está en cada celda
void Learning::filtraPatrones()
{
  Tuple<CELDA> aux;
  Tuple<CELDA> aux2;

  for( vector<Patron>::iterator patron = patrones.begin();
       patron != patrones.end();
       patron++)
    {
      aux = patron->ristra.front();
      aux[0] = aux[0] + 10;
      for ( vector<Tuple<CELDA> >::iterator tuple = patron->ristra.begin();
	    tuple != patron->ristra.end();
	    )
	{
	  aux2 = *tuple;
	  if ( (aux2[0] == aux[0])&&(aux2[1] == aux[1]) ) // si son iguales, se elimina la celda
	    patron->ristra.erase(tuple);
	  else // si son distintos, 'aux' pasa a tomar los valores de ese tuple
	    {
	      aux = *tuple;
	      tuple++;
	    }
	}
      
    }  
}

// Función para leer un fichero de secuencia.
bool Learning::cargaSecuencia(string fichero_sec)
{
  FILE *fichamp, *fichkey;
  // Leer el fichero de secuencia
  string auxname = fichero_sec + ".dat";
  ifstream ifs(auxname.c_str());
  copy(istream_iterator<Tuple<CELDA> >(ifs), istream_iterator<Tuple<CELDA> >(), back_inserter(secuencia));

  if ( secuencia.size()==0 )
    {
      cout << "AVISO (learning.cpp): La secuencia que se quiere pasar al aprendiz está vacía"<< endl;
      return false;
    }
  else
    {
      bool ret = true;

      // Carga la amplitud de los movimientos realizados
      auxname = fichero_sec + ".amp";
      fichamp = fopen(auxname.c_str(),"r");
      if (fichamp == NULL) {
	cout << "AVISO (learning.cpp): La secuencia que se quiere pasar al aprendiz no tiene fichero de amplitud de movimientos"<< endl;
	ret = false;
      }
      else {
	fscanf(fichamp, "%d %d %d\n", &secuencia_mov_izq.x, &secuencia_mov_izq.y, &secuencia_mov_izq.z);
	fscanf(fichamp, "%d %d %d\n", &secuencia_mov_der.x, &secuencia_mov_der.y, &secuencia_mov_der.z);
	fclose(fichamp);  
      }

      // Carga los keypoints
      auxname = fichero_sec + ".key";
      fichkey = fopen(auxname.c_str(),"r");
      if (fichkey == NULL) {
	cout << "AVISO (learning.cpp): La secuencia que se quiere pasar al aprendiz no tiene fichero de keypoints"<< endl;
	ret = false;
      }
      else {
	int X,Y,Z;
	COORDENADA coord_aux;
	int cuenta;
	fscanf(fichkey,"%d\n",&cuenta);
	for (int i=0;i<cuenta;i++)
	  {
	    fscanf(fichkey, "%d %d %d\n", &X, &Y, &Z);
	    coord_aux.x = X;
	    coord_aux.y = Y;
	    coord_aux.z = Z;
	    Secuencia_KeyXZ_izq.push_back(coord_aux);
	  }
	fscanf(fichkey,"%d\n",&cuenta);
	for (int i=0;i<cuenta;i++)
	  {
	    fscanf(fichkey, "%d %d %d\n", &X, &Y, &Z);
	    coord_aux.x = X;
	    coord_aux.y = Y;
	    coord_aux.z = Z;
	    Secuencia_KeyXZ_der.push_back(coord_aux);
	  }
	fscanf(fichkey,"%d\n",&cuenta);
	for (int i=0;i<cuenta;i++)
	  {
	    fscanf(fichkey, "%d %d %d\n", &X, &Y, &Z);
	    coord_aux.x = X;
	    coord_aux.y = Y;
	    coord_aux.z = Z;
	    Secuencia_KeyYZ_izq.push_back(coord_aux);
	  }
	fscanf(fichkey,"%d\n",&cuenta);
	for (int i=0;i<cuenta;i++)
	  {
	    fscanf(fichkey, "%d %d %d\n", &X, &Y, &Z);
	    coord_aux.x = X;
	    coord_aux.y = Y;
	    coord_aux.z = Z;
	    Secuencia_KeyYZ_der.push_back(coord_aux);
	  }
	fclose(fichkey);  
      }

      return ret;
    }
}

// Función para guardar un fichero de secuencia.
void Learning::guardaSecuencia(string fichero_sec)
{
  FILE *fichetiqueta;
  // Salva las etiquetas de las posiciones visitadas
  string auxname = fichero_sec + ".dat";
  fichetiqueta = fopen(auxname.c_str(),"w");
  for ( vector<Tuple<CELDA> >::iterator ag = secuencia.begin();
	ag != secuencia.end();
	ag++)
    {
      if ( fichetiqueta )
	{
	  Tuple<CELDA> urs;
	  urs = *ag;
	  // Almacena las celdas en el fichero correspondiente
	  fprintf(fichetiqueta, "Izq: %lx;", urs[0]);
	  fprintf(fichetiqueta, "Der: %lx\n", urs[1]);
	}
    }
  fclose(fichetiqueta);

  // Salva la amplitud de los movimientos realizados
  auxname = fichero_sec + ".amp";
  fichetiqueta = fopen(auxname.c_str(),"w");
  fprintf(fichetiqueta, "%d %d %d\n", secuencia_mov_izq.x, secuencia_mov_izq.y, secuencia_mov_izq.z);
  fprintf(fichetiqueta, "%d %d %d\n", secuencia_mov_der.x, secuencia_mov_der.y, secuencia_mov_der.z);
  fclose(fichetiqueta);  

  // Salva las ristras de keypoints
  if ( (!Secuencia_KeyXZ_izq.empty())&&(!Secuencia_KeyYZ_izq.empty())&&
       (!Secuencia_KeyXZ_der.empty())&&(!Secuencia_KeyYZ_der.empty()) )
    {
      auxname = fichero_sec + ".key";
      fichetiqueta = fopen(auxname.c_str(),"w");
      int X,Y,Z;
      fprintf(fichetiqueta, "%d\n",Secuencia_KeyXZ_izq.size());
      for (vector<COORDENADA>::iterator it = Secuencia_KeyXZ_izq.begin();
	   it != Secuencia_KeyXZ_izq.end();
	   it++)
	{
	  X = (*it).x; Y = (*it).y; Z = (*it).z;
	  fprintf(fichetiqueta, "%d %d %d\n", X, Y, Z);
	}
      fprintf(fichetiqueta, "%d\n",Secuencia_KeyXZ_der.size());
      for (vector<COORDENADA>::iterator it = Secuencia_KeyXZ_der.begin();
	   it != Secuencia_KeyXZ_der.end();
	   it++)
	{
	  X = (*it).x; Y = (*it).y; Z = (*it).z;
	  fprintf(fichetiqueta, "%d %d %d\n", X, Y, Z);
	}
      fprintf(fichetiqueta, "%d\n",Secuencia_KeyYZ_izq.size());
      for (vector<COORDENADA>::iterator it = Secuencia_KeyYZ_izq.begin();
	   it != Secuencia_KeyYZ_izq.end();
	   it++)
	{
	  X = (*it).x; Y = (*it).y; Z = (*it).z;
	  fprintf(fichetiqueta, "%d %d %d\n", X, Y, Z);
	}
      fprintf(fichetiqueta, "%d\n",Secuencia_KeyYZ_der.size());
      for (vector<COORDENADA>::iterator it = Secuencia_KeyYZ_der.begin();
	   it != Secuencia_KeyYZ_der.end();
	   it++)
	{
	  X = (*it).x; Y = (*it).y; Z = (*it).z;
	  fprintf(fichetiqueta, "%d %d %d\n", X, Y, Z);
	}
      fclose(fichetiqueta);
    }
}

// Función para añadir una posición al final de la secuencia
void Learning::incluirEnSecuencia(CELDA izq, CELDA der)
{
  Tuple<CELDA> aux;
  aux[0] = izq;
  aux[1] = der;
  secuencia.push_back(aux);

  Tuple<COORDENADA> aux_coord;
  aux_coord[0] = this->manceldas->coordenadaCelda(izq);
  aux_coord[1] = this->manceldas->coordenadaCelda(der);
  secuencia_coord.push_back(aux_coord);
}
void Learning::incluirEnSecuencia(CELDA izq, CELDA der, COORDENADA coord_izq, COORDENADA coord_der)
{
  Tuple<CELDA> aux;
  aux[0] = izq;
  aux[1] = der;
  secuencia.push_back(aux);

  Tuple<COORDENADA> aux_coord;
  aux_coord[0] = coord_izq;
  aux_coord[1] = coord_der;
  secuencia_coord.push_back(aux_coord);
}

// Calcula distancias entre la secuencia y los patrones
void Learning::calculaDistancias()
{
  ptime time_inicio;
  ptime time_fin;
  time_duration intervalo;

  ptime timea;
  ptime timeafin;
  ptime timeb;
  ptime timebfin;

  // Captura inicial del tiempo
  time_inicio = microsec_clock::local_time();

  // Ya que da igual (aunque en la práctica los resultados cambian), para darle más velocidad al proceso, 
  // FILTRAMOS la secuencia de entrada antes de compararla con los patrones.
  filtraSecuencia();  
  
  vector<double> distancias_simetricas;
  vector<double> distancias_asimetricas;
  vector<double> distancias_asimetricas_nuestras;
  vector<double> distancias_medias;
  double peso_horizontal, peso_vertical, peso_diagonal;

  vector<double> distancias_amp;

  vector<double> distancias_key_simetricas;
  vector<double> distancias_key_asimetricas;
  vector<double> distancias_key_asimetricas_nuestras;
  vector<double> distancias_key_medias;
  double parcXZizq, parcXZder, parcYZizq, parcYZder;

  for( vector<Patron>::iterator patron = patrones.begin();
       patron != patrones.end();
       patron++)
    {
      // Calcular una ventana razonable
      int ventana = std::max(patron->ristra.size(), secuencia.size());

//      // ------------------------------------------------------------------------
//      // PRIMERA MEDIDA DE PARECIDO: DISTANCIAS ENTRE TRAYECTORIAS USANDO DTW
//
//      // AMBOS BRAZOS
//      // Calcular distancias simetricas (NO SE UTILIZAN)
//      //      peso_horizontal = 1; peso_diagonal = 2; peso_vertical = 1;
//      //      distancias_simetricas.push_back(DistanciaSecuencias(patron->ristra, 
//      //							  secuencia,
//      //							  peso_horizontal,
//      //							  peso_vertical, 
//      //							  peso_diagonal,
//      //							  *dist2D,
//      //							  ventana));
//
//      // Calcular distancias asimetricas
//      peso_horizontal = 1; peso_diagonal = 1; peso_vertical = 0;
//      distancias_asimetricas.push_back(DistanciaSecuencias(patron->ristra, 
//							   secuencia,
//							   peso_horizontal,
//							   peso_vertical, 
//							   peso_diagonal,
//							   *dist2D,
//							   ventana));
//      // Calcular distancias asimetricas nuestras
//      peso_horizontal = 0; peso_diagonal = 1; peso_vertical = 1;
//      distancias_asimetricas_nuestras.push_back(DistanciaSecuencias(patron->ristra, 
//								    secuencia,
//								    peso_horizontal,
//								    peso_vertical, 
//								    peso_diagonal,
//								    *dist2D,
//								    ventana));				      
//
//      distancias_medias.push_back( ( distancias_asimetricas.back() + distancias_asimetricas_nuestras.back() )/2 );
//      // ------------------------------------------------------------------------

      // ------------------------------------------------------------------------
      // SEGUNDA MEDIDA DE PARECIDO: AMPLITUD DE MOVIMIENTOS
      distancias_amp.push_back(parecidoAmplitudes(secuencia_mov_izq, secuencia_mov_der,
						   patron->mov_izq, patron->mov_der));
      // ------------------------------------------------------------------------

      // ------------------------------------------------------------------------
      // TERCERA MEDIDA DE PARECIDO: COMPARACIÓN DE KEYPOINTS CON EL DTW

      // Calcular distancias asimetricas
      peso_horizontal = 1; peso_diagonal = 1; peso_vertical = 0;
      ventana = std::max(patron->Secuencia_KeyXZ_izq.size(), Secuencia_KeyXZ_izq.size());
      parcXZizq = DistanciaSecuencias(patron->Secuencia_KeyXZ_izq, 
				      Secuencia_KeyXZ_izq,
				      peso_horizontal,
				      peso_vertical, 
				      peso_diagonal,
				      *distCoord,
				      ventana);
      ventana = std::max(patron->Secuencia_KeyXZ_der.size(), Secuencia_KeyXZ_der.size());
      parcXZder = DistanciaSecuencias(patron->Secuencia_KeyXZ_der, 
				      Secuencia_KeyXZ_der,
				      peso_horizontal,
				      peso_vertical, 
				      peso_diagonal,
				      *distCoord,
				      ventana);
      ventana = std::max(patron->Secuencia_KeyYZ_izq.size(), Secuencia_KeyYZ_izq.size());
      parcYZizq = DistanciaSecuencias(patron->Secuencia_KeyYZ_izq, 
				      Secuencia_KeyYZ_izq,
				      peso_horizontal,
				      peso_vertical, 
				      peso_diagonal,
				      *distCoord,
				      ventana);
      ventana = std::max(patron->Secuencia_KeyYZ_der.size(), Secuencia_KeyYZ_der.size());
      parcYZder = DistanciaSecuencias(patron->Secuencia_KeyYZ_der, 
				      Secuencia_KeyYZ_der,
				      peso_horizontal,
				      peso_vertical, 
				      peso_diagonal,
				      *distCoord,
				      ventana);
      distancias_key_asimetricas.push_back( (parcXZizq + parcXZder + parcYZizq + parcYZder)/40000 );

      // Calcular distancias asimetricas nuestras
      peso_horizontal = 0; peso_diagonal = 1; peso_vertical = 1;
      ventana = std::max(patron->Secuencia_KeyXZ_izq.size(), Secuencia_KeyXZ_izq.size());
      parcXZizq = DistanciaSecuencias(patron->Secuencia_KeyXZ_izq, 
				      Secuencia_KeyXZ_izq,
				      peso_horizontal,
				      peso_vertical, 
				      peso_diagonal,
				      *distCoord,
				      ventana);
      ventana = std::max(patron->Secuencia_KeyXZ_der.size(), Secuencia_KeyXZ_der.size());
      parcXZder = DistanciaSecuencias(patron->Secuencia_KeyXZ_der, 
				      Secuencia_KeyXZ_der,
				      peso_horizontal,
				      peso_vertical, 
				      peso_diagonal,
				      *distCoord,
				      ventana);
      ventana = std::max(patron->Secuencia_KeyYZ_izq.size(), Secuencia_KeyYZ_izq.size());
      parcYZizq = DistanciaSecuencias(patron->Secuencia_KeyYZ_izq, 
				      Secuencia_KeyYZ_izq,
				      peso_horizontal,
				      peso_vertical, 
				      peso_diagonal,
				      *distCoord,
				      ventana);
      ventana = std::max(patron->Secuencia_KeyYZ_der.size(), Secuencia_KeyYZ_der.size());
      parcYZder = DistanciaSecuencias(patron->Secuencia_KeyYZ_der, 
				      Secuencia_KeyYZ_der,
				      peso_horizontal,
				      peso_vertical, 
				      peso_diagonal,
				      *distCoord,
				      ventana);
      distancias_key_asimetricas_nuestras.push_back( (parcXZizq + parcXZder + parcYZizq + parcYZder)/40000 );				      

      distancias_medias.push_back( ( distancias_key_asimetricas.back() + distancias_key_asimetricas_nuestras.back() )/2 );
      distancias_key_medias.push_back( ( distancias_key_asimetricas.back() + distancias_key_asimetricas_nuestras.back() )/2 );

      // ------------------------------------------------------------------------

      // ------------------------------------------------------------------------
      // REPRESENTACIÓN DE RESULTADOS
      cout << "Distancias al patrón: " << patron->nombre << endl;
      //cout << " Parecido según algoritmo DTW: " << distancias_medias.back() << endl;
      cout << " Parecido en amplitud de movimientos: "<< distancias_amp.back() << endl;
      cout << " Parecido en distancias entre keypoints: "<<distancias_key_medias.back() << endl;
      //cout << " --------------------- " << endl;
      // ------------------------------------------------------------------------
    }     
  
  // Captura final del tiempo para calcular el tiempo de procesado por cada frame
  time_fin = microsec_clock::local_time();
  intervalo = time_fin - time_inicio;
  cout<<" Tiempo total calculando distancias: "<<((double)(intervalo.total_microseconds())/1000000)<<endl;

  #ifdef BOOKCHAPTER
  // Bucle de obtención de máximos
  double pesominimo = 20000;
  double pesototal = 0;
  int N = 0;
  double max_amplitud = 0.0;
  vector<double>::iterator amplitud = distancias_amp.begin();
  vector<double>::iterator dist;
  for( dist = distancias_medias.begin();
       dist != distancias_medias.end();
       dist++, amplitud++)
    {
      if (*amplitud > max_amplitud)
	max_amplitud = *amplitud;
      if (*dist < pesominimo )
	pesominimo = *dist;
      pesototal = pesototal + *dist;
      N++;
    }

  // Bucle final de obtención de la máxima confianza, en tanto por ciento
  dist = distancias_medias.begin();
  amplitud = distancias_amp.begin();
  double multipl;
  double conf_max = 0.0;
  double conf_actual;
  for ( vector<Patron>::iterator patron = patrones.begin();
	patron != patrones.end();
	patron++)
    {
      multipl = (*amplitud)/max_amplitud;
      conf_actual = 100*multipl*( 1 - (N)*(*dist/pesototal) );
      if (conf_actual < 0.0) conf_actual = 0.0;
      cout << " Valor confianza: " << patron->nombre << " = " << conf_actual << endl;
      if (conf_actual > conf_max ) conf_max = conf_actual;
      dist++;
      amplitud++;
    }
  cout << " --------------------- " << endl;
  cout << " VALOR DE CONFIANZA MÁXIMO: " << conf_max << endl;
  #endif
  //// Representar las distancias a cada minimo.
  //cout << "DISTANCIAS SIMETRICAS: " << endl;
  //copy(distancias_simetricas.begin(), distancias_simetricas.end(), ostream_iterator<double>(cout, "\n"));
  //
  //cout << "DISTANCIAS ASIMETRICAS: " << endl;
  //copy(distancias_asimetricas.begin(), distancias_asimetricas.end(), ostream_iterator<double>(cout, "\n"));
  //
  //cout << "DISTANCIAS ASIMETRICAS NUESTRAS: " << endl;
  //copy(distancias_asimetricas_nuestras.begin(), distancias_asimetricas_nuestras.end(), ostream_iterator<double>(cout, "\n"));

}

// Función para actualizar los valores de amplitud de movimientos de la sec. percibida
void Learning::actualizaAmplitudMovimientos(COORDENADA pos_izq, COORDENADA pos_der)
{
  // Al inicializar, las posiciones mínimas se ponen mayores que las máximas
  if (min_izq.x>max_izq.x)
    {
      min_izq.x = max_izq.x = pos_izq.x;
      min_izq.y = max_izq.y = pos_izq.y;
      min_izq.z = max_izq.z = pos_izq.z;

      min_der.x = max_der.x = pos_der.x;
      min_der.y = max_der.y = pos_der.y;
      min_der.z = max_der.z = pos_der.z;
    }
  else
    {
      if ( pos_izq.x<min_izq.x ) min_izq.x = pos_izq.x;
      if ( pos_izq.y<min_izq.y ) min_izq.y = pos_izq.y;
      if ( pos_izq.z<min_izq.z ) min_izq.z = pos_izq.z;
      if ( pos_izq.x>max_izq.x ) max_izq.x = pos_izq.x;
      if ( pos_izq.y>max_izq.y ) max_izq.y = pos_izq.y;
      if ( pos_izq.z>max_izq.z ) max_izq.z = pos_izq.z;
      
      if ( pos_der.x<min_der.x ) min_der.x = pos_der.x;
      if ( pos_der.y<min_der.y ) min_der.y = pos_der.y;
      if ( pos_der.z<min_der.z ) min_der.z = pos_der.z;
      if ( pos_der.x>max_der.x ) max_der.x = pos_der.x;
      if ( pos_der.y>max_der.y ) max_der.y = pos_der.y;
      if ( pos_der.z>max_der.z ) max_der.z = pos_der.z;
    }

  secuencia_mov_izq.x = max_izq.x - min_izq.x;
  secuencia_mov_izq.y = max_izq.y - min_izq.y;
  secuencia_mov_izq.z = max_izq.z - min_izq.z;

  secuencia_mov_der.x = max_der.x - min_der.x;
  secuencia_mov_der.y = max_der.y - min_der.y;
  secuencia_mov_der.z = max_der.z - min_der.z;

}

void Learning::actualizaAmplitudMovimientos(int izq_x, int izq_y, int izq_z, int der_x, int der_y, int der_z)
{
  // Al inicializar, las posiciones mínimas se ponen mayores que las máximas
  if (min_izq.x>max_izq.x)
    {
      min_izq.x = max_izq.x = izq_x;
      min_izq.y = max_izq.y = izq_y;
      min_izq.z = max_izq.z = izq_z;

      min_der.x = max_der.x = der_x;
      min_der.y = max_der.y = der_y;
      min_der.z = max_der.z = der_z;
    }
  else
    {
      if ( izq_x<min_izq.x ) min_izq.x = izq_x;
      if ( izq_y<min_izq.y ) min_izq.y = izq_y;
      if ( izq_z<min_izq.z ) min_izq.z = izq_z;
      if ( izq_x>max_izq.x ) max_izq.x = izq_x;
      if ( izq_y>max_izq.y ) max_izq.y = izq_y;
      if ( izq_z>max_izq.z ) max_izq.z = izq_z;
      
      if ( der_x<min_der.x ) min_der.x = der_x;
      if ( der_y<min_der.y ) min_der.y = der_y;
      if ( der_z<min_der.z ) min_der.z = der_z;
      if ( der_x>max_der.x ) max_der.x = der_x;
      if ( der_y>max_der.y ) max_der.y = der_y;
      if ( der_z>max_der.z ) max_der.z = der_z;
    }
      
  secuencia_mov_izq.x = max_izq.x - min_izq.x;
  secuencia_mov_izq.y = max_izq.y - min_izq.y;
  secuencia_mov_izq.z = max_izq.z - min_izq.z;

  secuencia_mov_der.x = max_der.x - min_der.x;
  secuencia_mov_der.y = max_der.y - min_der.y;
  secuencia_mov_der.z = max_der.z - min_der.z;

}


// Función para calcular el parecido en amplitud de movimientos
double Learning::parecidoAmplitudes(COORDENADA a_izq, COORDENADA a_der, COORDENADA b_izq, COORDENADA b_der)
{
  double parecido_amp = 0.0;
  double parecido_izq, parecido_der;
  double parecido_x, parecido_y, parecido_z;
  
  COORDENADA a_total, b_total;
  double a_rel_izq[3];
  double a_rel_der[3];
  double b_rel_izq[3];
  double b_rel_der[3];
  double parecido_mov_rel = 0.0;

  // Se calcula cuánto se ha movido cada mano (en porcentaje) respecto al movimiento global...
  // Para ello, se comparan los movimientos en cada eje de cada mano
  a_total.x = a_izq.x + a_der.x;
  a_total.y = a_izq.y + a_der.y;
  a_total.z = a_izq.z + a_der.z;
  b_total.x = b_izq.x + b_der.x;
  b_total.y = b_izq.y + b_der.y;
  b_total.z = b_izq.z + b_der.z;

  if (a_total.x!=0) {
    a_rel_izq[0] = ((double)a_izq.x)/((double)a_total.x);
    a_rel_der[0] = ((double)a_der.x)/((double)a_total.x);
  }
  else {
    a_rel_izq[0] = 0.5;
    a_rel_der[0] = 0.5;
  }
  if (a_total.y!=0) {
    a_rel_izq[1] = ((double)a_izq.y)/((double)a_total.y);
    a_rel_der[1] = ((double)a_der.y)/((double)a_total.y);
  }
  else {
    a_rel_izq[1] = 0.5;
    a_rel_der[1] = 0.5;
  }
  if (a_total.z!=0) {
    a_rel_izq[2] = ((double)a_izq.z)/((double)a_total.z);
    a_rel_der[2] = ((double)a_der.z)/((double)a_total.z);
  }
  else {
    a_rel_izq[2] = 0.5;
    a_rel_der[2] = 0.5;
  }

  if (b_total.x!=0) {
    b_rel_izq[0] = ((double)b_izq.x)/((double)b_total.x);
    b_rel_der[0] = ((double)b_der.x)/((double)b_total.x);
  }
  else {
    b_rel_izq[0] = 0.5;
    b_rel_der[0] = 0.5;
  }
  if (b_total.y!=0) {
    b_rel_izq[1] = ((double)b_izq.y)/((double)b_total.y);
    b_rel_der[1] = ((double)b_der.y)/((double)b_total.y);
  }
  else {
    b_rel_izq[1] = 0.5;
    b_rel_der[1] = 0.5;
  }
  if (b_total.z!=0) {
    b_rel_izq[2] = ((double)b_izq.z)/((double)b_total.z);
    b_rel_der[2] = ((double)b_der.z)/((double)b_total.z);
  }
  else {
    b_rel_izq[2] = 0.5;
    b_rel_der[2] = 0.5;
  }

  parecido_mov_rel = (fabs(a_rel_izq[0]-b_rel_izq[0]) + fabs(a_rel_izq[1]-b_rel_izq[1]) + fabs(a_rel_izq[2]-b_rel_izq[2])) / 3;
  parecido_mov_rel = 100*(1 - parecido_mov_rel);
  cout << "Parecido respecto a mov. relativo : " << parecido_mov_rel << endl;  

  if ((a_izq.x==0)&&(b_izq.x==0))
    parecido_x = 1.0;
  else {
    if (a_izq.x > b_izq.x) parecido_x = ((double)b_izq.x)/((double)a_izq.x);
    else parecido_x = ((double)a_izq.x)/((double)b_izq.x);
  }

  if ((a_izq.y==0)&&(b_izq.y==0))
    parecido_y = 1.0;
  else {
    if (a_izq.y > b_izq.y) parecido_y = ((double)b_izq.y)/((double)a_izq.y);
    else parecido_y = ((double)a_izq.y)/((double)b_izq.y);
  }

  if ((a_izq.z==0)&&(b_izq.z==0))
    parecido_z = 1.0;
  else {
    if (a_izq.z > b_izq.z) parecido_z = ((double)b_izq.z)/((double)a_izq.z);
    else parecido_z = ((double)a_izq.z)/((double)b_izq.z);
  }
  // el parecido total izquierdo es la media de los parecidos en los tres ejes
  parecido_izq = (parecido_x + parecido_y + parecido_z)/3; 

  if ((a_der.x==0)&&(b_der.x==0))
    parecido_x = 1.0;
  else {
    if (a_der.x > b_der.x) parecido_x = ((double)b_der.x)/((double)a_der.x);
    else parecido_x = ((double)a_der.x)/((double)b_der.x);
  }

  if ((a_der.y==0)&&(b_der.y==0))
    parecido_y = 1.0;
  else {
    if (a_der.y > b_der.y) parecido_y = ((double)b_der.y)/((double)a_der.y);
    else parecido_y = ((double)a_der.y)/((double)b_der.y);
  }

  if ((a_der.z==0)&&(b_der.z==0))
    parecido_z = 1.0;
  else {
    if (a_der.z > b_der.z) parecido_z = ((double)b_der.z)/((double)a_der.z);
    else parecido_z = ((double)a_der.z)/((double)b_der.z);
  }
  // el parecido total derecho es la media de los parecidos en los tres ejes
  parecido_der = (parecido_x + parecido_y + parecido_z)/3; 

  // el parecido total es la media entre el parecido izquierdo y el parecido derecho
  parecido_amp = 100*(parecido_izq + parecido_der)/2;
  cout<< "Parecido de amplitudes: "<<parecido_amp<<endl;

  return ((parecido_amp+parecido_mov_rel)/2);
}

void Learning::calculaCurvaturas(int factor_diezmado)
{

  // Obtiene el tamaño de las trayectorias
  int Tamano = secuencia.size() / factor_diezmado + 1;

  // Inicializa los objetos que se utilizarán para calcular las curvaturas
  //curvaturaXZ_izq = new CURVATURALEARNER(Tamano,50,50,1);  
  //curvaturaYZ_izq = new CURVATURALEARNER(Tamano,50,50,1);  
  //curvaturaXZ_der = new CURVATURALEARNER(Tamano,50,50,1);  
  //curvaturaYZ_der = new CURVATURALEARNER(Tamano,50,50,1);  
  curvaturaXZ_izq = new CURVATURALEARNER(Tamano,-1,200,10);//10,10,1);  
  curvaturaYZ_izq = new CURVATURALEARNER(Tamano,-1,200,10);//10,10,1);  
  curvaturaXZ_der = new CURVATURALEARNER(Tamano,-1,200,10);//10,10,1);  
  curvaturaYZ_der = new CURVATURALEARNER(Tamano,-1,200,10);//10,10,1);  

  ptime time_inicio;
  ptime time_fin;
  time_duration intervalo;

  // Captura inicial del tiempo
  time_inicio = microsec_clock::local_time();

  // Carga los datos de las curvas en los objetos
  int k = 0;
  int kk = 0;
  for (vector<Tuple<COORDENADA> >::iterator posicion = secuencia_coord.begin();
       posicion != secuencia_coord.end();
       posicion++, kk++) 
    {
      if ( (kk%factor_diezmado) == 0 ) {

	COORDENADA aux_izq, aux_der;
	aux_izq = (*posicion)[0];
	aux_der = (*posicion)[1];
	
	curvaturaXZ_izq->ArrayIN[k].f = (float)(aux_izq.x);
	curvaturaXZ_izq->ArrayIN[k].c = (float)(aux_izq.z);
	
	curvaturaYZ_izq->ArrayIN[k].f = (float)(aux_izq.y);
	curvaturaYZ_izq->ArrayIN[k].c = (float)(aux_izq.z);

	curvaturaXZ_der->ArrayIN[k].f = (float)(aux_der.x);
	curvaturaXZ_der->ArrayIN[k].c = (float)(aux_der.z);

	curvaturaYZ_der->ArrayIN[k].f = (float)(aux_der.y);
	curvaturaYZ_der->ArrayIN[k].c = (float)(aux_der.z);

	k++;
      }
    }
  
  // Calcula curvaturas
  curvaturaXZ_izq->CalculoCurvatura();
  curvaturaYZ_izq->CalculoCurvatura();
  curvaturaXZ_der->CalculoCurvatura();
  curvaturaYZ_der->CalculoCurvatura();

  // Salva las curvaturas obtenidas en los vectores correspondientes
  CurvaXZ_izq.clear();
  CurvaYZ_izq.clear();
  CurvaXZ_der.clear();
  CurvaYZ_der.clear();
  for (k=0; k<Tamano; k++)
    {
      CurvaXZ_izq.push_back(curvaturaXZ_izq->CurvDef[k]);
      CurvaYZ_izq.push_back(curvaturaYZ_izq->CurvDef[k]);
      CurvaXZ_der.push_back(curvaturaXZ_der->CurvDef[k]);
      CurvaYZ_der.push_back(curvaturaYZ_der->CurvDef[k]);
    }

  // Salva los keypoints
  KeyXZ_izq = curvaturaXZ_izq->Keypoints;
  KeyYZ_izq = curvaturaYZ_izq->Keypoints;
  KeyXZ_der = curvaturaXZ_der->Keypoints;
  KeyYZ_der = curvaturaYZ_der->Keypoints;

  // Y rellena el vector que contiene las posiciones XYZ de esos keypoints
  Secuencia_KeyXZ_izq.clear();
  Secuencia_KeyYZ_izq.clear();
  Secuencia_KeyXZ_der.clear();
  Secuencia_KeyYZ_der.clear();

  for (vector<int>::iterator it = KeyXZ_izq.begin();
       it != KeyXZ_izq.end();
       it++)
    Secuencia_KeyXZ_izq.push_back( secuencia_coord.at((*it))[0] );
  for (vector<int>::iterator it = KeyYZ_izq.begin();
       it != KeyYZ_izq.end();
       it++)
    Secuencia_KeyYZ_izq.push_back( secuencia_coord.at((*it))[0] );

  for (vector<int>::iterator it = KeyXZ_der.begin();
       it != KeyXZ_der.end();
       it++)
    Secuencia_KeyXZ_der.push_back( secuencia_coord.at((*it))[1] );
  for (vector<int>::iterator it = KeyYZ_der.begin();
       it != KeyYZ_der.end();
       it++)
    Secuencia_KeyYZ_der.push_back( secuencia_coord.at((*it))[1] );

  // Captura final del tiempo para calcular el tiempo de procesado por cada frame
  time_fin = microsec_clock::local_time();
  intervalo = time_fin - time_inicio;
  cout<<" Tiempo total calculando curvaturas: "<<((double)(intervalo.total_microseconds())/1000000)<<endl;

  // Obtenidas las curvaturas, se cierran los objetos
  delete(curvaturaXZ_izq);
  delete(curvaturaYZ_izq);
  delete(curvaturaXZ_der);
  delete(curvaturaYZ_der);

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
	      intervalo_mic = intervalo_mic * 4; // una vez moviéndose, las comprobaciones se espacian más
	      _frameInicio = _frameNumber;
	      cout<<"--------------->Detectado inicio de movimiento. En: "<<_frameNumber<<endl;
	    }
	}
      else
	{
	  if (!movido) // ... si se ha dejado de mover, se cambia al estado 'esta_quieto'
	    {
	      esta_quieto = true;
	      intervalo_mic = intervalo_mic / 4; // una vez parado, las comprobaciones son más rápidas
	      _frameFin = _frameNumber;
	      cout<<endl<<"-> Movimiento detectado desde: "<<_frameInicio<<" hasta: "<<_frameFin<<endl;
	    }
	}

      // Desactiva el flag de movido
      movido =false;

      // Duerme hasta que pase el intervalo programado
      OpenThreads::Thread::microSleep(intervalo_mic);
      if (_para) return;
    }
}
