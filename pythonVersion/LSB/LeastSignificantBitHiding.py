from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from PIL import Image
import numpy as np
from collections import deque


def encrypt(message):
    key = get_random_bytes(16)
    cipher = AES.new(key, AES.MODE_EAX)
    cipher_text, tag = cipher.encrypt_and_digest(message)
    nonce = cipher.nonce
    return cipher_text, tag, nonce, key


def get_bit_array(number):
    result = [1 if digit == "1" else 0 for digit in bin(number)[2:]]
    result = deque(result)
    while len(result) != 8:
        result.appendleft(0)
    return list(result)


def hide(key, tag, nonce, message, image_path):
    i, j, k = 0, 0, 0
    message += b"xyzXYZ"
    image = Image.open(image_path)
    image_matrix = np.array(image)
    for part in [key, tag, nonce, message]:
        for element in part:
            bit_array = get_bit_array(element)
            for bit_value in bit_array:
                print(f"i: {i}, j: {j} and k: {k}")
                if bit_value == 0 and image_matrix[i, j, k] % 2 == 1:
                    image_matrix[i, j, k] += 1
                elif bit_value == 1 and image_matrix[i, j, k] % 2 == 0:
                    image_matrix[i, j, k] -= 1
                k += 1
                if k >= image_matrix.shape[2]:
                    k = 0
                    j += 1
                if j >= image_matrix.shape[1]:
                    j = 0
                    i += 1
    return Image.fromarray(image_matrix)


if __name__ == "__main__":
    cipher_text, tag, nonce, key = encrypt(b"hello")
    cipher = AES.new(key, AES.MODE_EAX, nonce)
    plaint_text = cipher.decrypt_and_verify(cipher_text, tag)
    print(plaint_text)
    print(
        f"length of key: {len(key)}, length of tag: {len(tag)}, length of nonce: {len(nonce)} and length of cipher text: {len(cipher_text)}"
    )
    result_image = hide(key, tag, nonce, cipher_text, "test.jpg")
    result_image.save("hasan.jpg")
