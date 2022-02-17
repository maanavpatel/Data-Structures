#pragma once

#include "cs225/PNG.h"

#include <string>

void drawRectangle(cs225::PNG& image, size_t x1, size_t y1, size_t x2, size_t y2, cs225::HSLAPixel& pixel);
void rotate(std::string inputFile, std::string outputFile);
cs225::PNG myArt(unsigned int width, unsigned int height);
