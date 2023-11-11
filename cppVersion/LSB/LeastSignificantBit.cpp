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

    // region decrypt
    cipherText.resize(cipherText.size() - 6);
    std::string cipherTextString(cipherText.begin(), cipherText.end());
    std::string keyString(key.begin(), key.end());
    std::string nonceString(nonce.begin(), nonce.end());
    std::string tagString(tag.begin(), tag.end());
    return decryptAESGCM(cipherTextString, keyString, nonceString, tagString);
    // endregion
}

void LSB::handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

std::string LSB::decryptAESGCM(const std::string& ciphertext, const std::string& key, const std::string& iv, const std::string& tag) {
    OpenSSL_add_all_algorithms();

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL) != 1) {
        handleErrors();
    }

    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(iv.size()), NULL) != 1) {
        handleErrors();
    }

    if (EVP_DecryptInit_ex(ctx, NULL, NULL, reinterpret_cast<const unsigned char*>(key.c_str()), reinterpret_cast<const unsigned char*>(iv.c_str())) != 1) {
        handleErrors();
    }

    if (EVP_DecryptUpdate(ctx, NULL, NULL, reinterpret_cast<const unsigned char*>(tag.c_str()), tag.size()) != 1) {
//        handleErrors();
    }

    std::string plaintext;
    plaintext.resize(ciphertext.size());

    int len;
    if (EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(&plaintext[0]), &len, reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size()) != 1) {
//        handleErrors();
    }

    int plaintext_len = len;

    if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(&plaintext[0] + len), &len) != 1) {
//        handleErrors();
    }

    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    // Resize the plaintext to its actual length
    plaintext.resize(plaintext_len);

    return plaintext;
}