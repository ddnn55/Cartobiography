//
//  CBUtil.cpp
//  emptyExample
//
//  Created by David Stolarsky on 4/4/13.
//
//

#include "CBUtil.h"

string vec2string(ofVec2f vec)
{
    stringstream ss;
    ss << vec.x << "," << vec.y;
    return ss.str();
}