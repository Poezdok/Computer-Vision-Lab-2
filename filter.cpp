//
// Created by alex on 12.12.2021.
//

#include "filter.h"

#include <opencv2/imgproc.hpp>


using namespace cv;
using namespace std;


static void add_mask(Mat source, Mat destination, Mat mask);
static void mask_cycle(const Mat& source, const Mat& destination, const Mat& mask);


Mat average(const Mat& source, Size mask_size) {

    Mat mask = Mat::ones(mask_size, CV_8S);

    Mat result = source.clone();
    mask_cycle(source, result, mask);

    return result;

}


Mat laplacian(const Mat& source, double coefficient){

    Mat mask = Mat::zeros(Size(3, 3), CV_8S);

    int8_t mask_array[] = {
            0, 1, 0,
            1, -4, 1,
            0, 1, 0
    };

    for(auto i = 0; i < 9; i++){
        mask.at<int8_t>(i) = mask_array[i];
    }

    Mat laplace = source.clone();
    mask_cycle(source, laplace, mask);
    Mat result = source - coefficient*laplace;

    return result;

}


void mask_cycle(const Mat& source, const Mat& destination, const Mat& mask){

    auto width = source.size().width;
    auto height = source.size().height;

    for(int x = 0; x < width - mask.size().width; x++){
        for(int y = 0; y < height - mask.size().height; y++){
            Rect rect(Point2i(x, y), mask.size());
            auto src_roi = source(rect);
            auto dest_roi = destination(rect);
            add_mask(src_roi, dest_roi, mask);
        }
    }

}


void add_mask(Mat source, Mat destination, Mat mask){

    auto center_x = source.size().width / 2;
    auto center_y = source.size().height / 2;

    if(source.channels() == 1){

        int64_t gray = 0;

        for (auto i = 0; i < source.size().area(); i++){
            auto pixel = source.at<uint8_t>(i);
            auto multiplier = mask.at<int8_t>(i);
            gray += pixel * multiplier;

        }

        uint8_t new_center = gray/source.size().area();

        destination.at<uint8_t>(center_x, center_y) = new_center;

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


Mat box_unsharp(const Mat& source, Size box_size, double coefficient){

    auto blurred = average(source, box_size);
    Mat diff = source - blurred;
    Mat result = source + coefficient*diff;

    return result;

}


Mat gaussian_unsharp(const Mat& source, Size box_size, double sigmaX, double coefficient){

    Mat blurred;
    GaussianBlur(source, blurred, box_size, sigmaX, 0);

    Mat diff = source - blurred;
    Mat result = source + coefficient*diff;

    return result;

}


Mat laplace_unsharp(const Mat& source, double coefficient_laplace, double coefficient_unsharp){

    Mat blurred = laplacian(source, coefficient_laplace);

    Mat diff = source - blurred;
    Mat result = source + coefficient_unsharp*diff;

    return result;

}


double calculate_matched_ness(Mat a, Mat b){

    vector<uint64_t> diff = {0, 0, 0};
    vector<double> match_rgb = {0, 0, 0};
    double match_gray = 0;

    double matchedness = 0;

    switch (a.channels()){
        case 1:
            for (auto i = 0; i < a.size().area(); i++){
                auto a_pixel = a.at<uint8_t>(i);
                auto b_pixel = b.at<uint8_t>(i);

                if(max(a_pixel, b_pixel)){
                    match_gray += (double) min(a_pixel, b_pixel) / (double) max(a_pixel, b_pixel);
                }
            }

            matchedness = match_gray / a.size().area();
            break;
        case 3:
            for (auto i = 0; i < a.size().area(); i++) {
                auto a_pixel = a.at<Vec3b>(i);
                auto b_pixel = b.at<Vec3b>(i);

                for (auto j = 0; j < 3; j++) {
                    if(max(a_pixel[j], b_pixel[j])){
                        match_rgb[j] += (double) min(a_pixel[j], b_pixel[j]) / (double) max(a_pixel[j], b_pixel[j]);
                    }
                }
            }

            for(auto i = 0; i < 3; i++){
                matchedness += match_rgb[i] / a.size().area();
            }
            matchedness /= 3;
            break;
    }

    return matchedness;

}
