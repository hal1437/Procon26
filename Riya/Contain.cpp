
#include "Contain.h"



bool operator<(const Contain::geometry_feature& rhs, const Contain::geometry_feature& lhs){
    return std::accumulate(rhs.first.begin(), rhs.first.end(), 0) + std::accumulate(rhs.second.begin(), rhs.second.end(), 0) < std::accumulate(lhs.first.begin(), lhs.first.end(), 0) + std::accumulate(lhs.second.begin(), lhs.second.end(), 0);
}

Contain::Contain(const Problem& prob):_prob(prob){
    std::vector< std::pair< geometry_feature, std::size_t > > _features_pair;
    _features.resize(prob.Count());
    _features_index.resize(prob.Count());
    
    _features_pair.push_back(std::make_pair(calcGeometryFeature(particalBlock(_prob.GetBlock(0))),0));
    
    for(std::size_t i=1; i<_prob.Count(); i++){
        geometry_feature gf = calcGeometryFeature(particalBlock(_prob.GetBlock(i)));
        for(long j=_features_pair.size()-1; j>=0; j--){
            if(j==0){
                _features_pair.insert(_features_pair.begin(), std::make_pair(gf,i));
                break;
            }
            if(gf < _features_pair[j].first){
                _features_pair.insert(_features_pair.begin()+j+1, std::make_pair(gf,i));
                break;
            }
        }
    }
    
    for(std::size_t i=0; i<_prob.Count(); i++){
        _features[i] = _features_pair[i].first;
        _features_index[i] = _features_pair[i].second;
    }
    
}

bool Contain::isFieldContainAllRemainBlocks(const Field& field,const std::vector<std::vector<int>>& closed,std::size_t index){
    bool isFieldContain=false;
    int N = _prob.Count();
#ifdef USE_PARALLEL
    PARALLEL_FOR(if(reserveBlocks[P_IT] < 0){
        if(isContain(closed,_features[_features_index[P_IT]])){
            isFieldContain = true;
            reserveBlocks[P_IT] = 1; //used
            break;
        }
    });
#else
    for(int i=index; i<N; i++){
        //std::cout << _prob.GetBlock(i) << std::endl;
        if(isContain(closed,_features[_features_index[i]])){
            isFieldContain = true;
            break;
        }
    }
#endif
    return isFieldContain;
}

Contain::closed_range Contain::searchClosedField(const Field& field,const Point& pos){
    std::queue<Point> work;
    Field _field = field;
    square closed_sq;
    
    if(_field[pos.y][pos.x] == 1)std::make_pair(closed_sq, std::vector<std::vector<int>>());
    
    work.push(pos);
    closed_sq.begin = pos; closed_sq.end = pos;
    
    while(!work.empty()){
        if(work.size() > MAX_CLOSED_FIELD_SIZE){closed_sq.begin = Point(-1,-1); return std::make_pair(closed_sq, std::vector<std::vector<int>>());}
        Point pos = work.front();
        
        closed_sq.begin.x = std::min(closed_sq.begin.x,pos.x);
        closed_sq.begin.y = std::min(closed_sq.begin.y,pos.y);
        closed_sq.end.x = std::max(closed_sq.end.x,pos.x);
        closed_sq.end.y = std::max(closed_sq.end.y,pos.y);
        
        CLOCKWISE_FOR(clock){
            Point search_point = pos + clock;
            if(search_point.x < 0 || search_point.x >= FIELD_WIDTH ||
               search_point.y < 0 || search_point.y >= FIELD_HEIGHT )continue;
            if(_field[search_point.y][search_point.x] == false){
                work.push(search_point);
                _field[search_point.y][search_point.x] = true;
            }
        }
        work.pop();
    }
    
    return std::make_pair(closed_sq , xorField(trimField(field, closed_sq),trimField(_field, closed_sq)));
}

bool Contain::Execution(const Field& field,const Transform& trans, std::size_t index){
    Field _field;
    square closed_sq;
    std::vector<Point> reachable = getReachable(field | _prob.GetField(), _prob.GetBlock(index), trans);
    std::vector<closed_range> closed_fields;

    bool isAllFieldContainRemainBlock=true;
    _field = static_cast<Field>(field | _prob.GetField()); //copy
    for(Point& pos: reachable){
        auto closed = searchClosedField(_field,pos);
        if(closed.second.size() == 0) continue;
        closed_fields.push_back(closed);
    }

    //sorting closed field
    std::sort(closed_fields.begin(),closed_fields.end(),[&](const closed_range& rhs,const closed_range& lhs){
        return std::count_if(rhs.second.begin(),rhs.second.end(),[](const std::vector<int>& v){
                                                                                return std::count(v.begin(),v.end(),0);
                                                                          }
                             ) < std::count_if(rhs.second.begin(),rhs.second.end(),[](const std::vector<int>& v){
                                                                                return std::count(v.begin(),v.end(),0);
                                                                          }
                                               );
        }
    );
    
    for(auto& closed: closed_fields){
        isAllFieldContainRemainBlock &= isFieldContainAllRemainBlocks(field,closed.second,index);
        if(!isAllFieldContainRemainBlock)return false;
    }
    
    return isAllFieldContainRemainBlock;
}

