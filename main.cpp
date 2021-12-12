#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


#include "filter.h"

using namespace cv;
using namespace std;


int main() {

    Mat lenna = imread("../Lenna.png");

    imshow("Lenna", lenna);
//    waitKey(0);
    filter(lenna, 10);
    imshow("Lenna-Filtered", lenna);
    waitKey(0);



    return 0;
}
