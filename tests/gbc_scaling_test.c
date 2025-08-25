#include <assert.h>
#include <stdio.h>

#define LCD_WIDTH 160
#define LCD_HEIGHT 144
#define WIDTH 240
#define HEIGHT 240

int main(void) {
    unsigned char lut_x[WIDTH];
    unsigned char lut_y[HEIGHT];

    int err = 0;
    int src = 0;
    for (int dst = 0; dst < WIDTH; dst++) {
        lut_x[dst] = src;
        err += LCD_WIDTH;
        if (err >= WIDTH) {
            err -= WIDTH;
            src++;
        }
    }

    err = HEIGHT - LCD_HEIGHT;
    src = 0;
    for (int dst = 0; dst < HEIGHT; dst++) {
        lut_y[dst] = src;
        err += LCD_HEIGHT;
        if (err >= HEIGHT) {
            err -= HEIGHT;
            src++;
        }
    }

    unsigned char expected_x[] = {0, 0, 1};
    for (int i = 0; i < 3; i++) {
        assert(lut_x[i] == expected_x[i]);
    }

    unsigned char expected_y[] = {0, 1, 1, 2, 2};
    for (int i = 0; i < 5; i++) {
        assert(lut_y[i] == expected_y[i]);
    }

    assert(lut_x[WIDTH - 1] == LCD_WIDTH - 1);
    assert(lut_y[HEIGHT - 1] == LCD_HEIGHT - 1);

    printf("OK\n");
    return 0;
}
