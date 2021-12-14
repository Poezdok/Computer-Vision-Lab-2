#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"



#include "filter.h"

using namespace cv;
using namespace std;


int main() {

    Mat lenna = imread("../Lenna.png");
    cv::resize(lenna, lenna, Size(1024, 1024));

//    return 0;
    auto avg = average(lenna);
    Mat blurred;
    blur(lenna, blurred, Size(3, 3));

    auto diff = blurred - avg;

    auto matchedness = calculate_matched_ness(blurred, avg);
    cout << "Mathced on " << matchedness*100 << "%" << endl;

//    auto laplace = laplacian(lenna, 1);
//    auto unsharped = unsharp_masking(lenna, Size(5,5), 2);

    imshow("Lenna", lenna);
    imshow("Lenna-Average", avg);
    imshow("Lenna-Blurred", blurred);
    imshow("Lenna-Diff", diff);




//    imshow("Lenna-Laplace", laplace);

//    imshow("Lenna-Unsharped", unsharped);
    waitKey(0);



    return 0;
}
