//
// Created by alex on 12.12.2021.
//

#include "filter.h"
#include "iostream"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


using namespace cv;
using namespace std;

void add_mask(Mat source, Mat mask);

void mask_filtration(Mat source, Mat mask);


void filter(Mat source, int mask_size){




//    imshow("Res", result);
//    waitKey(0);
}

Mat average(Mat source, Size mask_size) {
    Mat mask = Mat::ones(mask_size, CV_8S);

    cout << mask << endl;

    Mat copied;
    source.copyTo(copied);

    mask_filtration(copied, mask);
    return copied;

}

Mat laplacian(Mat source, double coefficient){

    Mat mask = Mat::zeros(3, 3, CV_8S);
    int8_t laplace_mask[] = {
            0, 1, 0,
            1, -4, 1,
            0, 1, 0
    };

    for(auto i = 0; i < 9; i++){
        mask.at<int8_t>(i) = laplace_mask[i];
    }


    Mat subtract_mask;
//    source.copyTo(subtract_mask);
    source.convertTo(subtract_mask, CV_8U);
    mask_filtration(subtract_mask, mask);
    imshow("Mask", subtract_mask);
    waitKey(0);
    auto result = source + coefficient*subtract_mask;

    return result;

}


Mat unsharp_masking(Mat source, Size mask_size, double coefficient){
    Mat mask = average(source, mask_size);
    Mat delta = source - mask;
    Mat result = source + delta*coefficient;
    return result;
}



void mask_filtration(Mat source, Mat mask){

    auto width = source.size().width;
    auto height = source.size().height;



    for(int x = 0; x < width - mask.size().width; x++){
        for(int y = 0; y < height - mask.size().height; y++){
            Rect rect(Point2i(x, y), mask.size());
            auto roi = source(rect);
            add_mask(roi, mask);

        }
    }


}


void add_mask(Mat source, Mat mask) {

    auto size = source.size();

    auto center_x = size.width / 2;
    auto center_y = size.height / 2;
//    cout << "Source: " << source << endl;
//    cout << "Mask: " << mask << endl;
    switch (source.channels()) {
        case 1: {
            int64_t sum = 0;
            for (auto i = 0; i < size.area(); i++) {
                auto pixel = source.at<int8_t>(i);
                sum += pixel * mask.at<int8_t>(i);
            }
        }
            break;
        case 3: {
            Vec3i sum;
            for (auto i = 0; i < size.area(); i++) {
                auto pixel = source.at<Vec3b>(i);
//                cout << pixel << " ";
//                for (int j = 0; j < 3; j++) {
//                    sum[j] += pixel[j] * mask.at<int8_t>(i);
//                }

                sum += Vec3i(pixel) * mask.at<int8_t>(i);
//                cout << i << " " <<   sum << " " << int(mask.at<int8_t>(i)) << endl;
            }

            auto average = sum / size.area();
//            cout << sum << average << endl;
//            cout << "Center " << source.at<Vec3b>(center_x, center_y) << endl;
            source.at<Vec3b>(center_x, center_y) = Vec3b(average);

//            cout << source;
//            waitKey(0);


        }
            break;
    }
}


