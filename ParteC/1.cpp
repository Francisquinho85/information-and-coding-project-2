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

Mat converted_image;
Mat original_image;

void BGRtoYUV420(){
    cvtColor(original_image, converted_image, COLOR_BGR2YUV_I420);
}

void predictorJPEGls(){
    for(int x=0; x<converted_image.rows; x++){
        for(int y=0; y<converted_image.cols; y++){
            int a,b,c = 0;
            if(x!=0 && y==0) b = converted_image.at<int>(x-1,y);
            else if(x==0 && y!=0) a = converted_image.at<int>(x,y-1);
            else if(x!=0 && y!=0){
                a = converted_image.at<int>(x,y-1);
                b = converted_image.at<int>(x-1,y);
                c = converted_image.at<int>(x-1,y-1);
            }
            int prev = 0;
            if(c>=max(a,b)) prev = min(a,b);
            else if(c<=min(a,b)) prev = max(a,b);
            else prev = a+b-c;

            int pixel = converted_image.at<int>(x,y);

            cout << "pixel " << pixel << " X " << prev << '\n';
        }
    }
}

int main(int argc, char** argv){
    original_image = imread(argv[1]);
    BGRtoYUV420();
    //predictorJPEGls();
    vector<Mat> bgr_planes;
    split( original_image, bgr_planes );
    vector<Mat> yuv_planes;
    split( converted_image, yuv_planes );


    Mat YUV(converted_image.rows * 2 / 3, converted_image.cols, CV_8UC3);
    insertChannel(converted_image(Rect(0, 0, YUV.cols, YUV.rows)), YUV, 0);

    Mat U = converted_image(Rect(0, YUV.rows, YUV.cols, YUV.rows / 4));
    imshow("U1", U);
    U = U.reshape(1, YUV.rows / 2);
    imshow("U2", U);
    resize(U, U, Size(YUV.cols, YUV.rows));
    insertChannel(U, YUV, 2);

    Mat V = converted_image(Rect(0, YUV.rows + YUV.rows/4, YUV.cols, YUV.rows / 4));
    V = V.reshape(1, YUV.rows / 2);
    resize(V, V, Size(YUV.cols, YUV.rows));
    insertChannel(V, YUV, 1);

    cvtColor(YUV, YUV, COLOR_YUV2BGR);
    imwrite("Restored.ppm", YUV);
}