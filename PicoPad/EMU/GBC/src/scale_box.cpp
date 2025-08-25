#include "scale_box.h"
#include <string.h>

// lookup tables for RGB components
static uint8_t lut2a1b_r[32][32];
static uint8_t lut1a2b_r[32][32];
static uint8_t lut2a1b_g[64][64];
static uint8_t lut1a2b_g[64][64];
static uint8_t lut2a1b_b[32][32];
static uint8_t lut1a2b_b[32][32];

// gamma conversion tables
static uint8_t lut_in_r[32];
static uint8_t lut_out_r[256];
static uint8_t lut_in_g[64];
static uint8_t lut_out_g[256];
static uint8_t lut_in_b[32];
static uint8_t lut_out_b[256];

static void gamma_lut_init(void)
{
    static const uint8_t gamma5_to_lin8[32] = {
        0,0,1,1,3,5,7,10,13,17,21,26,32,38,44,52,
        60,68,77,87,97,108,120,132,145,159,173,188,204,220,237,255
    };

    static const uint8_t gamma6_to_lin8[64] = {
        0,0,0,0,1,1,1,2,3,4,4,5,7,8,9,11,
        13,14,16,18,20,23,25,28,31,33,36,40,43,46,50,54,
        57,61,66,70,74,79,84,89,94,99,105,110,116,122,128,134,
        140,147,153,160,167,174,182,189,197,205,213,221,229,238,246,255
    };

    static const uint8_t lin8_to_gamma5[256] = {
        0,2,3,4,5,5,6,6,6,7,7,7,8,8,8,9,
        9,9,9,10,10,10,10,10,11,11,11,11,11,12,12,12,
        12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,
        15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,
        17,17,17,17,17,17,17,17,17,18,18,18,18,18,18,18,
        18,18,19,19,19,19,19,19,19,19,19,19,20,20,20,20,
        20,20,20,20,20,20,20,21,21,21,21,21,21,21,21,21,
        21,21,21,22,22,22,22,22,22,22,22,22,22,22,23,23,
        23,23,23,23,23,23,23,23,23,23,23,24,24,24,24,24,
        24,24,24,24,24,24,24,24,25,25,25,25,25,25,25,25,
        25,25,25,25,25,25,26,26,26,26,26,26,26,26,26,26,
        26,26,26,26,26,27,27,27,27,27,27,27,27,27,27,27,
        27,27,27,27,28,28,28,28,28,28,28,28,28,28,28,28,
        28,28,28,28,29,29,29,29,29,29,29,29,29,29,29,29,
        29,29,29,29,29,30,30,30,30,30,30,30,30,30,30,30,
        30,30,30,30,30,30,30,31,31,31,31,31,31,31,31,31
    };

    static const uint8_t lin8_to_gamma6[256] = {
        0,5,7,8,10,11,11,12,13,14,14,15,16,16,17,17,
        18,18,19,19,20,20,21,21,22,22,22,23,23,23,24,24,
        25,25,25,26,26,26,27,27,27,27,28,28,28,29,29,29,
        29,30,30,30,31,31,31,31,32,32,32,32,33,33,33,33,
        34,34,34,34,35,35,35,35,35,36,36,36,36,37,37,37,
        37,37,38,38,38,38,38,39,39,39,39,39,40,40,40,40,
        40,41,41,41,41,41,42,42,42,42,42,42,43,43,43,43,
        43,44,44,44,44,44,44,45,45,45,45,45,45,46,46,46,
        46,46,46,47,47,47,47,47,47,48,48,48,48,48,48,48,
        49,49,49,49,49,49,49,50,50,50,50,50,50,51,51,51,
        51,51,51,51,52,52,52,52,52,52,52,53,53,53,53,53,
        53,53,54,54,54,54,54,54,54,54,55,55,55,55,55,55,
        55,56,56,56,56,56,56,56,56,57,57,57,57,57,57,57,
        57,58,58,58,58,58,58,58,58,59,59,59,59,59,59,59,
        59,60,60,60,60,60,60,60,60,60,61,61,61,61,61,61,
        61,61,62,62,62,62,62,62,62,62,62,63,63,63,63,63
    };

    memcpy(lut_in_r, gamma5_to_lin8, 32);
    memcpy(lut_in_b, gamma5_to_lin8, 32);
    memcpy(lut_in_g, gamma6_to_lin8, 64);
    memcpy(lut_out_r, lin8_to_gamma5, 256);
    memcpy(lut_out_b, lin8_to_gamma5, 256);
    memcpy(lut_out_g, lin8_to_gamma6, 256);
}

