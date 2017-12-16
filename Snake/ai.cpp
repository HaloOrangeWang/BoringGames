#include <total.h>

vector<Point> findNextPointList(Point thispoint,Point lastpoint,ImaginaryMap imap,int food_valid){
    vector<Point> availpointlist = vector<Point>();
    Point pl[4]= {Point(thispoint.x-1,thispoint.y),Point(thispoint.x,thispoint.y-1),Point(thispoint.x+1,thispoint.y),Point(thispoint.x,thispoint.y+1)}; //左上右下四个点
    for(int t=0;t<=3;t++){ //从四个点中筛选出可以通行的点
        if(pl[t].x<=-1||pl[t].x>=MAP_X||pl[t].y<=-1||pl[t].y>=MAP_Y) continue;
        if(food_valid==1){
            if(imap.ditu[pl[t].x][pl[t].y]!=0&&imap.ditu[pl[t].x][pl[t].y]!=3) continue;
        }else{
            if(imap.ditu[pl[t].x][pl[t].y]!=0) continue;
        }
        if(pl[t]==lastpoint) continue;
        availpointlist.push_back(pl[t]);
    }
    return availpointlist;
}

int FindPoint(vector<Point> vp,Point p){
    if(!vp.empty()){
        for(int t=0;t<=vp.size()-1;t++){
            if(vp.at(t)==p){
                return t;
            }
        }
    }
    return -1;
}

/**
 * @brief FindPath
 * @param start
 * @param method 策略：0为寻找两点间最短路径 1为寻找两点间最长路径
 * @param end
 */
vector<Point> FindPath(Point start, Point end, int use_imap, vector<Point> body,int food_valid){
    vector<Point> opentable=vector<Point>();
    vector<Point> closetable=vector<Point>();
    vector<ImaginaryMap> imap_list=vector<ImaginaryMap>();  //每一步对应的虚拟地图
    int fscore[MAP_X][MAP_Y];
    int G[MAP_X][MAP_Y]; //从起点到这里走了多少步
    Point parent[MAP_X][MAP_Y]; //记录每个节点的父节点 (-1,-1)代表没有父节点
    int x,y;
    for(y=0;y<=MAP_Y-1;y++){
        for(x=0;x<=MAP_X-1;x++){
            fscore[x][y]=MAP_X*MAP_Y;
            parent[x][y]=Point(-1,-1);
            G[x][y]=0;
        }
    }
    opentable.push_back(start);  //将开始节点插入到open表中
    imap_list.push_back(ImaginaryMap(body));
    fscore[start.x][start.y]=start-end;
    while(!opentable.empty()){
        Point selectpoint = opentable.at(0);
        vector<Point> isnake=imap_list.at(0).isnake;
        int fm=selectpoint-end;
        vector<Point>::iterator m_distance_location=opentable.begin();
        vector<ImaginaryMap>::iterator m_distance_imap=imap_list.begin();
        vector<ImaginaryMap>::iterator imap_it=imap_list.begin();
        if(opentable.size()>=2){
            for(vector<Point>::iterator t=opentable.begin()+1;t!=opentable.end();t++){ //从open表中选取f值最小的节点
                imap_it++;
                if(fscore[(*t).x][(*t).y]<fm){
                    fm=fscore[(*t).x][(*t).y];
                    selectpoint=(*t);
                    m_distance_location=t;
                    m_distance_imap=imap_it;
                    isnake=(*m_distance_imap).isnake;
                }
            }
        }
        if(selectpoint==end){
            break;
        }
        opentable.erase(m_distance_location);
        closetable.push_back(selectpoint);
        vector<Point> nextpointlist = findNextPointList(selectpoint,parent[selectpoint.x][selectpoint.y],ImaginaryMap(isnake),food_valid);
        imap_list.erase(m_distance_imap);
        if(!nextpointlist.empty()){
            for(int t=0;t<=nextpointlist.size()-1;t++){
                Point p=nextpointlist.at(t);
                if(FindPoint(closetable,p)!=-1) continue;
                int distance=p-end;
                int fpoint=G[selectpoint.x][selectpoint.y]+distance+1;
                int pindex=FindPoint(opentable,p);
                vector<Point> isnake_new=isnake;
                if(use_imap==1){
                    isnake_new.pop_back();
                    isnake_new.insert(isnake_new.begin(),p);
                }
                if(pindex!=-1){
                    if(fpoint<fscore[p.x][p.y]){
                        fscore[p.x][p.y]=fpoint;
                        parent[p.x][p.y]=selectpoint;
                        G[p.x][p.y]=G[selectpoint.x][selectpoint.y]+1;
                        ImaginaryMap imap_new = ImaginaryMap(isnake_new);
                        imap_list.at(pindex)=imap_new;
                    }
                }else{
                    parent[p.x][p.y]=selectpoint;
                    fscore[p.x][p.y]=fpoint;
                    G[p.x][p.y]=G[selectpoint.x][selectpoint.y]+1;
                    opentable.push_back(p);
                    ImaginaryMap imap_new = ImaginaryMap(isnake_new);
                    imap_list.push_back(imap_new);
                }
            }
        }
    }
    vector<Point> result=vector<Point>();
    if(opentable.empty()){ //列表为空 说明不可能到达终点 返回一个空列表
        return result;
    }
    Point cur=end;
    result.push_back(cur);
    while(parent[cur.x][cur.y].x!=-1){
        cur=parent[cur.x][cur.y];
        result.push_back(cur);
    }
    return result;
}

