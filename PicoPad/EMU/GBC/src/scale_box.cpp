#include "scale_box.h"
#include <string.h>

// lookup tables for RGB components
static uint8_t lut2a1b_r[32][32];
static uint8_t lut1a2b_r[32][32];
static uint8_t lut2a1b_g[64][64];
static uint8_t lut1a2b_g[64][64];
static uint8_t lut2a1b_b[32][32];
static uint8_t lut1a2b_b[32][32];

void scale_box_init(void)
{
    for (int a = 0; a < 32; a++)
    {
        for (int b = 0; b < 32; b++)
        {
            lut2a1b_r[a][b] = (uint8_t)((2 * a + b) / 3);
            lut1a2b_r[a][b] = (uint8_t)((a + 2 * b) / 3);
            lut2a1b_b[a][b] = (uint8_t)((2 * a + b) / 3);
            lut1a2b_b[a][b] = (uint8_t)((a + 2 * b) / 3);
        }
    }
    for (int a = 0; a < 64; a++)
    {
        for (int b = 0; b < 64; b++)
        {
            lut2a1b_g[a][b] = (uint8_t)((2 * a + b) / 3);
            lut1a2b_g[a][b] = (uint8_t)((a + 2 * b) / 3);
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

