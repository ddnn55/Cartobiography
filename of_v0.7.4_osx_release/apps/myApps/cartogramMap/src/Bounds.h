//
//  Bounds.h
//  emptyExample
//
//  Created by David Stolarsky on 3/6/13.
//
//

#ifndef __emptyExample__Bounds__
#define __emptyExample__Bounds__

class Bounds {
public:
    float left;
    float right;
    float top;
    float bottom;
    
    Bounds();
    Bounds(float left, float right, float top, float bottom);
    float width() { return right - left; }
    float height() { return top - bottom; }
};

#endif /* defined(__emptyExample__Bounds__) */
