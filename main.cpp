#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/intensity_transform.hpp>
#include <opencv2/core/utility.hpp>


#include "filter.h"

using namespace cv;
using namespace std;


int main() {
    TickMeter tm;

    Mat lenna = imread("../Lenna.png");
    cv::resize(lenna, lenna, Size(1024, 1024));

    tm.start();
    auto avg = average(lenna, Size(3, 3));
    tm.stop();
    auto avg_ticks = tm.getTimeMilli();

    Mat blurred;
    tm.start();
    blur(lenna, blurred, Size(3, 3));
    tm.stop();
    auto blur_ticks = tm.getTimeMilli();

    cout << "Average millis: " << avg_ticks << ", Blur millis: " << blur_ticks << ". Difference: " << abs(blur_ticks - avg_ticks) << " milliseconds" << endl;

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

    auto laplace = laplacian(lenna, 0.05);

    imshow("Laplacian", laplace);


    waitKey(0);
    destroyAllWindows();

    auto laplace_unsharped = laplace_unsharp(lenna, .06, 1.2);

    imshow("Box Unsharped", box_unsharped);
    imshow("Gaussian Unsharped", gaussian_unsharped);
    imshow("Laplace Unsharped", laplace_unsharped);


    waitKey(0);
    destroyAllWindows();



    return 0;
}
