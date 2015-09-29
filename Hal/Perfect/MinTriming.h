#pragma once
#include "../Structures.h"

class MinTriming : public Perfect<Field,BlockLayer>{
private:
	typedef Perfect<Field,BlockLayer> h_type;

	template<size_t WIDTH,size_t HEIGHT>
	int GetMinLength(const Matrix<WIDTH,HEIGHT>& mat)const;

public:

	bool Execution(const Field& field,const BlockLayer& layer);

	MinTriming();
};

template<size_t WIDTH,size_t HEIGHT>
int MinTriming::GetMinLength(const Matrix<WIDTH,HEIGHT>& mat)const{
	Point trim;
	Matrix<WIDTH,HEIGHT> normalized = mat.GetNormalize();
	
	//width
	for(int i=0;i < WIDTH;i++){
		for(int j=0;j < HEIGHT;j++){
			if(normalized[i][j] == true){
				break;
			}
			if(j == HEIGHT - 1){
				trim.x = i;
				i = WIDTH;
			}
		}
	}
	//heigth
	for(int i=0;i < HEIGHT;i++){
		for(int j=0;j < WIDTH;j++){
			if(normalized[j][i] == true){
				break;
			}
			if(j == HEIGHT - 1){
				trim.y = i;
				i = HEIGHT;
			}
		}
	}

	return std::min(trim.x,trim.y);
}