Contain::geometry_feature Contain::calcGeometryFeatureField(const std::vector< std::vector<int> >& field){
    std::vector< std::vector<int> > parted_block = particalClosedField(field);
    geometry_feature gf;
    int initial_point,segment_size;
    
    for(int j=0; j<parted_block[0].size(); j++){
        initial_point=-1;
        segment_size=0;
        for(int i=0; i<parted_block.size(); i++){
            if(initial_point == -1 && parted_block[i][j] == 0)initial_point = i;
            if(initial_point != -1 && parted_block[i][j] == 1){segment_size = i - initial_point; initial_point = -1;}
            if(initial_point != -1 && parted_block[i][j] == 0)segment_size = std::max(segment_size ,i - initial_point + 1);
        }
        if(segment_size > 0)gf.first.push_back(segment_size);
    }
    
    for(int i=0; i<parted_block.size(); i++){
        initial_point=-1;
        segment_size=0;
        for(int j=0; j<parted_block[i].size(); j++){
            if(initial_point == -1 && parted_block[i][j] == 0)initial_point = j;
            if(initial_point != -1 && parted_block[i][j] == 1){segment_size = j - initial_point; initial_point = -1;}
            if(initial_point != -1 && parted_block[i][j] == 0)segment_size = std::max(segment_size ,j - initial_point + 1);
        }
        if(segment_size > 0)gf.second.push_back(segment_size);
    }
    
    if(gf.second.size() > gf.first.size())std::swap(gf.first,gf.second);
    
    std::sort(gf.first.begin(),gf.first.end(),std::greater<int>());
    std::sort(gf.second.begin(),gf.second.end(),std::greater<int>());
    
    return gf;
}


Contain::geometry_feature Contain::calcGeometryFeature(const std::vector< std::vector<int> >& block){
    std::vector< std::vector<int> > parted_block = particalBlock(block);
    geometry_feature gf;
    int initial_point,segment_size;
    
    if(block.size()==0)return gf;
    
    for(int j=0; j<parted_block[0].size(); j++){
        initial_point=-1;
        segment_size=0;
        for(int i=0; i<parted_block.size(); i++){
            if(initial_point == -1 && parted_block[i][j] == 1)initial_point = i;
            if(initial_point != -1 && parted_block[i][j] == 1)segment_size = i - initial_point + 1;
        }
        if(segment_size > 0)gf.first.push_back(segment_size);
    }
    
    for(int i=0; i<parted_block.size(); i++){
        initial_point=-1;
        segment_size=0;
        for(int j=0; j<parted_block[i].size(); j++){
            if(initial_point == -1 && parted_block[i][j] == 1)initial_point = j;
            if(initial_point != -1 && parted_block[i][j] == 1)segment_size = j - initial_point + 1;
        }
        if(segment_size > 0)gf.second.push_back(segment_size);
    }
    
    if(gf.second.size() > gf.first.size())std::swap(gf.first,gf.second);
    
    std::sort(gf.first.begin(),gf.first.end(),std::greater<int>());
    std::sort(gf.second.begin(),gf.second.end(),std::greater<int>());
    
    return gf;
}

bool Contain::isContain(const std::vector< std::vector<int> >& field,geometry_feature gf){
    geometry_feature root_gf = calcGeometryFeatureField(field);
    
    if(root_gf.first.size() < gf.first.size() || root_gf.second.size() < gf.second.size())return false;
    
    for(int i=0; i<gf.first.size(); i++){
        if(root_gf.first[i] < gf.first[i])return false;
    }
    for(int i=0; i<gf.second.size(); i++){
        if(root_gf.second[i] < gf.second[i])return false;
    }
    
    return true;
}

std::vector<Point> Contain::getReachable(const Field& field,const Block& block,const Transform& trans){
    std::vector<Point> reachable;
    
    Transform setLarge(Point(1,1),Constants::ANGLE0,false);
    Matrix<BLOCK_WIDTH+2, BLOCK_HEIGHT+2> L_Block;
    
    L_Block.Projection(block,setLarge);
    L_Block = (L_Block.GetReachable()).GetRotate(trans.angle).GetReverse(trans.reverse);
    
    for(int i=0; i<BLOCK_HEIGHT+2; i++){
        for(int j=0; j<BLOCK_WIDTH+2; j++){
            Point onFieldPos = Point(j,i) + trans.pos - Point(1,1);
            if(onFieldPos.x >= 0 && onFieldPos.x < FIELD_WIDTH && onFieldPos.y >= 0 && onFieldPos.y < FIELD_HEIGHT){
                if(L_Block[i][j]==1 && field[onFieldPos.y][onFieldPos.x]==0)reachable.push_back(Point(j,i) + trans.pos - Point(1,1));
            }
        }
    }
    return reachable;
}

