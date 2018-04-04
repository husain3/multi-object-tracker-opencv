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
    
    Fruit(string name);
    
    int getXPos();
    void setXPos(int x);
    
    int getYPos();
    void setYPos(int y);
    
    Scalar getHSVmax();
    Scalar getHSVmin();
    
    void setHSVmin(Scalar min);
    void setHSVmax(Scalar max);
    
    string getType() {
        return type;
    }
    
    void setType(string t) { type = t; }
    
    Scalar getColour() {
        return Colour;
    }
    
    void setColour(Scalar c) {
        Colour = c;
    }
    
private:
    int xPos, yPos;
    string type;
    
    Scalar HSVmin, HSVmax;
    Scalar Colour;
};

#endif /* Fruit_hpp */
