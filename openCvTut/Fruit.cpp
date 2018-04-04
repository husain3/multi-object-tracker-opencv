//
//  Fruit.cpp
//  openCvTut
//
//  Created by Arshad Husain on 18-03-12.
//  Copyright © 2018 Arshad Husain. All rights reserved.
//

#include "Fruit.hpp"

Fruit::Fruit(void)
{
    
}

Fruit::~Fruit(void)
{
    
}

Fruit::Fruit(string name) {
    setType(name);
    
    if(name == "apple") {
        setHSVmin(Scalar(125,89,86)); //Substituted for BLUE pen ink colour
        setHSVmax(Scalar(155,256,145)); //Substituted for BLUE pen ink colour
        
        setColour(Scalar(255,97,0));
    }
    
    if(name == "cherry") {
        setHSVmin(Scalar(167,111,168)); //Substituted for RED pen ink colour
        setHSVmax(Scalar(202,256,256)); //Substituted for RED pen ink colour
        
        setColour(Scalar(0,0,255));
    }
}

int Fruit::getXPos() {
    return Fruit::xPos;
}

void Fruit::setXPos(int x) {
    Fruit::xPos = x;
}

int Fruit::getYPos() {
    return Fruit::yPos;
}

void Fruit::setYPos(int y) {
    Fruit::yPos = y;
}

Scalar Fruit::getHSVmin() {
    return Fruit::HSVmin;
}

Scalar Fruit::getHSVmax() {
    return Fruit::HSVmax;
}

void Fruit::setHSVmin(Scalar min) {
    Fruit::HSVmin = min;
}

void Fruit::setHSVmax(Scalar max) {
    Fruit::HSVmax = max;
}
