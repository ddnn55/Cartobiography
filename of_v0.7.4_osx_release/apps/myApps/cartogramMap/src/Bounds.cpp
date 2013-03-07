//
//  Bounds.cpp
//  emptyExample
//
//  Created by David Stolarsky on 3/6/13.
//
//

#include "Bounds.h"

Bounds::Bounds()
{
    left = right = top = bottom = 0.0;
}

Bounds::Bounds(float left, float right, float top, float bottom)
{
    this->left = left;
    this->right = right;
    this->top = top;
    this->bottom = bottom;
}