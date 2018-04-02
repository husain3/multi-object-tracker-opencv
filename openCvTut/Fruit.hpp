//
//  Fruit.hpp
//  openCvTut
//
//  Created by Arshad Husain on 18-03-12.
//  Copyright © 2018 Arshad Husain. All rights reserved.
//

#ifndef Fruit_hpp
#define Fruit_hpp

#pragma once
#include <stdio.h>
#include <string>

#include <opencv/cv.h>

using namespace std;
using namespace cv;

class Fruit
{
public:
    Fruit(void);
    ~Fruit(void);
    
    int getXPos();
    void setXPos(int x);
    
    int getYPos();
    void setYPos(int y);
    
    Scalar getHSVmax();
    Scalar getHSVmin();
    
    void setHSVmin(Scalar min);
    void setHSVmax(Scalar max);
    
private:
    int xPos, yPos;
    string type;
    
    Scalar HSVmin, HSVmax;
};

#endif /* Fruit_hpp */
