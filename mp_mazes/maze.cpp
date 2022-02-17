/* Your code here! */
#include <algorithm>
#include <vector>
#include <queue>
#include <cstdlib>
#include <sys/time.h>
#include <time.h>
#include "maze.h"
#include <math.h>

SquareMaze::SquareMaze():width(0), height(0){}

void SquareMaze::deleter(DisjointSets &d, int col, int row)
{
  if (col < (width - 1) && d.find(row * width + col) != d.find(row * width + col + 1)) {
    maze[col][row].r = false;
    d.setunion(row * width + col, row * width + col + 1);
  }

  if (row < (height - 1) && d.find(row * width + col) != d.find((row + 1) * width + col)) {
    maze[col][row].d = false;
    d.setunion(row * width + col, (row + 1) * width + col);
  }
}

void SquareMaze::makeMaze(int w, int h)
{
  // delete if maze already exists
  if (width != 0) {
    for(int i = 0; i < width; i++) {
      delete maze[i];
    }
    delete[] maze;
  }

  width = w;
  height = h;
  maze = new Cell*[width];
  DisjointSets d;
  int r;
  vector<int> arr;

  for(int i = 0; i < width; i++){
    maze[i] = new Cell[height];
  }

  d.addelements(width*height);

  //delete walls while preventing cycles
  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {
      maze[i][j].x = i;
      maze[i][j].y = j;

      arr.push_back(j * width + i);
    }
  }

  struct timeval time;
  gettimeofday(&time, NULL);
  srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

  random_shuffle(arr.begin(), arr.end());

  int temp = arr[0];

  int currx;
  int curry;

  for(unsigned i = 0; i < arr.size(); i++){
    currx  = arr[i] % width;
    curry = arr[i] / width;
    deleter(d, currx, curry);
  }
}

bool SquareMaze::canTravel(int x, int y, int dir) const
{
  if (dir == 0)
    if (!maze[x][y].r)
      return true;

  if (dir == 1)
    if (!maze[x][y].d)
      return true;

  if (dir == 2 && x-1 >= 0)
    if (!maze[x-1][y].r)
      return true;

  if (dir == 3 && y-1 >= 0)
    if (!maze[x][y-1].d)
      return true;

  return false;
}


void SquareMaze::setWall(int x, int y, int dir, bool exists)
{
  if (dir == 0)
    maze[x][y].r = exists;

  if(dir == 1)
    maze[x][y].d = exists;

  return;
}


vector<int> SquareMaze::solveMaze()
{
  std::queue<Cell> q;
  vector<int> lastR;
  vector<int> solved;
  vector<Cell> parent;
  Cell curr;

  int x = 0;
  int y = 0;
  int dist = 0;

  bool** isVisited = new bool*[width];

  lastR.reserve(width);
  parent.reserve(width*height);

  for (int i = 0; i < width; i++) {
    isVisited[i] = new bool[height];
    for(int j = 0; j < height; j++) {
      isVisited[i][j] = false;
    }
  }

  q.push(maze[x][y]);
  isVisited[x][y] = true;

  while (!q.empty()) {
    curr = q.front();
    q.pop();

    x = curr.x;
    y = curr.y;

    if (y == (height - 1))
      lastR[x] = dist;

    if (canTravel(x, y, 0) && !isVisited[x + 1][y]) {
      q.push(maze[x + 1][y]);
      isVisited[x + 1][y] = true;
      parent[y * width + x + 1] = curr;
    }

    if (canTravel(x, y, 1) && !isVisited[x][y + 1]) {
      q.push(maze[x][y + 1]);
      isVisited[x][y + 1] = true;
      parent[(y + 1) * width + x] = curr;
    }

    if (canTravel(x, y, 2) && !isVisited[x-1][y]) {
      q.push(maze[x-1][y]);
      isVisited[x-1][y] = true;
      parent[y*width + x-1] = curr;
    }

    if (canTravel(x, y, 3) && !isVisited[x][y-1]) {
      q.push(maze[x][y - 1]);
      isVisited[x][y - 1] = true;
      parent[(y - 1) * width + x] = curr;
    }

    dist++;
  }

  int maxDist = 0;
  int destX;
  for(int i = 0; i < width; i++) {
    if(lastR[i] > maxDist) {
      maxDist = lastR[i];
      destX = i;
    }
  }

  dest = maze[destX][height-1];
  curr = dest;
  Cell p;
  int dir;

  // backtrack from dest
  while(!(curr.x == 0 && curr.y == 0)) {
    p = parent[(curr.y) * width + curr.x];

    if (p.x == curr.x) {
      if (p.y + 1 == curr.y)
        dir = 1;

      if (p.y - 1 == curr.y)
        dir = 3;
    }

    if (p.y == curr.y) {
      if (p.x + 1 == curr.x)
        dir = 0;

      if (p.x - 1 == curr.x)
        dir = 2;
    }

    solved.insert(solved.begin(), dir);
    curr = p;
  }

  for(int i = 0; i < width; i++)
    delete isVisited[i];
  delete[] isVisited;

  return solved;

}

/**
 * Draws maze using specified formulas in doxygen
 */
