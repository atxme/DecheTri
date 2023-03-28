/*
||---------------------------------------------------------------------------------------------||
||---------------------------------------------------------------------------------------------||
||                                                                                             ||
|| This software is a private part of the project "waste sorting by artificial intelligence".  ||
|| The software is provided "as is" without warranty of any kind.                              ||
|| The author of the software is not responsible for any damage caused by the software.        ||
|| You will be unable to use this software without the agreement of the autor                  ||
|| It's forbidden to use it without the agreement of the autor                                 ||
|| This software use a opencv librabry which have been boosted using cuda                      ||
|| You will be unable to use this software without recompiling opencv binaries                 ||
||                                                                                             ||
||   ";)/""                                                                                    ||
||---------------------------------------------------------------------------------------------||
||---------------------------------------------------------------------------------------------||
*/

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

// Include OpenCV's gpu C++ headers files 
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>


#include "communication.hpp"

using namespace std;
using namespace cv;
using namespace communication;

string classification =[ "can","plastic"];

int main(int argc , char** argv)
{
    communication::DataFromArduino communcation; // create a object of the class DataFromArduino
    communcation.init();  //init the communication with the arduino card 

    dnn::NET net = dnn::readNetFromONNX("torchNet_test4.onnx"); // load the model

    if net.isempty() {
        cerr<< "Cannot load the model" << endl;   //check if the model is correctly loaded 
        return -1;
    }
        
    net.setPreferableBackend(dnn::DNN_BACKEND_CUDA); // set the backend to cuda
    net.setPreferableTarget(dnn::DNN_TARGET_CUDA); // set the target to cuda

    cap = VideoCapture(0);  

    if(!cap.isOpened())
    {
        cout << "Cannot open the video cam" << endl;
        cout << "try to change the video port for the camera" << endl;
        return -1;
    }

    while true {
        cap.read(frame); //take a frame from the camera
        if frame.empty() {
            cout << "Cannot read a frame from video stream" << endl;
            cout << "try to change the video port for the camera" << endl;
            break;
        }
        imshow("Frame", frame); // show the frame on the screen

        Mat blob = dnn::blobFromImage(frame, 1.0, Size(225, 225), Scalar(0, 0, 0), true, false); // create a blob from the frame
        net.setInput(blob); // set the blob as input of the model

        Mat result = net.fordward(); // compute the result of the model

        float result1= result.at<float>(0,0); // get the result of the first class
        float result2= result.at<float>(0,1); // get the result of the second class

        if (result1<result2){
            communcation.send(classification[1]); // send the result to the arduino card
            cout << classification[1] <<" have been detected "<<endl; // print the result on the terminal
        }
        else {
            communcation.send(classification[0]); // send the result to the arduino card
            cout << classification[0] <<" have been detected "<<endl; // print the result on the terminal
        }

        char key= waitkey(); // wait for a key to be pressed
        if (key=='q') { // if the key is the escape key
            cout << "The program is going to stop" << endl;
            break; // break the loop
        }

    }

destroyAllwindows(); // destroy all the windows
cap.release(); // release the camera
communcation.close(); // close the communication with the arduino card
}