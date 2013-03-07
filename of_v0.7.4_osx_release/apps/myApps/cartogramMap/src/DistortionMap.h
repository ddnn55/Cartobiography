//
//  DistortionMap.h
//  emptyExample
//
//  Created by David Stolarsky on 3/5/13.
//
//

#ifndef __emptyExample__DistortionMap__
#define __emptyExample__DistortionMap__

#include <iostream>
#include <vector>

class DistortionMap {
public:
    void load(std::string filename);
    ofTexture map;
};

#endif /* defined(__emptyExample__DistortionMap__) */
