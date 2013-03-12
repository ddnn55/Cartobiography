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

void CBLog(std::string msg)
{
    std::cerr << msg << std::endl;
}

#endif
