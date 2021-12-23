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
Mat y,cb,cr,cb2;
Mat pred_y,pred_cb,pred_cr;

void BGRtoYUV420(){
    cvtColor(original_image, converted_image, COLOR_BGR2YUV_I420);
    y = converted_image(Range(0,converted_image.rows*2/3), Range(0,converted_image.cols));
    cb = converted_image(Range(converted_image.rows*2/3,converted_image.rows*2/3+((converted_image.rows-converted_image.rows*2/3)/2)),
        Range(converted_image.cols/2,converted_image.cols));
    cb2 = converted_image(Range(converted_image.rows*2/3,converted_image.rows*2/3+((converted_image.rows-converted_image.rows*2/3)/2)),
        Range(0,converted_image.cols/2));
    cr = converted_image(Range(converted_image.rows*2/3+((converted_image.rows-converted_image.rows*2/3)/2),converted_image.rows), 
        Range(converted_image.cols/2,converted_image.cols));
    imshow("y",y);
    imshow("cb",cb);
    imshow("cr",cr);
    imshow("cb2",cb2);
    for(int x=0; x<cb.rows; x++){
        for(int y=0; y<cb.cols; y++){
            int x1 = cb2.at<int>(x,y);
            int x2 = cb.at<int>(x,y);
            //cout << x1 << " X " << x2  << '\n';
        }
    }
 }


void predictorJPEGls(Mat img, Mat &predictor){
    int a,b,c;
    for(int x=0; x<img.rows; x++){
        for(int y=0; y<img.cols; y++){
            a = 0;
            b = 0;
            c = 0;
            if(x!=0 && y==0) b = img.at<uchar>(x-1,y);
            else if(x==0 && y!=0) a = img.at<uchar>(x,y-1);
            else if(x!=0 && y!=0){
                a = img.at<uchar>(x,y-1);
                b = img.at<uchar>(x-1,y);
                c = img.at<uchar>(x-1,y-1);
            }
            int prev = 0;
            if(c>=max(a,b)) prev = min(a,b);
            else if(c<=min(a,b)) prev = max(a,b);
            else prev = a+b-c;

            int pixel = img.at<uchar>(x,y);
            predictor.at<uchar>(x,y) = pixel - prev;
            
            //cout << "pixel " << pixel << " X " << prev << " - " << pixel - prev <<'\n';
        }
    }
    //cout << (int)predictor.at<uchar>(0,0) << " - " << (int)img.at<uchar>(0,0) <<'\n';
}

int main(int argc, char** argv){
    original_image = imread(argv[1],IMREAD_COLOR);
    BGRtoYUV420();
    y.copyTo(pred_y);
    predictorJPEGls(y,pred_y);
    cb.copyTo(pred_cb);
    predictorJPEGls(cb,pred_cb);
    cr.copyTo(pred_cr);
    predictorJPEGls(cr,pred_cb);
    imshow("Restored", converted_image);
    imshow("Restd", original_image);
    waitKey();
    return EXIT_SUCCESS;
}