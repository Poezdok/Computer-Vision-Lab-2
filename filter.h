//
// Created by alex on 12.12.2021.
//

#ifndef LAB_2_FILTER_H
#define LAB_2_FILTER_H

#include "opencv2/core.hpp"

using namespace  cv;

void filter(Mat source, int);

Mat average(Mat source, Size mask_size = Size(3, 3));

Mat laplacian(Mat source, double coefficient = 0.1);

Mat box_unsharp(Mat source, Size box_size = Size(3, 3), double coefficient = 1);
Mat gaussian_unsharp(Mat source, Size box_size = Size(0, 0), double sigmaX = 5, double coefficient = 1);

double calculate_matched_ness(Mat a, Mat b);


#endif //LAB_2_FILTER_H