PNG* SquareMaze::drawMaze() const
{
  PNG* pic = new PNG(width * 10 + 1, height * 10 + 1);

  for (unsigned i = 10; i < pic->width(); i++){
    HSLAPixel & p = pic->getPixel(i, 0);
    p.l = 0;
  }

  for (unsigned i = 0; i < pic->height(); i++){
    HSLAPixel & p = pic->getPixel(0, i);
    p.l = 0;
  }

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {

      if (maze[i][j].r){
        for(int k = 0; k <= 10; k++){
          HSLAPixel & p = pic->getPixel((i + 1) * 10, j * 10 + k);
          p.l = 0;
        }
      }

      if (maze[i][j].d){
        for (int k = 0; k <= 10; k++){
          HSLAPixel & p = pic->getPixel(i * 10 + k, (j + 1) * 10);
          p.l = 0;
        }
      }

    }
  }

  return pic;
}

/**
 * Draws maze with solution using specified formula in doxygen
 */
PNG* SquareMaze::drawMazeWithSolution()
{
  PNG *pic = drawMaze();
  vector<int> path = solveMaze();

  HSLAPixel red (0,1,0.5,1);

  int currx = 5;
  int curry = 5;
  int counter;

  for (unsigned i = 0; i < path.size(); i++) {

    counter = 0;

    if (path[i] == 0) {
      while (counter < 10) {
        pic->getPixel(currx + counter, curry) = red;
        counter++;
      }

      currx += counter;
    }

    else if (path[i] == 1) {
      while (counter < 10) {
        pic->getPixel(currx, curry + counter) = red;
        counter++;
      }

      curry += counter;
    }

    else if (path[i] == 2) {
      while (counter < 10) {
        pic->getPixel(currx - counter, curry) = red;
        counter++;
      }

      currx -= counter;
    }

    else if (path[i] == 3) {
      while (counter < 10) {
        pic->getPixel(currx, curry - counter) = red;
        counter++;
      }

      curry -= counter;
    }
  }

  pic->getPixel(currx, curry) = red;

  dest.d = false;
  maze[dest.x][dest.y].d = false;

  for (int k = 1; k <= 9; k++) {
    pic->getPixel(dest.x * 10 + k, (dest.y + 1) * 10).l = 1.0;
  }

  return pic;
}



/**
 * Draw creative maze
 */
PNG* SquareMaze::drawCreativeMaze() const
{
  PNG* pic = new PNG(width * 10 + 1, height * 10 + 1);

  HSLAPixel cyan (189,1.0, 0.31, 1.0);

  for (unsigned i = 10; i < pic->width(); i++) {
    pic->getPixel(i, 0) = cyan;
  }

  for (unsigned i = 0; i < pic->height(); i++) {
    pic->getPixel(0, i) = cyan;
  }

  for(int i = 0; i < width; i++) {
    for(int j = 0; j < height; j++) {

      HSLAPixel wall ( dist(i, j, width, height), 1, 0.74, 1);

      if (maze[i][j].r) {
        for (int k = 0; k<=10; k++) {
           pic->getPixel((i + 1) * 10, j * 10 + k) = wall;
        }
      }

      if (maze[i][j].d) {
        for (int k = 0; k <= 10; k++) {
          pic->getPixel(i * 10 + k, (j + 1) * 10) = wall;
        }
      }

    }
  }

  for (unsigned i = 0; i < pic->width(); i++){
    pic->getPixel(i, pic->height() - 1) = cyan;
  }

  for (unsigned i = 0; i < pic->height(); i++){
    pic->getPixel(pic->width() - 1, i) = cyan;
  }

  return pic;
}

PNG* SquareMaze::drawCreativeMazeWithSolution()
{
  PNG *ret = drawCreativeMaze();
  vector<int> path = solveMaze();

  int curr_x = 5;
  int curr_y = 5;

  int counter;

  for (unsigned i = 0; i < path.size(); i++) {
    HSLAPixel red ( rand() % 360 , 1.0 , 0.27 , 1);
    counter = 0;

    if (path[i] == 0) {
      while (counter < 10) {
        ret->getPixel(curr_x + counter, curr_y) = red;
        counter++;
      }

      curr_x += counter;
    }

    else if (path[i] == 1) {
      while (counter < 10) {
        ret->getPixel(curr_x, curr_y + counter) = red;
        counter++;
      }

      curr_y += counter;
    }

    else if (path[i] == 2) {
      while (counter < 10) {
        ret->getPixel(curr_x - counter, curr_y) = red;
        counter++;
      }

      curr_x -= counter;
    }

    else if (path[i] == 3) {
      while (counter < 10) {
        ret->getPixel(curr_x, curr_y - counter) = red;
        counter++;
      }

      curr_y -= counter;
    }
  }

  dest.d = false;
  maze[dest.x][dest.y].d = false;

  for(int k = 1; k <= 9; k++){
    ret->getPixel(dest.x * 10 + k, (dest.y + 1) * 10).l = 1.0;
  }

  return ret;
}


int SquareMaze::dist( int i, int j, int w, int h) const{
  double sq = pow((w / 2 - i),2) + pow((h / 2 - j), 2);
  sq = sqrt(sq);
  double max = sqrt(pow(w / 2, 2) + pow(h / 2, 2));
  return (int) (360 * (sq / max));
}