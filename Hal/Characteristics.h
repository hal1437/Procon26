
#include "Structures.h"

struct Characteristics{
	Block block;
	int Mp[4]={};	//P特徴量
	int Mm[4]={};	//M特徴量
	int Ms[4]={};	//S特徴量

	int vM[4]={};	//凹凸の対応数		Mpi - Mm(i+2)
	int eM[4]={};	//i辺の長さの総和	Mpi + Ms(i−1) + Msi - Mmi
	int Mh=0; 	//穴の数,常に0

	int S_Mp=0;	//Pの総和 ΣMpi
	int S_Mm=0;	//Mの総和 ΣMmi
	int S_Ms=0;	//Sの総和 ΣMsi
	
	void Recalc();

	Characteristics();
	Characteristics(Block b);
};
