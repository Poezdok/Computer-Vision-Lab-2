//
// Created by alex on 12.12.2021.
//

#include "filter.h"
#include "iostream"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"



using namespace cv;
using namespace std;

void add_mask(Mat source, Mat mask);

Mat mask_filtration(Mat source, Mat destination, Mat mask);
void add_mask(Mat src, Mat dest, Mat mask);

void filter(Mat source, int mask_size){




//    imshow("Res", result);
//    waitKey(0);
}

Mat average(Mat source, Size mask_size) {

    Mat mask = Mat::ones(mask_size, CV_8S);

    Mat result = source.clone();

    auto width = source.size().width;
    auto height = source.size().height;

    for(int x = 0; x < width - mask.size().width; x++){
        for(int y = 0; y < height - mask.size().height; y++){
            Rect rect(Point2i(x, y), mask.size());
            auto src_roi = source(rect);
            auto dest_roi = result(rect);
            add_mask(src_roi, dest_roi, mask);
        }
    }

    return result;

}

void add_mask(Mat source, Mat destination, Mat mask){

    auto center_x = source.size().width / 2;
    auto center_y = source.size().height / 2;

    if(source.channels() == 1){



    } else if (source.channels() == 3){


        vector<int64_t> rgb = {0, 0, 0};

        for (auto i = 0; i < source.size().area(); i++){
            auto pixel = source.at<Vec3b>(i);
            auto multiplier = mask.at<int8_t>(i);
            for (auto j = 0; j < 3; j++){
                rgb[j] += pixel[j] * multiplier;
            }
        }

        Vec3b new_center;
        for(auto i = 0; i < 3; i++){
            new_center[i] = rgb[i]/source.size().area();
        }
        destination.at<Vec3b>(center_x, center_y) = new_center;
    }
}




Mat laplacian(Mat source, double coefficient){


    Mat mask = Mat::zeros(Size(3, 3), CV_8S);

    int8_t mask_array[] = {
            0, 1, 0,
            1, -4, 1,
            0, 1, 0
    };

    for(auto i = 0; i < 9; i++){
        mask.at<int8_t>(i) = mask_array[i];
    }


    cout << mask << endl;

    Mat laplace = source.clone();

    auto width = source.size().width;
    auto height = source.size().height;

    for(int x = 0; x < width - mask.size().width; x++){
        for(int y = 0; y < height - mask.size().height; y++){
            Rect rect(Point2i(x, y), mask.size());
            auto src_roi = source(rect);
            auto dest_roi = laplace(rect);
            add_mask(src_roi, dest_roi, mask);
        }
    }

    Mat result = source - coefficient*laplace;


    return result;






}

Mat box_unsharp(Mat source, Size box_size, double coefficient){

    auto blurred = average(source, box_size);
    Mat diff = source - blurred;
    Mat result = source + coefficient*diff;

    return result;

}


Mat gaussian_unsharp(Mat source, Size box_size, double sigmaX, double coefficient){

    Mat blurred;
    GaussianBlur(source, blurred, box_size, sigmaX, 0);

    Mat diff = source - blurred;
    Mat result = source + coefficient*diff;

    return result;

}


Mat laplace_unsharp(Mat source, double coefficient_laplace, double coefficient_unsharp){

    Mat blurred = laplacian(source, coefficient_laplace);

    Mat diff = source - blurred;
    Mat result = source + coefficient_unsharp*diff;

    return result;

}


double calculate_matched_ness(Mat a, Mat b){
    uint64_t matched = 0;
    vector<uint64_t> diff = {0, 0, 0};
    vector<double> match = {0, 0, 0};

    if (a.size().area() != b.size().area()){

        cout << "Images has different sizes!" << endl;
        return 0;
    }
    if(a.channels() != b.channels()){

        cout << "Images has different numbers of channels!" << endl;
        return 0;

    }

    for (auto i = 0; i < a.size().area(); i++){
        switch (a.channels()){
            case 1:
                if(a.at<uint8_t>(i) == b.at<uint8_t>(i)){
                    matched++;
                }
                break;
            case 3:
                auto a_pixel = a.at<Vec3b>(i);
                auto b_pixel = b.at<Vec3b>(i);

                for (auto j = 0; j < 3; j++){

                    diff[j] += abs(a_pixel[j] - b_pixel[j]);
                    if(a_pixel[j] && b_pixel[j]) {
                        if (a_pixel[j] >= b_pixel[j]) {
                            match[j] += (double) b_pixel[j] / (double) a_pixel[j];
                        } else {
                            match[j] += (double) a_pixel[j] / (double) b_pixel[j];
                        }
                    }
                }

        }
    }

    double difference = 0;
    double matchedness = 0;

    for(auto i = 0; i < 3; i++){

//        cout << diff[i];

        difference += ((double) diff[i] / 255) / a.size().area();
//        cout << " " << difference;
        matchedness += match[i] / a.size().area();
//        cout << " " << match[i] << " " << matchedness << endl;
    }

    difference /= 3;
    matchedness /= 3;
//    cout << matchedness << endl;

    return matchedness;

}
