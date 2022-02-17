/**
 * @file HSLAPixel.h
 *
 * @author CS 225: Data Structures
 * @version 2018r1-lab1
 */

#pragma once

#include <iostream>
#include <sstream>

namespace cs225 {
class HSLAPixel {
public:
    HSLAPixel ();
    HSLAPixel (double hue, double saturation, double luminance);
    HSLAPixel (double hue, double saturation, double luminance, double alpha);
    //hue [0, 360)
    double h;
    //saturation [0,1]
    double s;
    //luminance [0,1]
    double l;
    //alpha [0,1]
    double a;
};
}
