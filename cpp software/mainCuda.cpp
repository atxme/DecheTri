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

#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudacodec.hpp>

#ifndef __include_communication__
#define __include_communication__
#include "communication.hpp"
#endif


using namespace std;
using namespace cv;
using namespace communication;

const string classification[] ={"can","plastic"};


int main(int argc , char ** argv ){

    cuda::setDevice(0);  //set the cuda device to your graphic card

    dnn::Net net = dnn::readNetFromONNX("torchNet_test4.onnx"); // load the model
    net.setPreferableBackend(dnn::DNN_BACKEND_CUDA); // set the backend to cuda
    net.setPreferableTarget(dnn::DNN_TARGET_CUDA); // set the target to cuda

    if (net.empty()) { //check if weights are correctly loaded 
        cerr<< "Cannot load the model" << endl;  
        return -1;
    }

    communication::TransfertDataToArduino communication; // create a object of the class DataFromArduino
    communication.init();  //init the communication with the arduino card

    cuda::GpuMat frame,resizedFrame;
    VideoCapture cap;
    

    if (!cap.isOpened()) {
        cout << "Cannot open the video cam" << endl;
        cout << "try to change the video port for the camera" << endl;
        return -1;
    }

    while (true) {

        if(!cap.read(frame)) { //check if the frame is correctly read
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        cuda::resize(frame,resizedFrame,cv::Size(225,225),0,0,INTER_LINEAR); // resize the frame to 225x225
        cuda::cvtColor(resizedFrame,resizedFrame,COLOR_BGR2RGB); // convert the frame to RGB
        cv::cuda::divide(resizedFrame, cv::Scalar(255.0), resizedFrame, 1, -1, cv::cuda::Stream::Null());
        resizedFrame.convertTo(resizedFrame, CV_32F, 1.0 / 255.0);  // convert the frame to float from 0-255 to 0-1
        
        net.setInput(resizedFrame); // set the input of the model
        Mat result = net.forward(); // forward the frame to the model
        
        float result1= result.at<float>(0,0); // get the result of the first class
        float result2= result.at<float>(0,1); // get the result of the second class

        if (result1<result2){
            communication.send(classification[1]); // send the result to the arduino card
            cout << classification[1] <<" have been detected "<<endl; // print the result on the terminal
        }
        else {
            communication.send(classification[0]); // send the result to the arduino card
            cout << classification[0] <<" have been detected "<<endl; // print the result on the terminal
        }

        char key= waitKey(); // wait for a key to be pressed
        if (key=='q') { // if the key is the escape key
            cout << "The program is going to stop" << endl;
            break; // break the loop
        }
    }
    cv::destroyAllWindows(); // destroy all the windows
    cap.release(); // release the camera
    communication.close(); // close the communication with the arduino card
}