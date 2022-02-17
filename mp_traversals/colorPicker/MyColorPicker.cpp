#include "../cs225/HSLAPixel.h"
#include "../Point.h"

#include "ColorPicker.h"
#include "MyColorPicker.h"
#include <time.h>  
using namespace cs225;

/**
 * Picks the color for pixel (x, y).
 * Using your own algorithm
 */
HSLAPixel MyColorPicker::getColor(unsigned x, unsigned y) {
  // srand(time(NULL));
  int a =  rand() % (y % 360);
  if (x > y) {
    a = a/ 2;
  }
  HSLAPixel pixel(a, 1, 0.5);
  return pixel;
}