int inaccessable(ImaginaryMap imap, int x, int y){
    if(x<0||x>=MAP_X){
        return 1;
    }
    if(y<0||y>=MAP_Y){
        return 1;
    }
    if(imap.ditu[x][y]==1||imap.ditu[x][y]==2){
        return 1;
    }
    return 0;
}

int JudgeWaySafe(vector<Point> way, int way_to_food, int food_valid){
    //way的第一项为目标 最后一项为当前头部
    vector<Point> body=Snake;
    if(way_to_food==1){
        if(way.size()>=3){
            for(int t=way.size()-2;t>=1;t--){
                body.pop_back();
                body.insert(body.begin(),way.at(t));
            }
        }
        body.insert(body.begin(),way.at(0));
    }else{
        if(way.size()>=2){
            for(int t=way.size()-2;t>=0;t--){
                body.pop_back();
                body.insert(body.begin(),way.at(t));
            }
        }else{
            return 0;
        }
    }
    ImaginaryMap imap=ImaginaryMap(body);
    if(Snake.size()>=0.9*MAP_X*MAP_Y){
//    if(way_to_food==1){
//        cout<<body.at(0).x<<" "<<body.at(0).y<<endl;
//        cout<<inaccessable(imap,body.at(0).x-1,body.at(0).y)<<" "<<inaccessable(imap,body.at(0).x,body.at(0).y-1)<<" "<<inaccessable(imap,body.at(0).x+1,body.at(0).y)<<" "<<inaccessable(imap,body.at(0).x,body.at(0).y+1)<<endl;
//        system("pause");
//    }
    }
    if(inaccessable(imap,body.at(0).x-1,body.at(0).y)==1&&inaccessable(imap,body.at(0).x,body.at(0).y-1)==1&&inaccessable(imap,body.at(0).x+1,body.at(0).y)==1&&inaccessable(imap,body.at(0).x,body.at(0).y+1)==1){
        return 0; //周围四格至少一个可通行
    }
    vector<Point> new_path=FindPath(way.at(0),body.at(body.size()-1),1,body,food_valid);
    if(new_path.empty()){
        return 0;
    }else{
        return 1;
    }
}

Point ChaseTail(){
    ImaginaryMap imap=ImaginaryMap(Snake);
    Point thispoint=Snake.at(0);
    Point lastpoint=Snake.at(1);
    vector<Point> nextpointlist=findNextPointList(thispoint, lastpoint, imap,1);
    if(!nextpointlist.empty()){
        int distance=-1;
        int m_distance_index=-1;
        for(int t=0;t<=nextpointlist.size()-1;t++){
            vector<Point> way={nextpointlist.at(t), Snake.at(0)};
            float straight=0.5*(nextpointlist.at(t).x-Snake.at(0).x==Snake.at(0).x-Snake.at(1).x&&nextpointlist.at(t).y-Snake.at(0).y==Snake.at(0).y-Snake.at(1).y);
            if(JudgeWaySafe(way,0,1)){ //寻找到尾巴的安全路线
                if(nextpointlist.at(t)-Snake.at(Snake.size()-1)+straight>distance){
                    distance=nextpointlist.at(t)-Snake.at(Snake.size()-1);
                    m_distance_index=t;
                }
            }
        }
        if(m_distance_index!=-1){
            return nextpointlist.at(m_distance_index);
        }else{
            return Point(-1,-1);
        }
    }else{
        return Point(-1,-1);
    }
}

