#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/intensity_transform.hpp"


#include "filter.h"

using namespace cv;
using namespace std;


int main() {

    Mat lenna = imread("../Lenna.png");
    cv::resize(lenna, lenna, Size(1024, 1024));

    auto avg = average(lenna, Size(3, 3));
    Mat blurred;
    blur(lenna, blurred, Size(3, 3));

    auto diff = blurred - avg;

    auto matchedness = calculate_matched_ness(blurred, avg);
    cout << "Matched on " << matchedness*100 << "%" << endl;


    imshow("Lenna", lenna);
    imshow("Lenna-Average", avg);
    imshow("Lenna-Blurred", blurred);
    imshow("Lenna-Diff", diff);

    waitKey(0);
    destroyAllWindows();

    Mat gaussian;
    GaussianBlur(lenna, gaussian, Size(0,0), 1, 0);

    imshow("Lenna-Blurred", blurred);
    imshow("Gaussian", gaussian);
    cout << "Gaussian-blur matchedness: " << calculate_matched_ness(lenna, gaussian) << endl;

    Mat gaussian_blur_difference = gaussian - blurred;

    imshow("Gaussian Diff", gaussian_blur_difference);
    Mat log_difference;

    intensity_transform::logTransform(gaussian_blur_difference, log_difference);

    imshow("Log Gaussian Diff", log_difference);

    waitKey(0);
    destroyAllWindows();

    auto box_unsharped = box_unsharp(lenna);
    auto gaussian_unsharped = gaussian_unsharp(lenna);

    imshow("Box Unsharped", box_unsharped);
    imshow("Gaussian Unsharped", gaussian_unsharped);

    waitKey(0);
    destroyAllWindows();


//    imshow("Lenna-Laplace", laplace);

//    imshow("Lenna-Unsharped", unsharped);
//    waitKey(0);



    return 0;
}
