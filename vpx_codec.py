import ctypes
import cv2
import numpy as np


class VPXCodec:
    def __init__(self, lib_path, width, height):
        self.codec = ctypes.CDLL(lib_path)
        self.width = width
        self.height = height
        self.codec.init_vpx(width, height)

    def encode(self, yuv_bytes):
        self.codec.read_yuv_from_buf(yuv_bytes)
        self.codec.encode_frame()
        enc_size = self.codec.get_enc_data_size()
        enc_frame = bytes(enc_size)
        self.codec.write_enc_data_to_buf(enc_frame)
        return enc_frame

    def decode(self, enc_data):
        yuv_bytes = bytes(int(self.width * self.height * 3 / 2))
        self.codec.read_enc_data_from_buf(enc_data, len(enc_data))
        self.codec.decode_frame()
        self.codec.write_yuv_to_buf(yuv_bytes)
        return yuv_bytes

    def yuv_to_bgr(self, yuv_bytes):
        yuv_np = np.frombuffer(yuv_bytes, np.uint8)
        yuv_np = np.reshape(yuv_np, (int(self.height / 2 * 3), self.width))
        bgr_np = cv2.cvtColor(yuv_np, cv2.COLOR_YUV2BGR_I420)
        return bgr_np

    @staticmethod
    def bgr_to_yuv(bgr_np):
        yuv_np = cv2.cvtColor(bgr_np, cv2.COLOR_BGR2YUV_I420)
        yuv_bytes = yuv_np.tobytes()
        return yuv_bytes
