#include <total.h>

/**
 * @brief print_map 显示
 */
void MapShow(int print_map[MAP_X][MAP_Y]){
    int x,y;
    //system("clear"); //linux
    system("cls"); //windows
    for(x=0;x<=MAP_X+1;x++){
        printf("█");
    }
    printf("\n");
    for(y=0;y<=MAP_Y-1;y++){
        printf("█");
        for(x=0;x<=MAP_X-1;x++){
            switch(print_map[x][y]){
            case 0: //空地
                printf("  ");
                break;
            case 1: //头部
                printf("●");
                break;
            case 2: //身
                printf("※");
                break;
            case 3: //食物
                printf("□");
                break;
            }
        }
        printf("█\n");
    }
    for(x=0;x<=MAP_X+1;x++){
        printf("█");
    }
    printf("\n");
}

void PrintMap(){
    int print_map[MAP_X][MAP_Y];
    int x,y;
    for(y=0;y<=MAP_Y-1;y++){
        for(x=0;x<=MAP_X-1;x++){
            print_map[x][y]=0;
        }
    }
    for(int t=1;t<=Snake.size()-1;t++){
        print_map[Snake[t].x][Snake[t].y]=2;
    }
    if(Snake.at(0).x>=0&&Snake.at(0).x<=MAP_X-1&&Snake.at(0).y>=0&&Snake.at(0).y<=MAP_Y-1) print_map[Snake.at(0).x][Snake.at(0).y]=1;
    print_map[Food.x][Food.y]=3;
    MapShow(print_map);
}

int game_move(int drction){
    //方向：1左2上3右4下
    //首先根据前两个位置来确定不能移动的方向
    //返回值为1表示可以这么移动（如果移动之后撞墙，也认为是可以移动，这部分在gameover函数中判定）
    //返回值为0表示不能这么移动（掉头）
    int x_diff=Snake.at(1).x-Snake.at(0).x;
    int y_diff=Snake.at(1).y-Snake.at(0).y;
    int drction_deny;
    if(x_diff==1) drction_deny=2;
    else if(x_diff==-1) drction_deny=0;
    else if(y_diff==1) drction_deny=3;
    else if(y_diff==-1) drction_deny=1;
    //执行移动操作
    if(drction==drction_deny){
        return 0;
    }else{
        Point head_point=Point();
        switch(drction){
        case 0://左
            head_point.x=Snake.at(0).x-1;
            head_point.y=Snake.at(0).y;
            break;
        case 1://上
            head_point.x=Snake.at(0).x;
            head_point.y=Snake.at(0).y-1;
            break;
        case 2://右
            head_point.x=Snake.at(0).x+1;
            head_point.y=Snake.at(0).y;
            break;
        case 3://下
            head_point.x=Snake.at(0).x;
            head_point.y=Snake.at(0).y+1;
            break;
        }
        if(head_point==Food){ //撞上食物
            Snake.insert(Snake.begin(), head_point);
            Score += 1;
            generate_food();  //重新生成一个食物
        }else{
            Snake.insert(Snake.begin(), head_point);
            Snake.pop_back();
        }
        return 1;
    }
}

int gameover(){
    //判断游戏是否结束 1为成功 0为未结束 -1为失败
    if(Snake.at(0).x<=-1||Snake.at(0).x>=MAP_X||Snake.at(0).y<=-1||Snake.at(0).y>=MAP_Y){
        return -1;
    }
    for(int t=1;t<=Snake.size()-1;t++){
        if(Snake.at(t)==Snake.at(0)){
            return -1;
        }
    }
    if(Snake.size()==MAP_X*MAP_Y-1){
        return 1;
    }
    return 0;
}

void generate_food(){
    //生成食物
    set<int> pset=set<int>();  //先找出不能生成食物的地方
    for(int t=0;t<=Snake.size()-1;t++){
        pset.insert(Snake.at(t).y*MAP_X+Snake.at(t).x);
    }
    vector<Point> plist=vector<Point>();  //再找出可以生成食物的地方
    for(int y=0;y<=MAP_Y-1;y++){
        for(int x=0;x<=MAP_X-1;x++){
            Point p(x,y);
            if(pset.find(p.y*MAP_X+p.x)==pset.end()){
                plist.push_back(p);
            }
        }
    }
    int foodindex=rand()%plist.size(); //生成食物
    Food=plist[foodindex];
}

void game_init(){
    srand(time(NULL));
    for(int x=4;x>=0;x--){
        Point p(x,0);
        Snake.push_back(p);
    }
//    Snake.push_back(Point(1,1));
//    Snake.push_back(Point(0,1));
//    Snake.push_back(Point(0,2));
//    for(int x=0;x<=2;x++) Snake.push_back(Point(x,3));
//    for(int y=3;y>=1;y--) Snake.push_back(Point(3,y));
//    for(int x=3;x>=1;x--) Snake.push_back(Point(x,0));
    generate_food();
//    PrintMap();
//    system("pause");
}

void game_play(){
    game_init();
    _sleep(25);
    while(1){
        int drction=ai.play();
        game_move(drction);
        PrintMap();
        printf("Score: %d\n", Score);
        int game_status=gameover();
        if(game_status==1){
            cout<<"获胜!\n";
            break;
        }
        if(game_status==-1){
            cout<<"失败!\n";
            break;
        }
//        if(!ai.PlanPath.empty()){
//            for(int t=0;t<=ai.PlanPath.size()-1;t++){
//                cout<<ai.PlanPath.at(t).x<<"\t"<<ai.PlanPath.at(t).y<<endl;
//            }
//        }else{
//            cout<<"empty!\n";
//        }
        _sleep(25);
    }
}
