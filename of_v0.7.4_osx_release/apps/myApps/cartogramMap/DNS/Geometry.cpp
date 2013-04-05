//
//  Geometry.cpp
//  emptyExample
//
//  Created by David Stolarsky on 4/5/13.
//
//

#include "Geometry.h"

ofVec2f DNS::Geometry::MapBounds(ofVec2f fromPoint, Bounds<float> fromBounds, Bounds<float> toBounds)
{
    ofVec2f toPoint;
    toPoint.x = ofMap(fromPoint.x, fromBounds.left, fromBounds.right, toBounds.left, toBounds.right);
    toPoint.y = ofMap(fromPoint.y, fromBounds.bottom, fromBounds.top, toBounds.bottom, toBounds.top);
    
    return toPoint;
}