#include <iostream>
#include <stdio.h>
#include<fstream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../ParteA_Classes/Golomb.hpp"
// g++ 2.cpp ../ParteA_Classes/BitStream.cpp ../ParteA_Classes/Golomb.cpp -o 2 -std=c++11 `pkg-config --cflags --libs opencv`

using namespace cv;
using namespace std;

Mat converted_image;
Mat original_image;
Mat reduce_img;
Mat y,cb,cr,cb2;
Mat pred_y,pred_cb,pred_cr;
Mat dec_y,dec_cb,dec_cr;
Mat desp_y,desp_cb,desp_cr;
Mat resultYUV;
Mat result;

void BGRtoYUV420(){
    cvtColor(reduce_img, converted_image, COLOR_BGR2YUV_I420);
    y = converted_image(Range(0,converted_image.rows*2/3), Range(0,converted_image.cols));
    cb = converted_image(Range(converted_image.rows*2/3,converted_image.rows*2/3+((converted_image.rows-converted_image.rows*2/3)/2)),
        Range(0,converted_image.cols));
    cr = converted_image(Range(converted_image.rows*2/3+((converted_image.rows-converted_image.rows*2/3)/2),converted_image.rows), 
        Range(0,converted_image.cols));
    // imshow("y",y);
    // imshow("cb",cb);
    // imshow("cr",cr);
 }


 void YUV420toBGR(){
     desp_y.copyTo(resultYUV(Range(0, desp_y.rows),Range(0,desp_y.cols)));
     desp_cb.copyTo(resultYUV(Range(desp_y.rows, (desp_y.rows+desp_cb.rows)),Range(0,desp_cb.cols)));
     desp_cr.copyTo(resultYUV(Range((desp_y.rows+desp_cb.rows), resultYUV.rows),Range(0,desp_cr.cols)));
     cvtColor(resultYUV, result, COLOR_YUV2BGR_I420);

}

void predictorJPEGls(Mat img, Mat &predictor){
    int a,b,c;
    for(int x=0; x<img.rows; x++){
        for(int y=0; y<img.cols; y++){
            a = 0;
            b = 0;
            c = 0;
            if(x!=0 && y==0) b = (int)img.at<char>(x-1,y);
            else if(x==0 && y!=0) a = (int)img.at<char>(x,y-1);
            else if(x!=0 && y!=0){
                a = (int)img.at<char>(x,y-1);
                b = (int)img.at<char>(x-1,y);
                c = (int)img.at<char>(x-1,y-1);
            }
            int prev = 0;
            if(c>=max(a,b)) prev = min(a,b);
            else if(c<=min(a,b)) prev = max(a,b);
            else prev = a+b-c;

            int pixel = img.at<char>(x,y);
            predictor.at<char>(x,y) = pixel - prev;
            
            //cout << "pixel " << pixel << " X " << prev << " - " << pixel - prev <<'\n';
        }
    }
    //cout << (int)predictor.at<char>(0,0) << " - " << (int)img.at<char>(0,0) <<'\n';
}

void despredictor(Mat img, Mat &despredictor){
    int a,b,c;
    for(int x=0; x<img.rows; x++){
        for(int y=0; y<img.cols; y++){
            a = 0;
            b = 0;
            c = 0;
            if(x!=0 && y==0) b = (int)despredictor.at<char>(x-1,y);
            else if(x==0 && y!=0) a = (int)despredictor.at<char>(x,y-1);
            else if(x!=0 && y!=0){
                a = (int)despredictor.at<char>(x,y-1);
                b = (int)despredictor.at<char>(x-1,y);
                c = (int)despredictor.at<char>(x-1,y-1);
            }
            int prev = 0;
            if(c>=max(a,b)) prev = min(a,b);
            else if(c<=min(a,b)) prev = max(a,b);
            else prev = a+b-c;

            despredictor.at<char>(x,y) = (int)img.at<char>(x,y) + prev;
            //cout << (int)despredictor.at<char>(x,y) << " " << (int)converted_image.at<char>(x,y) << '\n';
            //cout << a << ' ' << b << ' ' << c << ' ' << (int)img.at<char>(x,y) << ' ' << prev <<'\n';
            //cout << y << " next " << '\n';
        }
    }
}

void encode(int m, char* file){

    //pred_y,pred_cb,pred_cr;
    Golomb g(m,file);
    g.openBsw();
    for(int x=0; x<pred_y.rows; x++){
        for(int y=0; y<pred_y.cols; y++){
            //cout << (int)pred_y.at<char>(x,y) << '\n';
            g.encode(pred_y.at<char>(x,y));
        }
    }
    for(int x=0; x<pred_cb.rows; x++){
        for(int y=0; y<pred_cb.cols; y++){
            //cout << (int)pred_cb.at<char>(x,y) << '\n';
            g.encode(pred_cb.at<char>(x,y));
        }
    }
    for(int x=0; x<pred_cr.rows; x++){
        for(int y=0; y<pred_cr.cols; y++){
            //cout << (int)pred_cr.at<char>(x,y) << '\n';
            g.encode(pred_cr.at<char>(x,y));
        }
    }
    g.closeBsw();
}

