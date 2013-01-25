// -*-c++-*-
#ifndef UTILS_H
#define UTILS_H

// Valores Angulo-arco distancia (AAL: Michail Vlachos, "Rotation Invariant Distance Measures for Trajectories")
struct AAL
{
  float angulo; // en radianes
  float arcodistancia; // distancia al siguiente punto. En unidades (mm. para el CODA)
};

// Valores Curvatura-arco distancia 
struct CAL
{
  float curva; // Valor de la función de curvatura. DE 0 A 1.
  float arcodistancia; // distancia al siguiente punto. En unidades (mm. para el CODA)
};

#endif
