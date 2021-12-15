//
// Created by alex on 12.12.2021.
//

#ifndef LAB_2_FILTER_H
#define LAB_2_FILTER_H

#include <opencv2/core.hpp>


cv::Mat average(const cv::Mat& source, cv::Size mask_size = cv::Size(3, 3));
cv::Mat laplacian(const cv::Mat& source, double coefficient = 0.05);

cv::Mat box_unsharp(const cv::Mat& source, cv::Size box_size = cv::Size(3, 3), double coefficient = 1);
cv::Mat gaussian_unsharp(const cv::Mat& source, cv::Size box_size = cv::Size(0, 0), double sigmaX = 5, double coefficient = 1);
cv::Mat laplace_unsharp(const cv::Mat& source, double coefficient_laplace = 0.05, double coefficient_unsharp = 1);

double calculate_matched_ness(cv::Mat a, cv::Mat b);


#endif //LAB_2_FILTER_H
