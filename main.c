#include "raylib.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include <stdio.h>

//CELL
#define CELL_SIZE 20
#define screenWidth 40*CELL_SIZE
#define screenHeight 30*CELL_SIZE
#define MAX_IN_CELL 32

#define GRID_WIDTH (screenWidth/CELL_SIZE)
#define GRID_HEIGHT (screenHeight/CELL_SIZE)        
#define MAX_PARTICLES 1000

typedef struct 
{
    /* data */
    int count;
    int id[MAX_IN_CELL];
}Cell;

Cell grid[GRID_HEIGHT][GRID_WIDTH];

void ClearGrid()
{
    for(int y=0;y<GRID_HEIGHT;y++)
    {
        for(int x=0;x<GRID_WIDTH;x++)
        {
            grid[y][x].count=0;
        }
    }
}

void BuildGrid(Vector2 position[], int n)
{
    for (int i=0; i<n; i++)
    {
        int gx=(int)(position[i].x/CELL_SIZE);
        int gy=(int)(position[i].y/CELL_SIZE);

        //safety check
        if(gx<0||gy<0||gx>=GRID_WIDTH||gy>=GRID_HEIGHT)
            continue;
        Cell *cell=&grid[gy][gx];

        if(cell->count<MAX_IN_CELL)
        {
            cell->id[cell->count]=i;
            cell->count++;
        }
    }
}

void PositionToGrid(Vector2 pos, int *gx, int *gy)
{
    *gx=(int)(pos.x/CELL_SIZE);
    *gy=(int)(pos.y/CELL_SIZE);
}

#define n 625
float e=0.1f;
float gravity =0.10f;
int radius=5;
int influenceRadius=25;
int mass=1;
// float Integralvolume=log((influenceRadius+10)/10)-(influenceRadius/10);


float magnitude2(Vector2 vec)
{
    return vec.x*vec.x + vec.y*vec.y;
}

float smoothingfunction(float r)
{
    if (r >= 0 && r <= influenceRadius)
    {
        /* code */
        return (powf((1-r/influenceRadius),2));
    }
    else
    {
        return 0.0f;
    }
   
}

float smoothingfunctionnear(float r)
{
    if (r >= 0 && r <= influenceRadius)
    {
        /* code */
        return (powf((1-r/influenceRadius),3));
    }
    else
    {
        return 0.0f;
    }
   
}

float density(int i, Vector2 position[])
{
    float density=0.0f;
    int gx,gy;
    PositionToGrid(position[i], &gx, &gy);

    for(int oy=-1;oy<=1;oy++)
{
    for(int ox=-1;ox<=1;ox++)
    {
        int ngx=gx+ox;
        int ngy=gy+oy;
        if(ngx<0||ngy<0||ngx>=GRID_WIDTH||ngy>=GRID_HEIGHT)
            continue;
        Cell *cell=&grid[ngy][ngx];
        for(int idx=0;idx<cell->count;idx++)
        {
            int pid=cell->id[idx];
            Vector2 diff={
                position[i].x - position[pid].x,
                position[i].y - position[pid].y
            };
            float r=sqrtf(magnitude2(diff));
            density+=mass*smoothingfunction(r);
        }
}
}}

float neardensity(int i, Vector2 position[])
{
    float density=0.0f;
    int gx,gy;
    PositionToGrid(position[i], &gx, &gy);

    for(int oy=-1;oy<=1;oy++)
{
    for(int ox=-1;ox<=1;ox++)
    {
        int ngx=gx+ox;
        int ngy=gy+oy;
        if(ngx<0||ngy<0||ngx>=GRID_WIDTH||ngy>=GRID_HEIGHT)
            continue;
        Cell *cell=&grid[ngy][ngx];
        for(int idx=0;idx<cell->count;idx++)
        {
            int pid=cell->id[idx];
            Vector2 diff={
                position[i].x - position[pid].x,
                position[i].y - position[pid].y
            };
            float r=sqrtf(magnitude2(diff));
            density+=mass*smoothingfunctionnear(r);
        }
}
}}


float pseudoPressure(float dens)
{
    float restDensity=0.10f;
    return ((6.0)*(dens - restDensity));
}

float pseudoPressurenear(float dens)
{
   
    return ((1.00)*(dens ));
}

float differentiateSmoothingfunction(float r)
{
    if (r >= 0 && r <= influenceRadius)
    {
        /* code */
        return (1 - r/influenceRadius);
    }
    else
    {
        return 0.0f;
    }
   
}

float differentiateSmoothingfunctionnear(float r)
{
    if (r >= 0 && r <= influenceRadius)
    {
        /* code */
        return pow((1 - r/influenceRadius), 2);
    }
    else
    {
        return 0.0f;
    }
   
}

