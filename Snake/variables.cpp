#include "total.h"

vector<Point> Snake=vector<Point>();
Point Food(0,0);
int Score=0;
AI ai=AI();

ImaginaryMap::ImaginaryMap(vector<Point> input_snake){
    int x,y;
    isnake=input_snake;
    for(y=0;y<=MAP_Y-1;y++){
        for(x=0;x<=MAP_X-1;x++){
            ditu[x][y]=0;
        }
    }
    for(int t=1;t<=isnake.size()-1;t++){
        ditu[isnake.at(t).x][isnake.at(t).y]=2;
    }
    if(isnake.at(0).x>=0&&isnake.at(0).x<=MAP_X-1&&isnake.at(0).y>=0&&isnake.at(0).y<=MAP_Y-1) ditu[isnake.at(0).x][isnake.at(0).y]=1;
    ditu[Food.x][Food.y]=3;
}