Point GetDirectPoint(){
    int m_score=-1;
    vector<Point> close_point_list=vector<Point>();
    vector<Point> next_point_list=vector<Point>();
    Point this_point=Snake.at(0);
    Point m_score_point=Point(-1,-1);
//    ImaginaryMap map=ImaginaryMap(Snake);
    int score_map[MAP_X][MAP_Y];
    int x,y;
    for(y=0;y<=MAP_Y-1;y++){
        for(x=0;x<=MAP_X-1;x++) score_map[x][y]=0;
    }
    for(int t=0;t<=Snake.size()-1;t++){
        score_map[Snake.at(t).x][Snake.at(t).y]=t+1;
    }
    do
    {
        Point pl[4]= {Point(this_point.x-1,this_point.y),Point(this_point.x,this_point.y-1),Point(this_point.x+1,this_point.y),Point(this_point.x,this_point.y+1)}; //左上右下四个点
        for(int t=0;t<=3;t++){
            if(pl[t].x>=0&&pl[t].x<=MAP_X-1&&pl[t].y>=0&&pl[t].y<=MAP_Y-1){
                if(score_map[pl[t].x][pl[t].y]==0){
                    if((FindPoint(close_point_list,pl[t])==-1)&&(FindPoint(next_point_list,pl[t])==-1)){
                        next_point_list.push_back(Point(pl[t].x,pl[t].y));
                    }
                }else{
                    if(score_map[pl[t].x][pl[t].y]>m_score&&score_map[this_point.x][this_point.y]==0){
                        m_score=score_map[pl[t].x][pl[t].y];
                        m_score_point=this_point;
                    }
                }
            }
        }
        if(score_map[this_point.x][this_point.y]==0){
            close_point_list.push_back(this_point);
            next_point_list.erase(next_point_list.begin());
        }
        if(!next_point_list.empty()){
            this_point=next_point_list.at(0);
        }
    }while(!next_point_list.empty());
    return m_score_point;
}

Point Wander(Point direct_point){
    ImaginaryMap imap=ImaginaryMap(Snake);
    Point thispoint=Snake.at(0);
    Point lastpoint=Snake.at(1);
    vector<Point> nextpointlist=findNextPointList(thispoint, lastpoint, imap, 1);
    if(!nextpointlist.empty()){
        int distance=-1;
        int m_distance_index=-1;
        for(int t=0;t<=nextpointlist.size()-1;t++){
            vector<Point> way={nextpointlist.at(t), Snake.at(0)};
            if(JudgeWaySafe(way,0,1)){ //寻找到目标点的安全路线
                if(nextpointlist.at(t)-direct_point>distance){
                    distance=nextpointlist.at(t)-direct_point;
                    m_distance_index=t;
                }
            }
        }
        if(m_distance_index!=-1){
            return nextpointlist.at(m_distance_index);
        }else{
            vector<Point> nextpointlist=findNextPointList(thispoint, lastpoint, imap, 1);
            if(!nextpointlist.empty()){
                return nextpointlist.at(0);
            }
            return Point(-1,-1);
        }
    }else{
        return Point(-1,-1);
    }
}

float AroundScore(int score_map[MAP_X][MAP_Y],int x, int y){
    if(x<0||x>=MAP_X||y<0||y>=MAP_Y){
        return Snake.size()*0.6;
    }
    return score_map[x][y];
}

