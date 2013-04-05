//
//  CBDebug.h
//  emptyExample
//
//  Created by David Stolarsky on 3/12/13.
//
//

#ifndef emptyExample_CBDebug_h
#define emptyExample_CBDebug_h

#include <string>
#include <iostream>

#include <ofMain.h>

#include "DNS/Geometry.h"

void CBLog(std::string msg)
{
    std::cerr << msg << std::endl;
}

void CBLog(ofVec2f vec)
{
    stringstream ss;
    ss << "ofVec2f(" << vec.x << "," << vec.y << ")";
    CBLog(ss.str());
}

template <class T>
void CBLog(Bounds<T> bounds)
{
    stringstream ss;
    ss << "Bounds(" << bounds.left << "," << bounds.right << ", " << bounds.top << ", " << bounds.bottom << ")";
    CBLog(ss.str());
}

#endif
