//
//  Geometry.cpp
//  emptyExample
//
//  Created by David Stolarsky on 4/5/13.
//
//

#include "Geometry.h"

ofVec2f DNS::Geometry::Map(ofVec2f fromPoint, Bounds<float> fromBounds, Bounds<float> toBounds)
{
    ofVec2f toPoint;
    toPoint.x = ofMap(fromPoint.x, fromBounds.left, fromBounds.right, toBounds.left, toBounds.right);
    toPoint.y = ofMap(fromPoint.y, fromBounds.bottom, fromBounds.top, toBounds.bottom, toBounds.top);
    
    return toPoint;
}

ofVec2f DNS::Geometry::Normalize(ofVec2f point, Bounds<float> inBounds)
{
    return DNS::Geometry::Map(point, inBounds, Bounds<float>::normalBounds());
}

ofFloatColor DNS::Color::InterpolateLinear(ofFloatColor a, ofFloatColor b, float amount)
{
    return ofFloatColor( DNS::Geometry::InterpolateLinear(a.r, b.r, amount),
                         DNS::Geometry::InterpolateLinear(a.g, b.g, amount),
                         DNS::Geometry::InterpolateLinear(a.b, b.b, amount),
                         DNS::Geometry::InterpolateLinear(a.a, b.a, amount)  );
}

ofFloatColor DNS::Image::Sample(const ofFloatImage & image, ofVec2f pixelCoordinate)
{
    /*
     
    (0,0)--(1,0)--(2,0)--(3,0)--(4,0)-
      |      |      |      |      |
      |      |      |      |      |
      |      |      |      |      |
    (0,1)--(1,1)--(2,1)--(3,1)--(4,1)-
      |      |      |      |      |
      |      |      |      |      |
      |      |      |      |      |
    (0,2)--(1,2)--(2,2)--(3,2)--(4,2)-
      |      |      |      |      |
     
     */
    
    int col1 = floor(pixelCoordinate.x - 0.5);
    int col2 = col1 + 1;
    col1 = MAX(0, col1);
    col2 = MIN(image.width-1, col2);
    
    int row1 = floor(pixelCoordinate.y - 0.5);
    int row2 = row1 + 1;
    row1 = MAX(0, row1);
    row2 = MIN(image.height-1, row2);
    
    float intPart;
    
    float xAmount = modf(pixelCoordinate.x - 0.5, &intPart);
    float yAmount = modf(pixelCoordinate.y - 0.5, &intPart);
    
    ofFloatColor row1Color = DNS::Color::InterpolateLinear( image.getColor(col1, row1), image.getColor(col2, row1), xAmount );
    ofFloatColor row2Color = DNS::Color::InterpolateLinear( image.getColor(col1, row2), image.getColor(col2, row2), xAmount );
    ofFloatColor finalColor = DNS::Color::InterpolateLinear( row1Color, row2Color, yAmount );
    
    return finalColor;
}

float DNS::Geometry::InterpolateLinear(float a, float b, float amount)
{
    return (1.0 - amount) * a + amount * b;
}

ofVec2f DNS::Geometry::InterpolateLinear(ofVec2f a, ofVec2f b, float amount)
{
    return ofVec2f( DNS::Geometry::InterpolateLinear(a.x, b.x, amount),
                    DNS::Geometry::InterpolateLinear(a.y, b.y, amount)  );
}