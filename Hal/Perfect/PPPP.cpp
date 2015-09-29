
#include "PPPP.h"
#include <iostream>

bool PPPP::Execution(const Field& field,const BlockLayer& layer){

	//ブロックサイズでの動的計画法で完成候補のブロックを求め、
	//Characteristicsの候補にする。
	
	/*
	std::map<int,Block> block_sizes;	//ブロックの大きさで分類
	std::vector<int> DP_list;			//候補

	for(int i=0;i<layer.size();i++){
		block_sizes[layer[i].matrix.size()] = layer[i].matrix;
	}*/

	Characteristics K(field);	//盤面の位相的特徴量
	Characteristics N;			//ブロックの位相的特徴量の総和

	//Nの算出
	for(int i=0;i<layer.size();i++){
		Characteristics Np(layer[i].matrix);
		for(int j=0;j<4;j++){
			N.Mp[j] += Np.Mp[j];
			N.Mm[j] += Np.Mm[j];
			N.Ms[j] += Np.Ms[j];
			N.vM[j] += Np.vM[j];
			N.eM[j] += Np.eM[j];
		}
		N.S_Mp += Np.S_Mp;
		N.S_Mm += Np.S_Mm;
		N.S_Ms += Np.S_Ms;
	}
	Characteristics delta;	//差分
	for(int i = 0;i < 4;i++){
		delta.Mp[i] = N.Mp[i] - K.Mp[i];
		delta.Mm[i] = N.Mm[i] - K.Mm[i];
		delta.Ms[i] = N.Ms[i] - K.Ms[i];
		delta.vM[i] = N.vM[i] - K.vM[i];
		delta.eM[i] = N.eM[i] - K.eM[i];
	}

	int d_s = 0;
	int d_p = 0;
	int d_m = 0;
	for(int i = 0;i < 4;i++){
		d_p += delta.Mp[i];
		d_m += delta.Mp[i];
		d_s += delta.Mp[i];
	}

	int e = delta.eM[0];
	int s = std::max(0, d_s);
	int m = std::max(0,-d_m);


	//第一段盤面評価
	std::cout << 
	(    e >= 0 && 
			e - m >= 0 && 
			2*e - (s+m) >= 0);

	return (    e >= 0 && 
			e - m >= 0 && 
			2*e - (s+m) >= 0);
}

PPPP::PPPP(){
}
