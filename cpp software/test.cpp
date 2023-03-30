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




using namespace std;
using namespace cv;


int main(){

    cuda::setDevice(0);  //set the cuda device to your graphic card

    VideoCapture cap (0); // open the video port 0
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 224);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 224);

    while (true){
        Mat cpuFrame;
        cap >> cpuFrame;
        imshow("cpuFrame",cpuFrame);
        waitKey(1);
    }
}