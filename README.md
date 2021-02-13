# iCodec
This is python bindings of Google vp8 codec. Developers can use this repository to simplify video codec development.  
<img src="https://ismallworld.github.io/ismallworld/iCode/iCodec/images/framework_small.png">  

**Note**: **vp8** is default used in this repository, you can change the code to use **vp9** in your application.

## Example
In this example, we just read one image and use this image to show encode and decode process, 
you can also read video frames in your application. If more frames are sent to the encoder, inter prediction will be automatically enabled. To run encoder and decoder with vp8:
```python
import cv2
from vpx_codec import VPXCodec


# init VPXCodec
vpx_codec = VPXCodec('./iCodec.so', 1920, 1080)

# read image as numpy format
bgr_np = cv2.imread("./test.png")

# change to yuv format
yuv_bytes = vpx_codec.bgr_to_yuv(bgr_np)

# encode this frame 
# if more frames are sent to the encoder, inter prediction will be automatically enabled
encode_data = vpx_codec.encode(yuv_bytes)

# decode this frame, be sure to use encode_data in order
out_data = vpx_codec.decode(encode_data)

# change to numpy format
bgr_np = vpx_codec.yuv_to_bgr(out_data)

# save decoded image
cv2.imwrite('./test_dec.png', bgr_np)
```
See **main.py** and **vpx_codec.py** for more information.

## Dependencies
```python
pip install opencv-contrib-python
pip install numpy
```

## For professional users
If you need to change vpx codec, you need to compile the vpx source code to get **iCodec.so**. Just reference **build_vpx.sh**
and **build_lib.sh**. Be sure **-fPIC** is added when compile the source files.