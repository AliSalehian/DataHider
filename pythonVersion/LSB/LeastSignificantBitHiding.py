from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from PIL import Image
import numpy as np
from collections import deque


# TODO: add type hint and document
def encrypt(message):
    key = get_random_bytes(16)
    cipher = AES.new(key, AES.MODE_GCM)
    cipher.update(b"freakydad")
    cipher_text, tag = cipher.encrypt_and_digest(message)
    nonce = cipher.nonce
    return cipher_text, tag, nonce, key


# TODO: add type hint and document
def get_bit_array(number):
    result = [1 if digit == "1" else 0 for digit in bin(number)[2:]]
    result = deque(result)
    while len(result) != 8:
        result.appendleft(0)
    return list(result)


# TODO: add type hint and document
def hide_process(key, tag, nonce, message, image_path):
    i, j, k = 0, 0, 0
    message += b"xyzXYZ"
    image = Image.open(image_path)
    image_matrix = np.array(image)
    for part in [key, tag, nonce, message]:
        for element in part:
            bit_array = get_bit_array(element)
            for bit_value in bit_array:
                # TODO: put it in log
                # print(f"i: {i}, j: {j} and k: {k}")
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


def hide(source_path, message, result_name):
    cipher_text, tag, nonce, key = encrypt(message.encode("utf-8"))
    result_image = hide_process(key, tag, nonce, cipher_text, source_path)
    result_image.save(f"{result_name}.png")


if __name__ == "__main__":
    hide("test.jpg", "hello", "hasan")
    # cipher_text, tag, nonce, key = encrypt(b"hello")
    # result_image = hide_process(key, tag, nonce, cipher_text, "test.jpg")
    # result_image.save("hasan.png")
