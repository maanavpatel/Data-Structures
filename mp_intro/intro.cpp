#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <ctime>
#include <cmath>
#include <string>

using cs225::PNG;
void rotate(std::string inputFile, std::string outputFile) {
  PNG input;
  input.readFromFile(inputFile);
  size_t w  = input.width();
  size_t h = input.height();

  PNG* output = new PNG(w, h); 

  for (size_t x = 0; x < w; x++) {
    for(size_t y = 0; y < h; y++) {
      output->getPixel(x, y) = input.getPixel(w - 1 - x, h - 1 - y);
    }
  }

  output->writeToFile(outputFile);
}

void drawRectangle(PNG& image, size_t x1, size_t y1, size_t x2, size_t y2, cs225::HSLAPixel& pixel) {
  if (x2 < x1) {
    size_t tempX = x2;
    x2 = x1;
    x1 = tempX;
  } 
  if (y2 < y1) {
    size_t tempY = y2;
    y2 = y1;
    y1 = tempY;
  }
  for (size_t x = x1; x < x2; x++) {
    for (size_t y = y1; y < y2; y++) {
      if (x < image.width() && x > 0 && y < image.height() && y > 0) {
        image.getPixel(x, y) = pixel;
      }
    }
  }
}


cs225::PNG myArt(unsigned int width, unsigned int height) {
  cs225::PNG png(width, height);
  size_t center = width/2;
  cs225::HSLAPixel black(0,0,0,1);
  for (size_t x = 0; x < width; x++) {
    for(size_t y = 0; y < height; y++) {
      png.getPixel(x, y) = black;
    }
  }

  size_t thickness = 1;
  size_t length = 10;
  cs225::HSLAPixel color(10, 100, 100);
  int x = center;
  int y = center;
  srand((unsigned) time(0));

  for (size_t i = 0; i < (width * 2) * 8; i++) {
    //generate number between 1 and 8
    // std::srand(std::time(nullptr));
    int random = std::rand() % 8 + 1;
    int newX;
    int newY;
    //handle all cases
    switch (random) {
      case 1:
        newX = x + thickness;
        newY = y + length;
        break;
      case 2:
        newX = x + thickness;
        newY = y - length;
        break;
      case 3:
        newX = x + length;
        newY = y + thickness;
        break;
      case 4:
        newX = x - length;
        newY = y + thickness;
        break;
      case 5:
        newX = x - thickness;
        newY = y - length;
        break;
      case 6:
        newX = x - thickness;
        newY = y + length;
        break;
      case 7:
        newX = x - length;
        newY = y - thickness;
        break;
      case 8:
        newX = x + length;
        newY = y - thickness;
        break;
    }
    
    //makes sure the drawn if will be in bounds.
    if (newX > 0 && newX < width && newY > 0  && newY < height){
      drawRectangle(png, x, y, newX, newY, color);
      x = newX;
      y = newY;
    } else {
      //otherwise try to reposition somwhere inside the frame
      x = (rand() % (width/4)+ 1)  + center;
      y = (rand() % (height/4) + 1) + center;
      i--;
    }

    color.h += 0.8;
    if (color.h > 360) {
      color.h = 0;
    }
    
  }

  return png;
}
