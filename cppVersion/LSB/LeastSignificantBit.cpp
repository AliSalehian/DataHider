//
// Created by freakydad on 10/21/23.
//

#include "LeastSignificantBit.h"

std::string LSB::reveal(const std::string &imagePath) {
    cv::Mat image = cv::imread(imagePath);
    std::vector<uchar> tag, key, nonce, cipherText;
    uchar byte = 0;
    int bitCounter = 0, byteCounter = 0, lastCipherByte = 0;
    bool tagCompleted = false, keyCompleted = false, nonceCompleted = false, cipherTextCompleted = false;

    // region Retrieve key, tag, nonce and cipherText
    for (int i=0; i < image.rows; i++){
        if (cipherTextCompleted) break;
        for (int j=0; j < image.cols; j++){
            if (cipherTextCompleted) break;
            for (int k = 2; k != -1; k--) {
                if (cipherTextCompleted) break;
                if ((int) image.at<cv::Vec3b>(i, j)[k] % 2 == 1) byte |= 1 << (7 - bitCounter);
                if (++bitCounter == 8) {
                    if (!keyCompleted) key.emplace_back(byte);
                    else if (!tagCompleted) tag.emplace_back(byte);
                    else if (!nonceCompleted) nonce.emplace_back(byte);
                    else cipherText.emplace_back(byte);
                    if (nonceCompleted){
                        if (lastCipherByte == 0 && (int)byte == 120) lastCipherByte = 120;
                        else if (lastCipherByte == 120){
                            if ((int)byte == 121) lastCipherByte = 121;
                            else lastCipherByte = 0;
                        } else if (lastCipherByte == 121){
                            if ((int)byte == 122) lastCipherByte = 122;
                            else lastCipherByte = 0;
                        } else if (lastCipherByte == 122){
                            if ((int)byte == 88) lastCipherByte = 88;
                            else lastCipherByte = 0;
                        } else if (lastCipherByte == 88){
                            if ((int)byte == 89) lastCipherByte = 89;
                            else lastCipherByte = 0;
                        } else if (lastCipherByte == 89){
                            if ((int)byte == 90) cipherTextCompleted = true;
                            else lastCipherByte = 0;
                        }
                    }
                    byte = bitCounter = 0;
                    byteCounter++;
                    if (byteCounter == 16) {
                        if (!keyCompleted) keyCompleted = true;
                        else if (!tagCompleted) tagCompleted = true;
                        else if (!nonceCompleted) nonceCompleted = true;
                        byteCounter = 0;
                    }
                }
            }
        }
    }
    // endregion
    cipherText.resize(cipherText.size() - 6);
    std::string cipherTextString1(cipherText.begin(), cipherText.end());
    std::cout << cipherTextString1 << std::endl;
}