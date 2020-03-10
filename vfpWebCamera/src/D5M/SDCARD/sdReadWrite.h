#ifndef SRC_D5M_SDCARD_SDREADWRITE_H_
#define SRC_D5M_SDCARD_SDREADWRITE_H_

#include <xbasic_types.h>

int wr_data_sd();
int rd_data_sd();
int wr_fr_data();
int rd_wr_data_sd();
int write_raw_frame_data(int height, int width);
int write_hd_data_sd();

typedef struct rgb_color {
    u8 r;
    u8 g;
    u8 b;
} rgb_color_reg;

typedef struct ycbcr_color {
    u16 y;
    u16 yS;
    u16 u;
    u16 v;
} ycbcr_color_reg;

ycbcr_to_rgb(rgb_color_reg *rgb_color_ptr, u32 adrSof, int cb);
ycbcr_to_rgb_print(rgb_color_reg *rgb_color_ptr, u32 adrSof, int cb);

int wr_fr_data();
int write_image_frame();

#endif /* SRC_D5M_SDCARD_SDREADWRITE_H_ */
