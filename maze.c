#include "graphics.h"

#define ROWS 15
#define COLS 15
#define CELL_SIZE 30

int maze[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
    {1,1,1,0,1,1,1,0,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,1,0,0,1},
    {1,0,1,1,1,0,1,1,1,1,0,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,1,0,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,1,0,1},
    {1,1,1,0,1,0,1,1,1,1,1,0,1,0,1},
    {1,0,0,0,1,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,1,1,1,1,1,1,0,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,1,0,1,0,1,0,1},
    {1,1,1,1,1,1,0,1,1,0,1,1,1,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,0,2},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}; //maze representation, 0 for path, 1 for wall, 2 for exit, 3 for visited cell

typedef struct {
    int x;
    int y;
    int dir;
} Robot;

int stopRunning = 0; 

void drawMaze() {
    background();
    
    setColour(lightgray); //grid lines for maze
    for (int x = 0; x <= COLS; x++) {
        int px = x * CELL_SIZE;
        drawLine(px, 0, px, ROWS * CELL_SIZE);
    }
    for (int y = 0; y <= ROWS; y++) {
        int py = y * CELL_SIZE;
        drawLine(0, py, COLS * CELL_SIZE, py);
    }

    //walls and exit
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (maze[y][x] == 1) {
                setColour(black);
                fillRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            } else if (maze[y][x] == 2) {
                setColour(gray);
                fillRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
            }
        }
    }
}

void drawRobot(Robot *r){
    foreground();
    clear();
    setColour(green);

    int cx = r->x * CELL_SIZE + CELL_SIZE/2; //the center coordinate of the robot
    int cy = r->y * CELL_SIZE + CELL_SIZE/2;

    int xs[3], ys[3];
    //robot is a isosceles triangle fits in a cell 
    //calculate triangle vertices based on the direction  
    if (r->dir == 0) { //up
        xs[0] = cx;         ys[0] = cy - CELL_SIZE/2;  
        xs[1] = cx - CELL_SIZE/2;  ys[1] = cy + CELL_SIZE/2;
        xs[2] = cx + CELL_SIZE/2;  ys[2] = cy + CELL_SIZE/2;
    } else if (r->dir == 1) {  //right
        xs[0] = cx + CELL_SIZE/2;  ys[0] = cy;
        xs[1] = cx - CELL_SIZE/2;  ys[1] = cy - CELL_SIZE/2;
        xs[2] = cx - CELL_SIZE/2;  ys[2] = cy + CELL_SIZE/2;
    } else if (r->dir == 2) {  //down
        xs[0] = cx;         ys[0] = cy + CELL_SIZE/2;
        xs[1] = cx - CELL_SIZE/2;  ys[1] = cy - CELL_SIZE/2;
        xs[2] = cx + CELL_SIZE/2;  ys[2] = cy - CELL_SIZE/2;
    } else if (r->dir == 3) {  //left
        xs[0] = cx - CELL_SIZE/2;  ys[0] = cy;
        xs[1] = cx + CELL_SIZE/2;  ys[1] = cy - CELL_SIZE/2;
        xs[2] = cx + CELL_SIZE/2;  ys[2] = cy + CELL_SIZE/2;
    }

    fillPolygon(3, xs, ys); 
    sleep(10); 
}

void forward (Robot *r){ //calculate the current grid coordinate after moving ont cell forward in current direction
    if (r->dir == 0) r->y--;
    else if (r->dir == 1) r->x++;
    else if (r->dir == 2) r->y++;
    else if (r->dir == 3) r->x--;
}

void right (Robot *r){
    r->dir = (r->dir + 1) % 4;
}

int canMoveForward (Robot *r){
    int nx = r->x;
    int ny = r->y;

    if (r->dir == 0) ny--;
    else if (r->dir == 1) nx++;
    else if (r->dir == 2) ny++;
    else if (r->dir == 3) nx--;

    if (nx < 0 || nx >= COLS || ny < 0 || ny >= ROWS) return 0; //check boundaries
    if (maze[ny][nx] == 1 || maze[ny][nx] == 3) return 0; //check for wall or visited cells
    return 1;
}

void solveMaze (Robot *r){
    if (stopRunning == 1) return; 

    drawRobot(r);

    if (maze[r->y][r->x] == 2){ //check the exit
        stopRunning = 1;
        return;
    }
    
    maze[r->y][r->x] = 3; 

    for (int i = 0; i < 4; i++){ 
        if (canMoveForward(r)){
            Robot next = *r; //create a clone of the robot to explore the path
            forward(&next);
            drawRobot(&next);
            sleep(100);
            solveMaze(&next); //recursively explore from the new position
            if (stopRunning == 1) return; 
        }
        //if blocked, turn right
        right(r);
        drawRobot(r);
        sleep(100);
    }
}

int main(){
    setWindowSize(COLS * CELL_SIZE, ROWS * CELL_SIZE); 

    drawMaze();

    Robot myRobot = {0, 1, 1}; //initialize the robot at position(0,1) and facing right
    solveMaze(&myRobot);

    while(1){ //keep window open after completion
        sleep(300);
    }
    return 0;
}