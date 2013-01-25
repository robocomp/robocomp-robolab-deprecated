#ifndef ANGULOS_H
#define ANGULOS_H


#define ULTIMO		0
#define INTERMEDIO	1

typedef struct ANGULOS{
	int control;		// Si es = INTERMEDIO => han de llegar más; SI es  = ULTIMO => se acabó
	float angulo[20];
}ANGULOS;

int EnviarAngulos(ANGULOS *ang, int Socket);
int RecibirAngulos(ANGULOS *ang, int Socket);


#endif
