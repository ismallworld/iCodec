import cv2
from vpx_codec import VPXCodec


def test_yuv():
    vpx_codec = VPXCodec('./iCodec.so', 640, 360)
    f_in = open("cuc_ieschool_640x360_yuv420p.yuv", "rb")
    yuv_bytes = f_in.read(int(640 * 360 * 3 / 2))
    encode_data = vpx_codec.encode(yuv_bytes)
    out_data = vpx_codec.decode(encode_data)
    bgr_np = vpx_codec.yuv_to_bgr(out_data)
    cv2.imwrite('./cuc_dec.png', bgr_np)


def test_numpy():
    vpx_codec = VPXCodec('./iCodec.so', 3840, 2160)
    bgr_np = cv2.imread("./test.jpg")
    yuv_bytes = vpx_codec.bgr_to_yuv(bgr_np)
    encode_data = vpx_codec.encode(yuv_bytes)
    out_data = vpx_codec.decode(encode_data)
    bgr_np = vpx_codec.yuv_to_bgr(out_data)
    cv2.imwrite('./test_dec.png', bgr_np)


test_yuv()
test_numpy()
