#include <iostream>
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// g++ 1.cpp -o 1 -std=c++11 `pkg-config --cflags --libs opencv`

using namespace cv;
using namespace std;


int main(int argc, char** argv){
    Mat original_image = imread(argv[1]);
    Mat converted_image;
    cvtColor(original_image, converted_image, COLOR_BGR2YUV_I420);
    vector<Mat> bgr_planes;
    split( original_image, bgr_planes );
    vector<Mat> yuv_planes;
    split( converted_image, yuv_planes );
    cout << "BGR" << original_image.at<Vec3b>(0,0) << "YUV" << converted_image.at<Vec3b>(0,0) << '\n';
}