void scale_box_init(void)
{
    gamma_lut_init();

    for (int a = 0; a < 32; a++)
    {
        for (int b = 0; b < 32; b++)
        {
            uint8_t ar = lut_in_r[a];
            uint8_t br = lut_in_r[b];
            lut2a1b_r[a][b] = lut_out_r[(2 * ar + br) / 3];
            lut1a2b_r[a][b] = lut_out_r[(ar + 2 * br) / 3];

            uint8_t ab = lut_in_b[a];
            uint8_t bb = lut_in_b[b];
            lut2a1b_b[a][b] = lut_out_b[(2 * ab + bb) / 3];
            lut1a2b_b[a][b] = lut_out_b[(ab + 2 * bb) / 3];
        }
    }

    for (int a = 0; a < 64; a++)
    {
        for (int b = 0; b < 64; b++)
        {
            uint8_t ag = lut_in_g[a];
            uint8_t bg = lut_in_g[b];
            lut2a1b_g[a][b] = lut_out_g[(2 * ag + bg) / 3];
            lut1a2b_g[a][b] = lut_out_g[(ag + 2 * bg) / 3];
        }
    }
}

static inline uint16_t mix2a1b(uint16_t a, uint16_t b)
{
    uint16_t r = lut2a1b_r[(a >> 11) & 0x1F][(b >> 11) & 0x1F];
    uint16_t g = lut2a1b_g[(a >> 5) & 0x3F][(b >> 5) & 0x3F];
    uint16_t bl = lut2a1b_b[a & 0x1F][b & 0x1F];
    return (r << 11) | (g << 5) | bl;
}

static inline uint16_t mix1a2b(uint16_t a, uint16_t b)
{
    uint16_t r = lut1a2b_r[(a >> 11) & 0x1F][(b >> 11) & 0x1F];
    uint16_t g = lut1a2b_g[(a >> 5) & 0x3F][(b >> 5) & 0x3F];
    uint16_t bl = lut1a2b_b[a & 0x1F][b & 0x1F];
    return (r << 11) | (g << 5) | bl;
}

static void hscale_2to3(const uint16_t* src, uint16_t* dst)
{
    for (int x = 0; x < 160; x += 2)
    {
        uint16_t a = src[x];
        uint16_t b = src[x + 1];
        *dst++ = a;
        *dst++ = mix2a1b(a, b);
        *dst++ = mix1a2b(a, b);
    }
}

static void mix_line_2a1b(uint16_t* dst, const uint16_t* a, const uint16_t* b)
{
    for (int i = 0; i < 240; i++)
        dst[i] = mix2a1b(a[i], b[i]);
}

static void mix_line_1a2b(uint16_t* dst, const uint16_t* a, const uint16_t* b)
{
    for (int i = 0; i < 240; i++)
        dst[i] = mix1a2b(a[i], b[i]);
}

void scale_box_160x144_to_240x240(const uint16_t* src, uint16_t* dst)
{
    uint16_t linebuf[3][240];
    for (int sy = 0; sy < 144; sy += 3)
    {
        const uint16_t* s = src + sy * 160;
        hscale_2to3(s, linebuf[0]);
        hscale_2to3(s + 160, linebuf[1]);
        hscale_2to3(s + 320, linebuf[2]);

        memcpy(dst, linebuf[0], 240 * sizeof(uint16_t));
        dst += 240;
        mix_line_2a1b(dst, linebuf[0], linebuf[1]);
        dst += 240;
        memcpy(dst, linebuf[1], 240 * sizeof(uint16_t));
        dst += 240;
        mix_line_1a2b(dst, linebuf[1], linebuf[2]);
        dst += 240;
        memcpy(dst, linebuf[2], 240 * sizeof(uint16_t));
        dst += 240;
    }
}

