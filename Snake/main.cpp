#include <iostream>
#include "total.h"
#include <time.h>

void test(){
    system("python3 E:\\Documents\\QtProjects\\Sokoban\\ai.cpp");
    Snake.push_back(Point(3,0));
    Snake.push_back(Point(4,0));
    for(int y=1;y<=4;y++) Snake.push_back(Point(4,y));
    for(int x=3;x>=0;x--) Snake.push_back(Point(x,4));
    for(int y=5;y<=7;y++) Snake.push_back(Point(0,y));

    Point x=GetDirectPoint();
    cout<<x.x<<" "<<x.y<<endl;
}

int main(int argc, char *argv[])
{
    system("pause");
    game_play();
//    test();
    return 0;
}
