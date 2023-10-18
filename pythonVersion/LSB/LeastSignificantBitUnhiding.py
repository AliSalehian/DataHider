from Crypto.Cipher import AES
from PIL import Image
import numpy as np


# TODO: add type hint and document
def derive_information(image_path):
    image = Image.open(image_path)
    image_matrix = np.array(image)
    i, j, k = 0, 0, 0
    key, tag, nonce, cipher_text = None, None, None, None
    temp = []
    last_char_in_pattern = ""
    fetching_cipher_text = False
    while True:
        temp_byte = []
        while len(temp_byte) != 8:
            if image_matrix[i, j, k] % 2 == 0:
                temp_byte.append(0)
            else:
                temp_byte.append(1)
            k += 1
            if k >= image_matrix.shape[2]:
                k = 0
                j += 1
            if j >= image_matrix.shape[1]:
                j = 0
                i += 1
        decimal = 0
        for bit in temp_byte:
            decimal = (decimal << 1) | bit
        temp.append(decimal)
        if len(temp) == 16 and not fetching_cipher_text:
            if key is None:
                key = bytes(temp)
            elif tag is None:
                tag = bytes(temp)
            elif nonce is None:
                nonce = bytes(temp)
                fetching_cipher_text = True
            temp.clear()
        if fetching_cipher_text:
            if last_char_in_pattern == "":
                if decimal == 120:
                    last_char_in_pattern = "x"
                    continue
            elif last_char_in_pattern == "x":
                if decimal == 121:
                    last_char_in_pattern = "y"
                    continue
                else:
                    last_char_in_pattern = ""
            elif last_char_in_pattern == "y":
                if decimal == 122:
                    last_char_in_pattern = "z"
                    continue
                else:
                    last_char_in_pattern = ""
            elif last_char_in_pattern == "z":
                if decimal == 88:
                    last_char_in_pattern = "X"
                    continue
                else:
                    last_char_in_pattern = ""
            elif last_char_in_pattern == "X":
                if decimal == 89:
                    last_char_in_pattern = "Y"
                    continue
                else:
                    last_char_in_pattern = ""
            elif last_char_in_pattern == "Y":
                if decimal == 90:
                    cipher_text = bytes(temp)
                    break
    # TODO: put it in log
    # print(
    #     f"key: [{key}], tag: [{tag}], nonce: [{nonce}] and cipherText: [{cipher_text}]"
    # )
    return key, tag, nonce, cipher_text


# TODO: add type hint and document
def decrypt(key, tag, nonce, cipher_text):
    cipher = AES.new(key, AES.MODE_EAX, nonce)
    plaint_text = cipher.decrypt_and_verify(cipher_text, tag)
    return plaint_text


def reveal(source_path):
    key, tag, nonce, cipher_text = derive_information(source_path)
    return decrypt(key, tag, nonce, cipher_text[:-6])


if __name__ == "__main__":
    key, tag, nonce, cipher_text = derive_information("hasan.png")
    print(decrypt(key, tag, nonce, cipher_text[:-6]))