Point Around(){
    ImaginaryMap imap=ImaginaryMap(Snake);
    Point thispoint=Snake.at(0);
    Point lastpoint=Snake.at(1);
    vector<Point> nextpointlist=findNextPointList(thispoint, lastpoint, imap,1);
    if(!nextpointlist.empty()){
        float m_score=-1;
        int m_score_index=-1;
        for(int t=0;t<=nextpointlist.size()-1;t++){
            vector<Point> isnake=Snake;
            isnake.pop_back();
            isnake.insert(isnake.begin(),nextpointlist.at(t));
            int score_map[MAP_X][MAP_Y];
            int x,y;
            for(y=0;y<=MAP_Y-1;y++){
                for(x=0;x<=MAP_X-1;x++) score_map[x][y]=0;
            }
            for(int t=0;t<=Snake.size()-1;t++){
                score_map[isnake.at(t).x][isnake.at(t).y]=Snake.size()-t;
            }
            score_map[Food.x][Food.y]=Snake.size();
            vector<Point> way={nextpointlist.at(t), Snake.at(0)};
            if(JudgeWaySafe(way,0,1)){ //寻找到尾巴的安全路线
                float score=AroundScore(score_map,nextpointlist.at(t).x-1,nextpointlist.at(t).y)+AroundScore(score_map,nextpointlist.at(t).x,nextpointlist.at(t).y-1)+AroundScore(score_map,nextpointlist.at(t).x+1,nextpointlist.at(t).y)+AroundScore(score_map,nextpointlist.at(t).x,nextpointlist.at(t).y+1);
                float straight=10*(nextpointlist.at(t).x-Snake.at(0).x==Snake.at(0).x-Snake.at(1).x&&nextpointlist.at(t).y-Snake.at(0).y==Snake.at(0).y-Snake.at(1).y);
//                cout<<nextpointlist.at(t).x<<" "<<nextpointlist.at(t).x<<" "<<score<<endl;
//                system("pause");
                if(score+straight>m_score){
                    m_score=score+straight;
                    m_score_index=t;
                }
            }
        }
        if(m_score_index!=-1){
            return nextpointlist.at(m_score_index);
        }else{
            return Point(-1,-1);
        }
    }else{
        return Point(-1,-1);
    }
}

int AI::play(){
    Point current_point=Snake.at(0);
    Point nextpoint=Point(-1,-1);
//    if(Snake.size()>=60){
//        system("pause");
//    }
    if(PlanPath.empty()){
        if(Snake.size()<=0.7*MAP_X*MAP_Y||wander_step>=MAP_X*MAP_Y){
//        if(wander_step>=MAP_X*MAP_Y){
//            cout<<"G"<<endl;
//            system("pause");
//        }
        vector<Point> plan_path;
        if(Snake.size()>=0.8*MAP_X*MAP_Y) plan_path=FindPath(current_point,Food,0,Snake,1);
        else plan_path=FindPath(current_point,Food,1,Snake,1);
        if(!plan_path.empty()){
            if(JudgeWaySafe(plan_path,1,1)){
                PlanPath=plan_path; //找到通往食物的一条路
            }
            if(!PlanPath.empty()){
                PlanPath.pop_back();
            }
        }
        }else{
            wander_step++;
            PlanPath=vector<Point>();
        }
    }
    if(!PlanPath.empty()){
        nextpoint=PlanPath.at(PlanPath.size()-1);
        PlanPath.pop_back();
    }else{
        if(Snake.size()>0.7*MAP_X*MAP_Y){
            nextpoint=Around();
        }else{
            nextpoint=ChaseTail();
        }
        if(nextpoint==Point(-1,-1)){  //也找不到尾巴
//            system("pause");
            if(direct_point==Point(-1,-1)){
                direct_point=GetDirectPoint();
            }
            if(direct_point.x>=0){
                nextpoint=Wander(direct_point);
            }
        }else{
            direct_point=Point(-1,-1);
        }
    }
    if(Food==nextpoint){
        wander_step=0;
    }
    if(nextpoint==Point(-1,-1)){
        return rand()%4;
    }else{
        if(nextpoint.x-current_point.x==-1&&nextpoint.y-current_point.y==0){
            return 0;
        }else if(nextpoint.x-current_point.x==0&&nextpoint.y-current_point.y==-1){
            return 1;
        }else if(nextpoint.x-current_point.x==1&&nextpoint.y-current_point.y==0){
            return 2;
        }else if(nextpoint.x-current_point.x==0&&nextpoint.y-current_point.y==1){
            return 3;
        }else{
            return rand()%4;
        }
    }
}
