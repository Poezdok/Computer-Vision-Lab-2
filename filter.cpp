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

    Mat copied = source.clone();



    mask_filtration(copied, mask);
    return copied;

}

Mat laplacian(Mat source, double coefficient){

}


Mat unsharp_masking(Mat source, Size mask_size, double coefficient){

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

double calculate_matched_ness(Mat a, Mat b){
    uint64_t mismatched = 0;
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
                if(a.at<uint8_t>(i) != b.at<uint8_t>(i)){
                    mismatched++;
                }
            case 3:
                if(a.at<Vec3b>(i) != b.at<Vec3b>(i)){
                    mismatched++;
                }
        }
    }

    double difference = ((double) mismatched) / (double) a.size().area();

    return difference;

}