void decode(int m, char* file, Mat &dy, Mat &dcb, Mat &dcr){
    Golomb g(m,file);
    g.openBsr();
    for(int x=0; x<pred_y.rows; x++){
        for(int y=0; y<pred_y.cols; y++){
            char a = g.decode();
            dy.at<char>(x,y) = a;
            //cout << (int)dec_y.at<char>(x,y) << " x " <<(int)pred_y.at<char>(x,y)<< '\n';
        }
    }
    for(int x=0; x<pred_cb.rows; x++){
        for(int y=0; y<pred_cb.cols; y++){
            char a = g.decode();
            dcb.at<char>(x,y) = a;
            //cout << (int)dec_cb.at<char>(x,y) << " x " <<(int)pred_cb.at<char>(x,y)<< '\n';
        }
    }
    for(int x=0; x<pred_cr.rows; x++){
        for(int y=0; y<pred_cr.cols; y++){
            char a = g.decode();
            dcr.at<char>(x,y) = a;
            //cout << (int)dec_cr.at<char>(x,y) << " x " <<(int)pred_cr.at<char>(x,y)<< '\n';
        }
    }
    g.closeBsr();
}

void reduceBits(int bits, Mat &reduce_img){
    for(int y=0; y<original_image.cols; y++){
        for(int x=0; x<original_image.rows; x++){
            Vec3b a = original_image.at<Vec3b>(x,y);
            a[0] = (a[0]>>bits); 
            a[1] = (a[1]>>bits);
            a[2] = (a[2]>>bits);
            reduce_img.at<Vec3b>(x,y) = a;
        }
    }
}

void resetBits(int bits, Mat &result){
    for(int y=0; y<result.cols; y++){
        for(int x=0; x<result.rows; x++){
            Vec3b a = result.at<Vec3b>(x,y);
            a[0] = (a[0]<<bits); 
            a[1] = (a[1]<<bits);
            a[2] = (a[2]<<bits);
            result.at<Vec3b>(x,y) = a;
        }
    }
}

int main(int argc, char** argv){

    std::ofstream clear_f;
    clear_f.open(argv[2], std::ofstream::out | std::ofstream::trunc);
    clear_f.close();

    original_image = imread(argv[1],IMREAD_COLOR);

    reduce_img = Mat::zeros(Size(original_image.cols,original_image.rows),CV_8UC3);
    int bits = atoi(argv[4]);
    reduceBits(bits,reduce_img);

    BGRtoYUV420();
    pred_y = Mat::zeros(y.rows, y.cols,CV_8U);
    predictorJPEGls(y,pred_y);
    pred_cb = Mat::zeros(cb.rows, cb.cols,CV_8U);
    predictorJPEGls(cb,pred_cb);
    pred_cr = Mat::zeros(cr.rows, cr.cols,CV_8U);
    predictorJPEGls(cr,pred_cr);

    int m = atoi(argv[3]);

    encode(m,argv[2]);
    dec_y = Mat::zeros(y.rows, y.cols,CV_8U);
    dec_cb = Mat::zeros(cb.rows, cb.cols,CV_8U);
    dec_cr = Mat::zeros(cr.rows, cr.cols,CV_8U);
    decode(m,argv[2],dec_y,dec_cb,dec_cr);

    desp_y = Mat::zeros(y.rows, y.cols,CV_8U);
    despredictor(dec_y,desp_y);
    desp_cb = Mat::zeros(cb.rows, cb.cols,CV_8U);
    despredictor(dec_cb,desp_cb);
    desp_cr = Mat::zeros(cr.rows, cr.cols,CV_8U);
    despredictor(dec_cr,desp_cr);
    
    resultYUV = Mat::zeros(converted_image.rows, converted_image.cols,CV_8U);
    YUV420toBGR();

    resetBits(bits, result);

    // imshow("r_y",desp_y);
    // imshow("r_cb",desp_cb);
    // imshow("r_cr",desp_cr);
    imshow("original2YUV", converted_image);
    imshow("Original", original_image);
    imshow("reduced",reduce_img);
    imshow("resultYUV",resultYUV);
    imshow("result",result);

    ifstream size_f(argv[2], ios::binary);
    size_f.seekg(0, ios::end);
    int file_size = size_f.tellg();
    cout<<"Size of the result file is"<<" "<< file_size<<" "<<"bytes\n";
    size_f.close();
    ifstream size_i(argv[1], ios::binary);
    size_i.seekg(0, ios::end);
    int img_size = size_i.tellg();
    cout<<"Size of the original image is"<<" "<< img_size<<" "<<"bytes\n";
    size_i.close();
    waitKey();
    return EXIT_SUCCESS;
}