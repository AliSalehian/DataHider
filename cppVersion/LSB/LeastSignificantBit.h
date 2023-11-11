//
// Created by freakydad on 10/21/23.
//

#ifndef CPPVERSION_LEASTSIGNIFICANTBIT_H
#define CPPVERSION_LEASTSIGNIFICANTBIT_H

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/err.h>


namespace LSB {
    std::string reveal(const std::string& imagePath);
    void handleErrors();
    std::string decryptAESGCM(const std::string& ciphertext, const std::string& key, const std::string& iv, const std::string& tag);
}


#endif //CPPVERSION_LEASTSIGNIFICANTBIT_H
