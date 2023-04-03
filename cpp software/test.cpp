#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudacodec.hpp>
#include<threads.h>

#include "communication.hpp"

using namespace std;
using namespace cv;
using namespace communication;

int main(){

    cuda::setDevice(0);  //set the cuda device to your graphic card

    VideoCapture cap (0); // open the video port 0
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 224);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 224);

    communication::TransfertDataToArduino communication; // create a object of the class DataFromArduino

    communication.init();

    int i(0);

    while (true){
    

        if (i == 0){
            communication.send("0");
            i++;
        }
        else {
            communication.send("1");
            i--;
        }
        
        cout <<"done"<<endl;
        sleep(3);
    }

    communication.close();
}