Vector2 acceleration(int i, Vector2 position[])
{
    float accx=0.0f;
    float accy=0.0f;
    int gx,gy;
    PositionToGrid(position[i], &gx, &gy);

    for(int oy=-1;oy<=1;oy++)
{
    for(int ox=-1;ox<=1;ox++)
    {
        int ngx=gx+ox;
        int ngy=gy+oy;
        if(ngx<0||ngy<0||ngx>=GRID_WIDTH||ngy>=GRID_HEIGHT)
            continue;
        Cell *cell=&grid[ngy][ngx];
        for(int idx=0;idx<cell->count;idx++)
        {
            int pid=cell->id[idx];
            if (pid==i)
            {
                continue;
            }
            Vector2 d={
                position[i].x - position[pid].x,
                position[i].y - position[pid].y
            };
            Vector2 dir;
            float r=sqrtf(magnitude2(d));
            float pres=pseudoPressure(density(i,position));

            float presnear=pseudoPressurenear(neardensity(i,position));
            dir.x=d.x / r;
            dir.y=d.y / r;
            accx+=(-mass)*(pres*differentiateSmoothingfunction(r)+presnear*differentiateSmoothingfunctionnear(r))*dir.x;
            accy+=(-mass)*(pres*differentiateSmoothingfunction(r)+presnear*differentiateSmoothingfunctionnear(r))*dir.y;
        }
}
}  
    Vector2 acc={accx,accy};
    return acc;
}
    
    


int main(void)
{
    srand(time(NULL));
    InitWindow(screenWidth, screenHeight, "SPH Simulation");
    int i=0,j=0;

    Vector2 position[n];
    Vector2 velocity[n];
    Vector2 predictedposition[n];


    // put particles in grid
    int rows= sqrt(n);
    int cols=(n+rows-1)/rows;
    float spacingx=(float)(screenWidth/((cols+1)*1.3));   
    float spacingy=(float)(screenHeight/((rows+1)*1.3));

    for ( i = 0; i < n; i++)
    {
        /* code */
        int row =i%rows;
        int col =i/rows;
        position[i].y= spacingy*(row+1);
        position[i].x= spacingx*(col+1);
        velocity[i].x=5.0f;
        velocity[i].y=5.0f;
        
    }

    SetTargetFPS(120);
    ClearGrid();

    while(!WindowShouldClose())
    {
        ClearGrid();
        BuildGrid(position,n);

        for(i=0;i<n;i++)
        {

            predictedposition[i].x=position[i].x+velocity[i].x;
            predictedposition[i].y=position[i].y+velocity[i].y;
            
            velocity[i].x+=acceleration(i,predictedposition).x;
            velocity[i].y+=acceleration(i,predictedposition).y+gravity;
            position[i].y+=velocity[i].y;
            position[i].x+=velocity[i].x;
        

            if(position[i].y+radius >= screenHeight){
                position[i].y=screenHeight-radius;
            
                velocity[i].y=(-e)*velocity[i].y;
            }
            if(position[i].y-radius <= 0){
                position[i].y=radius;
                
                velocity[i].y=(-e)*velocity[i].y;
            }
            
            if(position[i].x+radius >= screenWidth){
                position[i].x=screenWidth-radius;
                
                velocity[i].x=(-e)*velocity[i].x;
            }
            if(position[i].x-radius <=0){
                position[i].x=radius;
            
                velocity[i].x=(-e)*velocity[i].x;
            }
        for ( j = i+1; j < n; j++)
            {
                /* code */
                if (i!=j){
                    float d=sqrtf((position[i].y-position[j].y)*(position[i].y-position[j].y)+(position[i].x-position[j].x)*(position[i].x-position[j].x));
                    float dx=position[i].x-position[j].x;
                    float dy=position[i].y-position[j].y;
                    if(d*d<=4*radius*radius){
                        float tempx=velocity[i].x;
                        float tempy=velocity[i].y;
                        velocity[i].x=velocity[j].x;
                        velocity[j].x=tempx;
                        velocity[i].y=velocity[j].y;
                        velocity[j].y=tempy;
                        float t=radius-d*0.5f;
                        position[i].x+= t*(dx)/d;
                        position[j].x-=t*(dx)/d ;
                        position[i].y+= t*(dy)/d;
                        position[j].y-= t*(dy)/d;
                        
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for(i=0;i<n;i++)
        {
            DrawCircleV(position[i], radius, YELLOW);
        }

        DrawText("SPH Simulation",10,10,20,BLUE);
        EndDrawing();

    }
    CloseWindow();
    return 0;




}

//gcc src/main.c -o build/app.exe -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm
//build/app.exe