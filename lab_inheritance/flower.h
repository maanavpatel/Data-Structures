/**
 * @file flower.h
 * Declaration of the Flower class.
 */

#pragma once

#include "shape.h"
#include "rectangle.h"
#include "triangle.h"
#include "circle.h"

/**
 * A subclass of Drawable that can draw a flower
 */
class Flower : public Drawable
{
  private:
    Vector2 stem_center;
    Rectangle* stem;
    Circle* pistil; // center piece of flower
    Vector2 leaf_center;
    Triangle* leaf;
    void drawPetals(cs225::PNG* canvas, const Vector2& center, int x, int y) const;

  public:
    Flower(const Vector2& center);
    void draw(cs225::PNG* canvas) const;
};
