//
//  main.cpp
//  openCvTut
//
//  Created by Arshad Husain on 18-03-10.
//  Copyright © 2018 Arshad Husain. All rights reserved.
//

#include <iostream>
#include <vector>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Fruit.hpp"


using namespace cv;
//initial min and max HSV filter values
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;

//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

//max number of object to be detected in frame
const int MAX_NUM_OBJECTS = 50;

//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;

//names that will appear at the top of each window
const std::string windowName = "Original Image";
const std::string windowName1 = "HSV Image";
const std::string windowName2 = "Thresholded Image";
const std::string windowName3 = "After Morphological Operations";
const std::string trackbarWindowName = "Trackbars";

void on_trackbar(int, void*)
{
    //This function gets called whenever a trackbar position is changed
}

std::string intToString(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

void createTrackbars() {
    //create window for trackbars
    
    namedWindow(trackbarWindowName, 0);
    //create memory to store trackbar name on window
    
    char TrackbarName[50];
    
    /*Need to add conversion specifier %d*/
    sprintf(TrackbarName, "H_MIN", H_MIN);
    sprintf(TrackbarName, "H_MAX", H_MAX);
    sprintf(TrackbarName, "S_MIN", S_MIN);
    sprintf( TrackbarName, "S_MAX", S_MAX);
    sprintf( TrackbarName, "V_MIN", V_MIN);
    sprintf( TrackbarName, "V_MAX", V_MAX);
    //create trackbars and insert them into window
    //3 parameters are: the address of the variable that is changing when the trackbar is moved (e.g. H_LOW)
    //the max value the trackbar can move (e.g. H_HIGH),
    //and the function that is called wheneverthe trackbar is moved (e.g. on_trackbar)
    //                                  ---->    ---->     ---->
    
    createTrackbar("H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
}

void drawObject(vector<Fruit> theFruits, Mat &frame) {
    //use some of the OpenCV drawing functions to draw crosshairs on your tracked image
    
    //UPDATE:JUNE 18TH, 2013
    //added 'if' and 'else' statements to prevent
    //memory errors from writing off the screen (ie. (-25,-25) is not within the window!)
    
    /*
     * REVIEW ALL IF ELSE STATEMENTS
     * WHAT DOES Scalar() DO??
     */
    /*circle(frame, Point(x,y), 20, Scalar(0, 255, 0), 2);
    if(y-25>0)
    {
        line(frame, Point(x,y), Point(x,y-25), Scalar(0,255,0), 2);
    }
    else
    {
        line(frame, Point(x,y), Point(x,0), Scalar(0, 255, 0), 2);
    }
    
    if(y+25<FRAME_HEIGHT)
    {
        line(frame, Point(x,y), Point(x,y+25), Scalar(0,255,0), 2);
    }
    else
    {
        line(frame, Point(x,y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);
    }
    
    if(x-25>0)
    {
        line(frame, Point(x,y), Point(x-25, y), Scalar(0,255,0), 2);
    }
    else
    {
        line(frame, Point(x,y), Point(0,y), Scalar(0,255,0), 2);
    }
    
    if(x+25<FRAME_WIDTH)
    {
        line(frame, Point(x,y), Point(x+25,y), Scalar(0,255,0), 2);
    }
    else
    {
        line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);
    }
    
    putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);*/
    
    for(int i = 0; i < theFruits.size(); i++)
    {
        int x = theFruits.at(i).getXPos();
        int y = theFruits.at(i).getYPos();

        
        circle(frame, Point(x,y), 10, Scalar(0,0,255));
        putText(frame, intToString(x)+ " , " + intToString(y), Point(x,y+20),1,1,Scalar(0,255,0));
        
        putText(frame, theFruits.at(i).getType(), Point(theFruits.at(i).getXPos(), theFruits.at(i).getYPos()-20), 1, 2, theFruits.at(i).getColour());
    }
    
    

}

void morphOps(Mat &thresh) {
    //create structuring element that will be used to "dilate" and "erode" image.
    
    //the element chosen here is a 3px by 3px rectangle
    Mat erodeElement =  getStructuringElement(MORPH_RECT, Size(3,3));
    
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8,8));
    
    erode(thresh, thresh, erodeElement);
    erode(thresh, thresh, erodeElement);
    
    dilate(thresh, thresh, dilateElement);
    dilate(thresh, thresh, dilateElement);
}

void trackFilteredObject(Mat threshold, Mat HSV, Mat &cameraFeed)
{
    vector<Fruit> apples;
    
    Mat temp;
    threshold.copyTo(temp);
    
    //these two vectors needed for output of findContours
    std::vector<std::vector<Point> > contours;
    std::vector<Vec4i> heirarchy;
    
    //find contours of filtered image using OpenCV findContours function
    findContours(temp, contours, heirarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    
    if(heirarchy.size() > 0) {
        int numObjects = heirarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS) {
            for(int index = 0; index >= 0; index = heirarchy[index][0])
            {
                Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;
                
                //if the area is less than 20 px by 20px then it is probably noise
                
                
                /*REVIEW THE FUNCTIONALITY OF CODE FOLLOWING THIS COMMENT BLOCK*/
                /*if the area is the same as the 3/2 os the image size, probably just a bad filter we only want
                 the object with the largest area so we save a reference area each iteration and compare it to the
                 area in the next iteration*/
                
                if(area>MIN_OBJECT_AREA)
                {
                    Fruit apple;
                    
                    apple.setXPos(moment.m10/area);
                    apple.setYPos(moment.m01/area);
                    
                    apples.push_back(apple);
                    
                    //x = moment.m10/area;
                    //y = moment.m01/area;
                    
                    
                    objectFound = true;
                } else {
                    objectFound = false;
                }
            }
            //let user know you found an object
            if(objectFound == true) {
                //draw object location on screen
                drawObject(apples, cameraFeed);
            } else {
                putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0,50), 1, 2, Scalar(0,0,255), 2);
            }
        }
    }
}

