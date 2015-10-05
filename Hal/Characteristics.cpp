#include "Characteristics.h"


Characteristics::Characteristics(){
	for(int i=0;i<4;i++){
		Mp[i] = -1;
		Mm[i] = -1;
		Ms[i] = -1;
		vM[i] = -1;
		eM[i] = -1;
	}
}
Characteristics::Characteristics(Block b):
	block(b){
	Recalc();
}

void Characteristics::Recalc(){

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


