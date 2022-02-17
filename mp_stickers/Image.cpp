#include <iostream>
#include <cmath>
#include <cstdlib>
#include "Image.h"

using cs225::HSLAPixel;

void Image::lighten(){
    lighten(0.1);
}

void Image::lighten(double amount){
    for (unsigned x = 0; x < width(); x++) {
        for (unsigned y = 0; y < height(); y++) {
            HSLAPixel & pixel = getPixel(x, y);
            if ((pixel.l + amount) > 1) {
                pixel.l = 1;
            } else {
                pixel.l = pixel.l + amount;
            }
        }
    }
    return;
}

void Image::darken(){
    darken(0.1);
}

void Image::darken(double amount){
    for (unsigned x = 0; x < width(); x++) {
        for (unsigned y = 0; y < height(); y++) {
            HSLAPixel & pixel = getPixel(x, y);
            if ((pixel.l - amount) < 0) {
                pixel.l = 0;
            } else {
                pixel.l = pixel.l - amount;
            }
        }
    }
    return;
}

void Image::saturate(){
    saturate(0.1);
}

void Image::saturate(double amount){
    for (unsigned x = 0; x < width(); x++) {
        for (unsigned y = 0; y < height(); y++) {
            HSLAPixel & pixel = getPixel(x, y);
            if ((pixel.s + amount) > 1) {
                pixel.s = 1;
            } else {
                pixel.s = pixel.s + amount;
            }
        }
    }
    return;
}

void Image::desaturate(){
    desaturate(0.1);
}

void Image::desaturate(double amount){
    for (unsigned x = 0; x < width(); x++) {
        for (unsigned y = 0; y < height(); y++) {
            HSLAPixel & pixel = getPixel(x, y);
            if ((pixel.s - amount) < 0) {
                pixel.s = 0;
            } else {
                pixel.s = pixel.s - amount;
            }
        }
    }
    return;
}

void Image::grayscale(){
    for (unsigned x = 0; x < width(); x++) {
        for (unsigned y = 0; y < height(); y++) {
            HSLAPixel & pixel = getPixel(x, y);
            pixel.s = 0;
        }
    }
    return;
}

void Image::rotateColor(double degrees){
    for (unsigned x = 0; x < width(); x++) {
        for (unsigned y = 0; y < height(); y++) {
            HSLAPixel & pixel = getPixel(x, y);
            pixel.h = pixel.h + degrees;
            while (pixel.h >= 360) {
                pixel.h -= 360;
            }
            if (pixel.h < 0) {
                pixel.h += 360;
            }
        }
    }
    return;
}

void Image::illinify(){
    for (unsigned x = 0; x < width(); x++) {
        for (unsigned y = 0; y < height(); y++) {
            HSLAPixel & pixel = getPixel(x, y);
            double orangeDist = (int) abs(11 - pixel.h) % 360;
            double blueDist =  (int) abs(216 - pixel.h) % 360;
            if (blueDist > 180) {
                blueDist = 360 - blueDist;
            }
            if (orangeDist > 180) {
                orangeDist = 360 - orangeDist;
            }
            if (orangeDist < blueDist) {
                pixel.h = 11;
            } else {
                pixel.h = 216;
            }
        }
    }
    return;
}

void Image::scale(double factor){
    // std::cout << "Scale by " << factor << std::endl;
    scale(width() * factor, height() *  factor);
}

void Image::scale(unsigned w, unsigned h){
    // std::cout << "New dim: " << w << "x" << h << std::endl;
    double x_ratio = (double)w / width();
    double y_ratio = (double)h / height();
    // std::cout << "ratio: (" << x_ratio << "," << y_ratio << ")" << std::endl;
    unsigned scaledX = width() * x_ratio;
    unsigned scaledY = height() * y_ratio;
    PNG* scaled = new PNG(scaledX, scaledY);
    for (unsigned x = 0; x < w; x++) {
        for (unsigned y = 0; y < h; y++) {
            scaledX = x / x_ratio;
            scaledY = y / y_ratio;
            HSLAPixel & ogPixel = getPixel(scaledX, scaledY);
            scaled->getPixel(x, y) = ogPixel;
        }
    }
    resize(w, h);
    for (unsigned x = 0; x < width(); x++) {
        for (unsigned y = 0; y < height(); y++) {
            getPixel(x,y) = scaled->getPixel(x, y);
        }
    }
    delete scaled;
}
