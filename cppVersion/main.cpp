#include "LSB/LeastSignificantBit.h"

int main() {
    LSB::reveal("../mahnaz.png");
    return 0;
//    cv::Mat image = cv::imread("../mahnaz.png");
//    int counter = 0;
//    for (int i=0; i < image.rows; i++){
//        for (int j=0; j < image.cols; j++){
//            std::cout << (int)image.at<cv::Vec3b>(i, j)[0] << " " << (int)image.at<cv::Vec3b>(i, j)[1] <<
//                    " " << (int)image.at<cv::Vec3b>(i, j)[2];
//            std::cout << " : " << image.at<cv::Vec3b>(i, j)[0] << " " << image.at<cv::Vec3b>(i, j)[1] <<
//                      " " << image.at<cv::Vec3b>(i, j)[2] << std::endl;
//            counter += 3;
//            if (counter == 102) break;
//        }
//        if (counter == 102) break;
//    }
//    std::cout << "---------------------------------" << std::endl;
//    uchar* p;
//    counter = 0;
//    for (int i=0; i < image.rows; i++){
//        p = image.ptr<uchar>(i);
//        for (int j=0; j < image.cols; j++){
//            std::cout << (int)p[j] << std::endl;
//            counter++;
//            if (counter == 102) break;
//        }
//        if (counter == 102) break;
//    }
//    delete p;
}
