//
// Created by alex on 12.12.2021.
//

#ifndef LAB_2_FILTER_H
#define LAB_2_FILTER_H

#include "opencv2/core.hpp"

using namespace  cv;

void filter(Mat source, int);

Mat average(Mat source, Size mask_size = Size(3, 3));
void add_mask(Mat source, Mat mask);
Mat laplacian(Mat source, double coefficient = 0.1);
Mat unsharp_masking(Mat source, Size mask_size = Size(3, 3), double coefficient=1);

#endif //LAB_2_FILTER_H
