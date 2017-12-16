#ifndef _TOTAL_H_
#define _TOTAL_H_

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <set>
#include <time.h>

#define MAP_X 12
#define MAP_Y 12

using namespace std;

class Point
{
public:
    int x;
    int y;
    bool operator ==(Point p2){
        if(x==p2.x&&y==p2.y){
            return true;
        }else{
            return false;
        }
    }
    int operator -(Point p2){ //定义两个点之间的曼哈顿距离
        return abs(p2.x-x)+abs(p2.y-y);
    }

    Point(int input_x,int input_y){
        x=input_x;
        y=input_y;
    }
    Point(){

    }
};

//class PathPoint: public Point  //和上面的Point的区别是这里包含了从起点到达这个点的路径
//{
//public:
//    vector<Point> Path;
//    PathPoint(int input_x,int input_y):Point(input_x, input_y){
//        Path=vector<Point>();
//    }
//    PathPoint(){
//        Path=vector<Point>();
//    }
//};

class ImaginaryMap  //假想的地图
{
public:
    int ditu[MAP_X][MAP_Y];
    vector<Point> isnake;
    ImaginaryMap(vector<Point> input_snake);
};

class AI
{
public:
    vector<Point> PlanPath;
    Point direct_point;
    int wander_step;
    int play();
    AI(){
        wander_step=0;
        PlanPath=vector<Point>();
        direct_point=Point(-1,-1);
    }
};

void MapShow(int print_map[MAP_X][MAP_Y]);
void PrintMap();
int game_move(int drction);
int gameover();
void generate_food();
void game_init();
void game_play();

vector<Point> FindPath(Point start, Point end, int use_imap, vector<Point> body,int food_valid);
Point GetDirectPoint();
int AiPlay();

extern vector<Point> Snake;
extern Point Food;
extern int Score;
extern AI ai;

#endif
