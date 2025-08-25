#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// initialize lookup tables (includes gamma correction)
void scale_box_init(void);
void scale_box_160x144_to_240x240(const uint16_t* src, uint16_t* dst);

#ifdef __cplusplus
}
#endif

