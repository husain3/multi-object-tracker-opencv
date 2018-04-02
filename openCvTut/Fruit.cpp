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
