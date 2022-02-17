#include <cmath>
#include <iterator>
#include <iostream>

#include "../cs225/HSLAPixel.h"
#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() {
  /** @todo [Part 1] */
    t_ = NULL;
    finished = false;
}

ImageTraversal::Iterator::Iterator(PNG img, Point startPoint, double tolerance, ImageTraversal* traversal) {
   /** @todo [Part 1] */
   img_ = img;
   position_ = startPoint;
   beginning_ = position_;
   tol_ = tolerance;
   t_ = traversal;
  
  //all points unvisited
  for (size_t i = 0; i < img_.width() * img_.height(); i++) {
    posVisited_.push_back(false);
  }
  finished = false;

  if (isVisitable(position_)) {
    posVisited_[position_.x + position_.y * img_.width()] = true;

    path_.push_back(position_);
  } else {
    finished = true;
  }
}

/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  Point right = Point(position_.x + 1, position_.y);
  Point down = Point(position_.x, position_.y + 1);
  Point left = Point(position_.x-1, position_.y);
  Point up = Point(position_.x, position_.y - 1);

  if (isVisitable(right)) t_->add(right);
  if (isVisitable(down)) t_->add(down);
  if (isVisitable(left)) t_->add(left);
  if (isVisitable(up)) t_->add(up);

  if (t_->empty()) {
    setEnd(true);
    return *this;
  }

  Point p = t_->pop();
  while (posVisited_[p.x + p.y * img_.width()]) {
    if (t_->empty()) {
      setEnd(true);
      return *this;
    }
    p = t_->pop();
  }

  position_ = p;
  posVisited_[position_.x + position_.y * img_.width()] = true;
  path_.push_back(position_);
  return *this;
}

bool ImageTraversal::Iterator::isVisitable(Point point) {
  if (point.x < img_.width() && point.y < img_.height()) {
    HSLAPixel start = (img_.getPixel(beginning_.x, beginning_.y));
    HSLAPixel desired = (img_.getPixel(point.x, point.y));
    if (calculateDelta(start, desired) < tol_) {
      return true;
    }
  }
  return false;
} 


void ImageTraversal::Iterator::setEnd(bool end){
  finished = end;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  return position_;
;
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  return (finished != other.finished); 
}