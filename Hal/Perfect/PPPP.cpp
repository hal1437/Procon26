
#include "PPPP.h"
#include <iostream>


PPPP::Characteristics::Characteristics(){
	for(int i=0;i<4;i++){
		Mp[i] = -1;
		Mm[i] = -1;
		Ms[i] = -1;
		vM[i] = -1;
		eM[i] = -1;
	}
}
PPPP::Characteristics::Characteristics(Block b):
	block(b){
	Recalc();
}

void PPPP::Characteristics::Recalc(){

	//std::cout << block << std::endl;

	//位相的特徴量の計算
	for(int i = 0;i < BLOCK_HEIGHT+1;i++){
		for(int j = 0;j < BLOCK_WIDTH+1;j++){
			Point seach_list[] = {{-1,-1},{0,-1},{-1,0},{0,0}};
			Point seach_pos(i,j);
			int fill_index = 0;
			bool fill[4]={};
			// fill[0],fill[1]
			// fill[2],fill[3]
			
			//周辺の取得
			for(int k=0;k<4;k++){
				Point near_pos = seach_pos + seach_list[k];
				//ブロック外は空
				if(near_pos.x < 0 || near_pos.y < 0 ||
				   near_pos.x >= BLOCK_WIDTH || near_pos.y >= BLOCK_HEIGHT)fill[k]=false;
				else{
					fill[k] = block[near_pos.y][near_pos.x];
				}
			}
			
			switch(std::count(fill,fill+4,true)){
				case 1://P
					if(fill[1]==true)Mp[0]++;
					if(fill[0]==true)Mp[1]++;
					if(fill[2]==true)Mp[2]++;
					if(fill[3]==true)Mp[3]++;
					S_Mp++;
					break;

				case 2://S
					if(fill[0]==true && fill[1]==true)Ms[0]++;
					if(fill[0]==true && fill[2]==true)Ms[1]++;
					if(fill[2]==true && fill[3]==true)Ms[2]++;
					if(fill[1]==true && fill[3]==true)Ms[3]++;
					S_Mm++;
					break;

				case 3://M
					if(fill[1]==false)Mm[0]++;
					if(fill[0]==false)Mm[1]++;
					if(fill[2]==false)Mm[2]++;
					if(fill[3]==false)Mm[3]++;
					
					break;
					//count==4の特徴量は存在しない
			}
		}
	}
	for(int i=0;i<4;i++){
		vM[i] = Mp[i] - Mm[(i+2)%4];
		eM[i] = Mp[i] + Ms[(i+3)%4] + Ms[i] - Mm[i];
	}

}


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
