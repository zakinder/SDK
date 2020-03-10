#include <ff.h>
#include <integer.h>
#include <stdio.h>
#include <stdlib.h>
#include <xbasic_types.h>
#include <xil_io.h>
#include <xstatus.h>

#include "../UART/uartio.h"
#include "sdReadWrite.h"

#define MAX_STRING_LENGTH 80
#define PIXEL_LENGTH 7

static FIL iFil;
static FIL oFil;
static FATFS fatfs;
static char *SD_File;
static char *SD2File;

FRESULT res;
u32 adrSof;
u32 adrEof;
u8 nAddre;
u8 n4Addre;
u8 n8Addre;
u16 nPattn;
u16 nRgb;
u32 x;
int y;
int row;
int col;
int i;
int j;
int pix;
int len = 0;
UINT bw; // bytes written
TCHAR *Pth = "0:/";
ycbcr_color_reg ycbcr_color_ptr;
rgb_color_reg rgb_color_ptr;

int write_image_frame() {
    adrSof = 0x2008700; //skip 1st line
    nAddre = 0x02;
    row = 1071;
    col = 1920;

    int offset;
    int width;
    int height;
    unsigned char *response;
    unsigned char buff[PIXEL_LENGTH] = { 0 };
    unsigned char *buffer;
    char iFilName[MAX_STRING_LENGTH] = "RGB.BMP";
    SD_File = (char *) iFilName;
    res = f_mount(&fatfs, Pth, 0);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_open(&iFil, SD_File, FA_READ);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_lseek(&iFil, 10);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_read(&iFil, &offset, 4, &bw);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_lseek(&iFil, 18);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_read(&iFil, &width, 4, &bw);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_read(&iFil, &height, 4, &bw);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    printf("width  = %d\n", width);
    printf("height = %d\n", height);
    res = f_lseek(&iFil, 0);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    printf("FR_OK : 1\n");
    buffer = (unsigned char *) malloc(offset);
    if (buffer == NULL) {
        perror("malloc");
        exit(-1);
    }
    res = f_read(&iFil, buffer, offset, &bw);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    printf("FR_OK : 2\n");
    printf("Enter file name with extension\n");
    menu_print_prompt();
    char FileName[MAX_STRING_LENGTH] = { 0 };
    char_to_uart(FileName);
    SD2File = (char *) FileName;
    res = f_open(&oFil, SD2File, FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_write(&oFil, buffer, offset, &bw);
    if (res != FR_OK) {
        printf("error writing header!\n");
        return XST_FAILURE;
    }
    int cb = 0;
    int mod = width % 4;
    if (mod != 0) {
        mod = 4 - mod;
    }
    printf("mod = %d\n", mod);
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            cb++;
            if (cb == 2) {
                cb = 0;
            }
            ycbcr_to_rgb(&rgb_color_ptr, adrSof, cb);
            len = snprintf((void*) buff, sizeof(buff), "%d ", rgb_color_ptr.r);
            unsigned char *responsed1 = (void*) buff;
            res = f_write(&oFil, responsed1, sizeof(char), &bw);
            if (res != FR_OK) {
                printf("error writing header!\n");
                return XST_FAILURE;
            }
            len = snprintf((void*) buff, sizeof(buff), "%d ", rgb_color_ptr.g);
            unsigned char *responsed2 = (void*) buff;
            res = f_write(&oFil, responsed2, sizeof(char), &bw);
            if (res != FR_OK) {
                printf("error writing header!\n");
                return XST_FAILURE;
            }
            len = snprintf((void*) buff, sizeof(buff), "%d ", rgb_color_ptr.b);
            unsigned char *responsed3 = (void*) buff;
            response = (void*) buff;
            res = f_write(&oFil, responsed3, sizeof(char), &bw);
            if (res != FR_OK) {
                printf("error writing header!\n");
                return XST_FAILURE;
            }
            adrSof = adrSof + nAddre;
        }
        for (j = 0; j < mod; j++) {
            res = f_write(&oFil, response, sizeof(char), &bw);
            if (res != FR_OK) {
                printf("error writing header!\n");
                return XST_FAILURE;
            }
        }
    }
    res = f_close(&oFil);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_close(&iFil);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    free(buffer);
    return XST_SUCCESS;
}
int wr_fr_data() {
    adrSof = 0x2008700; //skip 1st line
    nAddre = 0x02;
    row = 1071;
    col = 1920;
    row = 0;
    col = 0;

    int offset;
    int width;
    int height;
    unsigned char *response;
    unsigned char buff[PIXEL_LENGTH] = { 0 };
    unsigned char *buffer;
    printf("Enter num of rows to be saved[1071]\n");
    menu_print_prompt();
    row = uart_prompt_io();
    printf("Enter num of rows to be saved[1920]\n");
    menu_print_prompt();
    col = uart_prompt_io();
    char iFilName[MAX_STRING_LENGTH] = "RGB.BMP";
    SD_File = (char *) iFilName;
    res = f_mount(&fatfs, Pth, 0);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_open(&iFil, SD_File, FA_READ);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_lseek(&iFil, 10);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_read(&iFil, &offset, 4, &bw);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_lseek(&iFil, 18);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_read(&iFil, &width, 4, &bw);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_read(&iFil, &height, 4, &bw);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    printf("width  = %d\n", width);
    printf("height = %d\n", height);
    res = f_lseek(&iFil, 0);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    printf("FR_OK : 1\n");
    buffer = (unsigned char *) malloc(offset);
    if (buffer == NULL) {
        perror("malloc");
        exit(-1);
    }
    res = f_read(&iFil, buffer, offset, &bw);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    printf("FR_OK : 2\n");
    printf("Enter file name with extension\n");
    menu_print_prompt();
    char FileName[MAX_STRING_LENGTH] = { 0 };
    char_to_uart(FileName);
    SD2File = (char *) FileName;
    res = f_open(&oFil, SD2File, FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_write(&oFil, buffer, offset, &bw);
    if (res != FR_OK) {
        printf("error writing header!\n");
        return XST_FAILURE;
    }
    int mod = col % 4;
    int cb = 0;
    if (mod != 0) {
        mod = 4 - mod;
    }
    printf("mod = %d\n", mod);
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            cb++;
            if (cb == 2) {
                cb = 0;
            }
            ycbcr_to_rgb_print(&rgb_color_ptr, adrSof, cb);
            len = snprintf((void*) buff, sizeof(buff), "%d ", rgb_color_ptr.r);
            unsigned char *responsed1 = (void*) buff;
            res = f_write(&oFil, responsed1, sizeof(char), &bw);
            if (res != FR_OK) {
                printf("error writing header!\n");
                return XST_FAILURE;
            }
            len = snprintf((void*) buff, sizeof(buff), "%d ", rgb_color_ptr.g);
            unsigned char *responsed2 = (void*) buff;
            res = f_write(&oFil, responsed2, sizeof(char), &bw);
            if (res != FR_OK) {
                printf("error writing header!\n");
                return XST_FAILURE;
            }
            len = snprintf((void*) buff, sizeof(buff), "%d ", rgb_color_ptr.b);
            unsigned char *responsed3 = (void*) buff;
            response = (void*) buff;
            res = f_write(&oFil, responsed3, sizeof(char), &bw);
            if (res != FR_OK) {
                printf("error writing header!\n");
                return XST_FAILURE;
            }
            adrSof = adrSof + nAddre;
        }
        for (j = 0; j < mod; j++) {
            res = f_write(&oFil, response, sizeof(char), &bw);
            if (res != FR_OK) {
                printf("error writing header!\n");
                return XST_FAILURE;
            }
        }
    }
    res = f_close(&oFil);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    res = f_close(&iFil);
    if (res != FR_OK) {
        return XST_FAILURE;
    }
    free(buffer);
    return XST_SUCCESS;
}
ycbcr_to_rgb(rgb_color_reg *rgb_color_ptr, u32 adrSof, int cb) {
    ycbcr_color_reg *ycbcr_color_ptr;
    nAddre = 0x2;
    ycbcr_color_ptr->yS = ((Xil_In16(adrSof) & 0x00ff));
    if (cb == 0) {
        ycbcr_color_ptr->u = ((Xil_In16(adrSof) & 0xff00) >> 8);
    }
    if (cb == 1) {
        ycbcr_color_ptr->v = ((Xil_In16(adrSof) & 0xff00) >> 8);
    }
    ycbcr_color_ptr->y = (((Xil_In16(adrSof + nAddre) & 0x00ff))
            + (ycbcr_color_ptr->yS)) / 2;
    //rgb_color_ptr->r = (unsigned)(ycbcr_color_ptr->yS);
    //rgb_color_ptr->g = (unsigned)(ycbcr_color_ptr->u);
    //rgb_color_ptr->b = (unsigned)(ycbcr_color_ptr->v);
    rgb_color_ptr->b = (unsigned) (ycbcr_color_ptr->y
            + 1.4075 * (ycbcr_color_ptr->v - 128));
    rgb_color_ptr->g = (unsigned) (ycbcr_color_ptr->y
            - 0.3455 * (ycbcr_color_ptr->u - 128)
            - (0.7169 * (ycbcr_color_ptr->v - 128)));
    rgb_color_ptr->r = (unsigned) (ycbcr_color_ptr->y
            + 1.7790 * (ycbcr_color_ptr->u - 128));
}
ycbcr_to_rgb_print(rgb_color_reg *rgb_color_ptr, u32 adrSof, int cb) {
    ycbcr_color_reg *ycbcr_color_ptr;
    nAddre = 0x2;
    n4Addre = 0x4;
    n8Addre = 0x8;
    ycbcr_color_ptr->yS = ((Xil_In16(adrSof) & 0x00ff));
    if (cb == 0) {
        ycbcr_color_ptr->u = ((Xil_In16(adrSof) & 0xff00) >> 8);
    }
    if (cb == 1) {
        ycbcr_color_ptr->v = ((Xil_In16(adrSof) & 0xff00) >> 8);
    }
    ycbcr_color_ptr->y = (((Xil_In16(adrSof + nAddre) & 0x00ff))
            + (ycbcr_color_ptr->yS)) / 2;
    //rgb_color_ptr->r = (unsigned)(ycbcr_color_ptr->y + 1.4075 * ycbcr_color_ptr->v);
    //rgb_color_ptr->g = (unsigned)(ycbcr_color_ptr->y - 0.395 * ycbcr_color_ptr->u - 0.581 * ycbcr_color_ptr->v);
    //rgb_color_ptr->b = (unsigned)(ycbcr_color_ptr->y + 2.032 * ycbcr_color_ptr->u);
    rgb_color_ptr->b = (unsigned) (ycbcr_color_ptr->y
            + 1.4075 * (ycbcr_color_ptr->v - 128));
    rgb_color_ptr->g = (unsigned) (ycbcr_color_ptr->y
            - 0.3455 * (ycbcr_color_ptr->u - 128)
            - (0.7169 * (ycbcr_color_ptr->v - 128)));
    rgb_color_ptr->r = (unsigned) (ycbcr_color_ptr->y
            + 1.7790 * (ycbcr_color_ptr->u - 128));
    printf("i %i [y %i u %i v %i] [r %i g %i b %i] \n", cb, ycbcr_color_ptr->y,
            ycbcr_color_ptr->u, ycbcr_color_ptr->v, rgb_color_ptr->r,
            rgb_color_ptr->g, rgb_color_ptr->b);
}
