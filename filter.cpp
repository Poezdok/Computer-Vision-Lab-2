//
// Created by alex on 12.12.2021.
//

#include "filter.h"
#include "iostream"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


using namespace cv;
using namespace std;

void average(Mat source);
void filtration(Mat source, Mat mask);


void filter(Mat source, int mask_size){

    auto width = source.size().width;
    auto height = source.size().height;

    cout << width << " " << height << endl;




    for(int x = 0; x < width - mask_size; x += 1){
        for(int y = 0; y < height - mask_size; y += 1){
            Rect rect(x, y, mask_size, mask_size);
            auto roi = source(rect);
            average(roi);

        }

    }

}



void average(Mat source) {



    Mat mask = Mat::ones(source.size(), CV_8U);

    filtration(source, mask);

}


void filtration(Mat source, Mat mask){

    auto size = source.size();

    auto center_x = size.width / 2;
    auto center_y = size.height / 2;

    switch (source.channels()){
        case 1:
        {
            uint64_t sum = 0;
            for (auto i = 0; i < size.area(); i++) {
                auto pixel = source.at<Point3i>(i);

            }
        }
            break;

        case 3:
        {
            Vec3i sum;
            for (auto i = 0; i < size.area(); i++) {
                auto pixel = source.at<Vec3b>(i);
                sum += pixel;
            }
            auto average = sum / size.area();
            source.at<Vec3b>(center_x, center_y) = average;
        }
            break;
    }



}