#ifndef TRAIN_H_INCLUDED
#define TRAIN_H_INCLUDED
#include "CPoint3f.h"
#include "CArc.h"
#include "CPointAnnexe.h"

class train{

public:
	CArc ArcSuivi;			//arc sur lequel se déplace le train
	double Vitesse;			//vitesse de déplacement
	CPoint3f Coord;			//position courante
	int IndPASuiv;			//indice du prochain point annexe à atteindre
	train();
	~train();
};

#endif
