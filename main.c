#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "vpx/vpx_encoder.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

int width, height;
int64_t frame_cnt = 0;

vpx_codec_ctx_t codec_enc;
vpx_image_t img_enc;
const vpx_codec_cx_pkt_t *pkt = NULL;

int dec_size = 0;
unsigned char *dec_buffer;
vpx_codec_ctx_t codec_dec;
vpx_image_t *img_dec;


int vpx_img_plane_width(const vpx_image_t *img, int plane) {
    if (plane > 0 && img->x_chroma_shift > 0)
        return (img->d_w + 1) >> img->x_chroma_shift;
    else
        return img->d_w;
}

int vpx_img_plane_height(const vpx_image_t *img, int plane) {
    if (plane > 0 && img->y_chroma_shift > 0)
        return (img->d_h + 1) >> img->y_chroma_shift;
    else
        return img->d_h;
}

void read_yuv_from_buf(unsigned char* img_buffer) {
    int plane, i;
    for (plane = 0; plane < 3; ++plane) {
        unsigned char *buf = img_enc.planes[plane];
        const int stride = img_enc.stride[plane];
        const int w = vpx_img_plane_width(&img_enc, plane) * ((img_enc.fmt & VPX_IMG_FMT_HIGHBITDEPTH) ? 2 : 1);
        const int h = vpx_img_plane_height(&img_enc, plane);
        for (i = 0; i < h; ++i) {
          memcpy(buf, img_buffer, w);
          buf += stride;
          img_buffer += w;
        }
    }
}

void read_enc_data_from_buf(unsigned char* enc_buffer, int enc_size) {
    dec_size = enc_size;
    memcpy(dec_buffer, enc_buffer, enc_size);
}

void write_yuv_to_buf(unsigned char* img_buffer) {
    int plane, i;
    for (plane = 0; plane < 3; ++plane) {
        const unsigned char *buf = img_dec->planes[plane];
        const int stride = img_dec->stride[plane];
        const int w = vpx_img_plane_width(img_dec, plane) * ((img_dec->fmt & VPX_IMG_FMT_HIGHBITDEPTH) ? 2 : 1);
        const int h = vpx_img_plane_height(img_dec, plane);
        for (i = 0; i < h; ++i) {
          memcpy(img_buffer, buf, w);
          buf += stride;
          img_buffer += w;
        }
    }
}

void write_enc_data_to_buf(unsigned char* enc_buffer) {
    memcpy(enc_buffer, pkt->data.frame.buf, (unsigned int)pkt->data.frame.sz);
}

unsigned int get_enc_data_size() {
    unsigned int enc_data_size = (unsigned int)pkt->data.frame.sz;
    return enc_data_size;
}

void read_yuv_from_file(FILE *file) {
    int plane, i;
    for (plane = 0; plane < 3; ++plane) {
        unsigned char *buf = img_enc.planes[plane];
        const int stride = img_enc.stride[plane];
        const int w = vpx_img_plane_width(&img_enc, plane) * ((img_enc.fmt & VPX_IMG_FMT_HIGHBITDEPTH) ? 2 : 1);
        const int h = vpx_img_plane_height(&img_enc, plane);
        for (i = 0; i < h; ++i) {
          fread(buf, 1, w, file);
          buf += stride;
        }
    }
}

void write_yuv_to_file(FILE *file) {
    int plane, i;
    for (plane = 0; plane < 3; ++plane) {
        const unsigned char *buf = img_dec->planes[plane];
        const int stride = img_dec->stride[plane];
        const int w = vpx_img_plane_width(img_dec, plane) * ((img_dec->fmt & VPX_IMG_FMT_HIGHBITDEPTH) ? 2 : 1);
        const int h = vpx_img_plane_height(img_dec, plane);
        for (i = 0; i < h; ++i) {
          fwrite(buf, 1, w, file);
          buf += stride;
        }
    }
}

void write_yuv_test() {
    FILE *file = fopen("decode.yuv", "wb");
    int plane, i;
    for (plane = 0; plane < 3; ++plane) {
        const unsigned char *buf = img_dec->planes[plane];
        const int stride = img_dec->stride[plane];
        const int w = vpx_img_plane_width(img_dec, plane) * ((img_dec->fmt & VPX_IMG_FMT_HIGHBITDEPTH) ? 2 : 1);
        const int h = vpx_img_plane_height(img_dec, plane);
        for (i = 0; i < h; ++i) {
          fwrite(buf, 1, w, file);
          buf += stride;
        }
    }
    fclose(file);
}

void init_vpx(int w, int h)
{
    width = w;
    height = h;

    vpx_img_alloc(&img_enc, VPX_IMG_FMT_I420, width, height, 1);

    vpx_codec_enc_cfg_t cfg_enc;
    vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg_enc, 0);
    cfg_enc.rc_target_bitrate = 800;
    cfg_enc.g_w = width;
    cfg_enc.g_h = height;

    vpx_codec_enc_init(&codec_enc, vpx_codec_vp8_cx(), &cfg_enc, 0);
    vpx_codec_dec_init(&codec_dec, vpx_codec_vp8_dx(), NULL, 0);

    dec_buffer = (unsigned char *) malloc(3840 * 2160 * 4);
}

void encode_frame()
{
    vpx_codec_iter_t iter = NULL;

    vpx_codec_encode(&codec_enc, &img_enc, frame_cnt, 1, 0, VPX_DL_REALTIME);
    pkt = vpx_codec_get_cx_data(&codec_enc, &iter);

    frame_cnt = frame_cnt + 1;
}

void decode_frame()
{
    vpx_codec_iter_t iter = NULL;
    vpx_codec_decode(&codec_dec, dec_buffer, dec_size, NULL, 0);
    img_dec = vpx_codec_get_frame(&codec_dec, &iter);
}