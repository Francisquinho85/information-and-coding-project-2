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
Mat y,cb,cr;

void BGRtoYUV420(){
    cvtColor(original_image, converted_image, COLOR_BGR2YUV_I420);
    y = converted_image(Range(0,converted_image.rows*2/3), Range(0,converted_image.cols));
    cb = converted_image(Range(converted_image.rows*2/3,converted_image.rows*2/3+((converted_image.rows-converted_image.rows*2/3)/2)),
        Range(converted_image.cols/2,converted_image.cols));
    cr = converted_image(Range(converted_image.rows*2/3+((converted_image.rows-converted_image.rows*2/3)/2),converted_image.rows), 
        Range(converted_image.cols/2,converted_image.cols));
    imshow("y",y);
    imshow("cb",cb);
    imshow("cr",cr);
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
    original_image = imread(argv[1],IMREAD_COLOR);
    BGRtoYUV420();
    //predictorJPEGls();   
    imshow("Restored", converted_image);
    imshow("Restd", original_image);
    waitKey();
    return EXIT_SUCCESS;
}