void trackFilteredObject(Fruit theFruit, Mat threshold, Mat HSV, Mat &cameraFeed)
{
    vector<Fruit> apples;
    
    Mat temp;
    threshold.copyTo(temp);
    
    //these two vectors needed for output of findContours
    std::vector<std::vector<Point> > contours;
    std::vector<Vec4i> heirarchy;
    
    //find contours of filtered image using OpenCV findContours function
    findContours(temp, contours, heirarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    //use moments method to find our filtered object
    double refArea = 0;
    bool objectFound = false;
    
    if(heirarchy.size() > 0) {
        int numObjects = heirarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS) {
            for(int index = 0; index >= 0; index = heirarchy[index][0])
            {
                Moments moment = moments((cv::Mat)contours[index]);
                double area = moment.m00;
                
                //if the area is less than 20 px by 20px then it is probably noise
                
                
                /*REVIEW THE FUNCTIONALITY OF CODE FOLLOWING THIS COMMENT BLOCK*/
                /*if the area is the same as the 3/2 os the image size, probably just a bad filter we only want
                 the object with the largest area so we save a reference area each iteration and compare it to the
                 area in the next iteration*/
                
                if(area>MIN_OBJECT_AREA)
                {
                    Fruit apple;
                    
                    apple.setXPos(moment.m10/area);
                    apple.setYPos(moment.m01/area);
                    
                    apple.setType(theFruit.getType());
                    apple.setColour(theFruit.getColour());
                    
                    apples.push_back(apple);
                    
                    //x = moment.m10/area;
                    //y = moment.m01/area;
                    
                    
                    objectFound = true;
                } else {
                    objectFound = false;
                }
            }
            //let user know you found an object
            if(objectFound == true) {
                //draw object location on screen
                drawObject(apples, cameraFeed);
            } else {
                putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0,50), 1, 2, Scalar(0,0,255), 2);
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    //if we would like to calibrate our filter values, set to true
    bool calibrationMode = false;
    
    //Matrix to sure each frame of the webcam feed
    Mat cameraFeed;
    
    //matrix storage for HSV image
    Mat HSV;
    
    //matrix storage for binary threshold image
    Mat threshold;
    
    //x and y values for the location of the object
    int x=0, y=0;
    

    if(calibrationMode)
    {
        //create slider bars for HSV filtering
        createTrackbars();
    }
    
    //video capture object to acquire webcam feed
    VideoCapture capture;
    
    //open capture object at location zero (default location for webcam)
    capture.open(0);
    
    //set height and width of capture frame
    capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    
    //start an infinite loop where webcam feed is copied to cameraFeed matrix
    //all of our operations will be performed withn this loop
    
    
    while(1)
    {
        //store image to matrix
        capture.read(cameraFeed);
        
        //convert frame from BGR to HSV colourspace
        cvtColor(cameraFeed, HSV, CV_BGR2HSV);
        //filter HSV image between values and store filters image to threshold matrix
        
        if(calibrationMode == true)
        {
            //convert frame from BGR to HSV colourspace
            cvtColor(cameraFeed, HSV, CV_BGR2HSV);
            
            inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
            morphOps(threshold);
            
            imshow(windowName2, threshold);
            moveWindow(windowName2, 0, 500);
            
            trackFilteredObject(threshold, HSV, cameraFeed);
            
            //cout << "MAT " << HSV << endl;
        } else {
            
            Fruit apple("apple"), banana, cherry("cherry");
            
            apple.setHSVmin(Scalar(125,89,86)); //Substituted for BLUE pen ink colour
            apple.setHSVmax(Scalar(155,256,145)); //Substituted for BLUE pen ink colour
            
            cherry.setHSVmin(Scalar(167,111,168)); //Substituted for RED pen ink colour
            cherry.setHSVmax(Scalar(202,256,256)); //Substituted for RED pen ink colour
            
            //convert frame from BGR to HSV colourspace
            cvtColor(cameraFeed, HSV, CV_BGR2HSV);
            inRange(HSV, apple.getHSVmin(), apple.getHSVmax(), threshold);
            morphOps(threshold);
            trackFilteredObject(apple, threshold, HSV, cameraFeed);
            
            /*cvtColor(cameraFeed, HSV, CV_BGR2HSV);
            inRange(HSV, banana.getHSVmin(), banana.getHSVmax(), threshold);
            morphOps(threshold);
            trackFilteredObject(threshold, HSV, cameraFeed);*/
            
            cvtColor(cameraFeed, HSV, CV_BGR2HSV);
            inRange(HSV, cherry.getHSVmin(), cherry.getHSVmax(), threshold);
            morphOps(threshold);
            trackFilteredObject(cherry, threshold, HSV, cameraFeed);
        }
        
        //show frames
        imshow(windowName, cameraFeed);
        moveWindow(windowName, 900, 0);
        
        //imshow(windowName1, HSV);
        //moveWindow(windowName1, 300, 0);
        
        
        
        //delay 30ms so that screen can refresh.
        //image will not appear without this waitKey() command
        
        waitKey(30);
    }
    
    return 0;
}
