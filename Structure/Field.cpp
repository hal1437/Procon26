
#include "Field.h"

Field::Field(){
	std::fill(begin(),end(),Constants::EMPTY);
}
Field::Field(std::initializer_list<std::initializer_list<int>> init){
	int i,j;
	i=0;
	for(std::initializer_list<int> list : init){
		j=0;
		for(int x : list){
			(*this)[i][j] = static_cast<Constants::ZUKU>(x);
			j++;
		}
		i++;
	}
}

Field::~Field(){

}

std::ostream& operator<<(std::ostream& out,Field field){
	for(int i=0;i<FIELD_HEIGHT;i++){
		for(int j=0;j<FIELD_WIDTH;j++){
			out << field[i][j];
		}
		out << "\n";
	}
	return out;
}

