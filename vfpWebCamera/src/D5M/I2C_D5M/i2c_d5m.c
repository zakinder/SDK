// LAST TESTED : 12/27/2019
#include "i2c_d5m.h"
#include <sleep.h>
#include <stdio.h>
#include <xiic_l.h>
#include <xil_printf.h>
#include <xparameters.h>
#include <xstatus.h>
#include "../SYSTEM_CONFIG_HEADER/system_config_defines.h"
#include "../UART/uartio.h"
#define SENSOR_ADDRESS 0x5d // 0xBA - write mode, 0xBB read mode
#define IMG_OUTPUT_CONTROL 7
#define IMG_GLOBAL_GAIN 0x35
#define IMG_PIXEL_CLOCK_CONTROL    10
#define IMG_PIXEL_CLOCK_INVERT 0x8000
#define IMG_RESET 13
#define IIC_BASE_ADDRESS XPAR_D5M_OUTPUT_D5M_IIC_BASEADDR//0x41610000
#define SENSOR_ADDRESS 0x5d // 0xBA - write mode, 0xBB read mode
d5m_i2c_reg d5m_i2c_ptr;
void d5m_regs(d5m_i2c_reg *d5m_i2c_ptr) {
    d5m_i2c_ptr->chipversion = d5m_i2c_read(0x000);
    d5m_i2c_ptr->row_start = d5m_i2c_read(0x001);
    d5m_i2c_ptr->column_star = d5m_i2c_read(0x002);
    d5m_i2c_ptr->row_size = d5m_i2c_read(0x003);
    d5m_i2c_ptr->column_size = d5m_i2c_read(0x004);
    d5m_i2c_ptr->horizontal_blank = d5m_i2c_read(0x005);
    d5m_i2c_ptr->vertical_blank = d5m_i2c_read(0x006);
    d5m_i2c_ptr->output_control = d5m_i2c_read(0x007);
    d5m_i2c_ptr->shutter_width_upper = d5m_i2c_read(0x008);
    d5m_i2c_ptr->shutter_width_lower = d5m_i2c_read(0x009);
    d5m_i2c_ptr->pixel_clock_control = d5m_i2c_read(0x00A);
    d5m_i2c_ptr->restart = d5m_i2c_read(0x00B);
    d5m_i2c_ptr->shutter_delay = d5m_i2c_read(0x00C);
    d5m_i2c_ptr->reset = d5m_i2c_read(0x00D);
    d5m_i2c_ptr->pll_control = d5m_i2c_read(0x010);
    d5m_i2c_ptr->pll_config_1 = d5m_i2c_read(0x011);
    d5m_i2c_ptr->pll_config_2 = d5m_i2c_read(0x012);
    d5m_i2c_ptr->read_mode_1 = d5m_i2c_read(0x01E);
    d5m_i2c_ptr->read_mode_2 = d5m_i2c_read(0x020);
    d5m_i2c_ptr->row_address_mode = d5m_i2c_read(0x022);
    d5m_i2c_ptr->column_address_mode = d5m_i2c_read(0x023);
    d5m_i2c_ptr->green1_gain = d5m_i2c_read(0x02B);
    d5m_i2c_ptr->blue_gain = d5m_i2c_read(0x02C);
    d5m_i2c_ptr->red_gain = d5m_i2c_read(0x02D);
    d5m_i2c_ptr->green2_gain = d5m_i2c_read(0x02E);
    d5m_i2c_ptr->global_gain = d5m_i2c_read(0x035);
    d5m_i2c_ptr->row_black_target = d5m_i2c_read(0x049);
    d5m_i2c_ptr->row_black_default_offset = d5m_i2c_read(0x04B);
    d5m_i2c_ptr->blc_sample_size = d5m_i2c_read(0x05B);
    d5m_i2c_ptr->blc_tune_1 = d5m_i2c_read(0x05C);
    d5m_i2c_ptr->blc_delta_thresholds = d5m_i2c_read(0x05D);
    d5m_i2c_ptr->blc_tune_2 = d5m_i2c_read(0x05E);
    d5m_i2c_ptr->blc_target_thresholds = d5m_i2c_read(0x05F);
    d5m_i2c_ptr->green1_offset = d5m_i2c_read(0x060);
    d5m_i2c_ptr->green2_offset = d5m_i2c_read(0x061);
    d5m_i2c_ptr->black_level_calibration = d5m_i2c_read(0x062);
    d5m_i2c_ptr->red_offset = d5m_i2c_read(0x063);
    d5m_i2c_ptr->blue_offset = d5m_i2c_read(0x064);
    d5m_i2c_ptr->test_pattern_control = d5m_i2c_read(0x0A0);
    d5m_i2c_ptr->test_pattern_green = d5m_i2c_read(0x0A1);
    d5m_i2c_ptr->test_pattern_red = d5m_i2c_read(0x0A2);
    d5m_i2c_ptr->test_pattern_blue = d5m_i2c_read(0x0A3);
    d5m_i2c_ptr->test_pattern_bar_width = d5m_i2c_read(0x0A4);
}
int img_read_register(u8 address, u16 *value) {
    u8 buffer[2];
    if (XIic_Send(IIC_BASE_ADDRESS, SENSOR_ADDRESS, &address, 1, XIIC_STOP)
            != 1) {
        return XST_FAILURE;
    }
    if (XIic_Recv(IIC_BASE_ADDRESS, SENSOR_ADDRESS, buffer, 2, XIIC_STOP)
            != 2) {
        return XST_FAILURE;
    }
    *value = ((buffer[0] << 8) & 0xFF00) + buffer[1];
    return XST_SUCCESS;
}
u16 d5m_i2c_read(u8 address) {
    u8 buffer[2];
    if (XIic_Send(IIC_BASE_ADDRESS, SENSOR_ADDRESS, &address, 1, XIIC_STOP)
            != 1) {
        return XST_FAILURE;
    }
    if (XIic_Recv(IIC_BASE_ADDRESS, SENSOR_ADDRESS, buffer, 2, XIIC_STOP)
            != 2) {
        return XST_FAILURE;
    }
    return ((buffer[0] << 8) & 0xFF00) + buffer[1];
}
void d5m_registers_write(u8 address, u16 value) {
    int ret;
    ret = d5m_i2c_write(address, value); //Blue Gain
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
        printf("address[%x],value[%x]\n", address, value);
    }
}
int d5m_i2c_write_to_reg(u8 address, u32 value) {
    u8 buffer[3] = { SENSOR_ADDRESS, 0, 0 };
    buffer[0] = address;
    buffer[1] = (value & 0xFFFF00) >> 16;
    buffer[2] = (value & 0x00FF00) >> 8;
    buffer[3] = (value & 0xFF);
    if (XIic_Send(XPAR_D5M_OUTPUT_D5M_IIC_BASEADDR, SENSOR_ADDRESS, buffer, 3,
    XIIC_STOP) != 3) {
        return 0;
    }
    else {
        return 1;
    }
}
void d5m_i2c_write_reg_val(u8 address, u32 value) {
    int ret;
    ret = d5m_i2c_write_to_reg(address, value); //Blue Gain
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
        printf("address[%x],value[%x]\n", address, value);
    }
}
int d5m_i2c_write(u8 address, u16 value) {
    u8 buffer[3] = { SENSOR_ADDRESS, 0, 0 };
    buffer[0] = address;
    buffer[1] = (value & 0xFF00) >> 8;
    buffer[2] = (value & 0xFF);
    if (XIic_Send(XPAR_D5M_OUTPUT_D5M_IIC_BASEADDR, SENSOR_ADDRESS, buffer, 3,
    XIIC_STOP) != 3) {
        return 0;
    }
    else {
        return 1;
    }
}
void d5m_config1() {
    u16 R_20_value = 0xc000;           //    Mirror Row and Columns
    d5m_registers_write(32, R_20_value);           // Mirror Row and Columns
    d5m_registers_write(9, 1984);       // Exposure default_exposure
    d5m_registers_write(5, 0);          // H_Blanking
    d5m_registers_write(6, 25);         // V_Blanking
    d5m_registers_write(10, 32768);     // change latch
    d5m_registers_write(43, 2);         // [h2B][h0013]Green 1 Gain
    d5m_registers_write(44, 3);         // [h2C][h009A]Blue Gain
    d5m_registers_write(45, 3);         // [h2D][h019C]Red Gain
    d5m_registers_write(46, 2);         // [h2E][h0013]Green 2 Gain
    d5m_registers_write(16, 81);        // set up PLL power on
    d5m_registers_write(17, 6151);      // PLL_m_Factor<<8+PLL_n_Divider
    d5m_registers_write(18, 2);         // PLL_p1_Divider
    usleep(1000);                      //1ms
    d5m_registers_write(16, 83);        // set USE PLL
    usleep(1000);                      //1ms
    d5m_registers_write(152, 0);        // disble calibration
    d5m_registers_write(1, 54);         // sensor_start_row
    d5m_registers_write(2, 16);         // sensor_start_column
    d5m_registers_write(3, 480);        // sensor_row_size
    d5m_registers_write(4, 640);       // sensor_column_size
    d5m_registers_write(34, 0);         // sensor_row_mode
    d5m_registers_write(35, 0);         // sensor_column_mode
    d5m_registers_write(73, 424);       // row black target
}
void d5m_config2() {
    //VGA_640x480p60 iZOOM_MODE not
    u16 R_20_value = 0xc000;           //    Mirror Row and Columns
    d5m_registers_write(32, R_20_value);           // Mirror Row and Columns
    d5m_registers_write(9, 1984);       // [h09][h7c0]Exposure default_exposure
    d5m_registers_write(5, 0);          // [h05][h0000]H_Blanking
    d5m_registers_write(6, 25);         // [h06][h19]V_Blanking
    d5m_registers_write(10, 32768);     // [h0A][h8000]change latch
    d5m_registers_write(43, 3);         // [h2B][h0013]Green 1 Gain
    d5m_registers_write(44, 2);         // [h2C][h009A]Blue Gain
    d5m_registers_write(45, 2);         // [h2D][h019C]Red Gain
    d5m_registers_write(46, 3);         // [h2E][h0013]Green 2 Gain
    d5m_registers_write(16, 81);        // [h10][h0051]set up PLL power on
    d5m_registers_write(17, 6151);  // [h11][h1f04]PLL_m_Factor<<8+PLL_n_Divider
    d5m_registers_write(18, 1);         // [h12][h0001]PLL_p1_Divider
    usleep(1000);                      //1ms
    d5m_registers_write(16, 83);        // [h10][h0053]set USE PLL
    usleep(1000);                      //1ms
    d5m_registers_write(152, 0);        // [h98][h1807]disble calibration
    d5m_registers_write(1, 54);         // [h01][h0036]sensor_start_row
    d5m_registers_write(2, 16);         // [h02][h0010]sensor_start_column
    d5m_registers_write(3, 480);        // [h03][h03BF]sensor_row_size
    d5m_registers_write(4, 640);       // [h04][h04FF]sensor_column_size
    d5m_registers_write(34, 0);         // [h22][h0000]sensor_row_mode
    d5m_registers_write(35, 0);         // [h23][h0000]sensor_column_mode
    d5m_registers_write(73, 424);       // [h49][h01A8]row black target
}
void d5m_config3() {
    //VGA_640x480p60 iZOOM_MODE
    u16 R_20_value = 0xc000;           //  Mirror Row and Columns
    d5m_registers_write(32, R_20_value);           //  Mirror Row and Columns
    d5m_registers_write(9, 1984);       // [h09][h7c0]Exposure default_exposure
    d5m_registers_write(5, 0);          // [h05][h0000]H_Blanking
    d5m_registers_write(6, 25);         // [h06][h19]V_Blanking
    d5m_registers_write(10, 32768);     // [h0A][h8000]change latch
    d5m_registers_write(43, 2);         // [h2B][h0013]Green 1 Gain
    d5m_registers_write(44, 3);         // [h2C][h009A]Blue Gain
    d5m_registers_write(45, 3);         // [h2D][h019C]Red Gain
    d5m_registers_write(46, 2);         // [h2E][h0013]Green 2 Gain
    d5m_registers_write(16, 81);        // [h10][h0051]set up PLL power on
    d5m_registers_write(17, 6151);  // [h11][h1f04]PLL_m_Factor<<8+PLL_n_Divider
    d5m_registers_write(18, 2);         // [h12][h0001]PLL_p1_Divider
    usleep(1000);                      //1ms
    d5m_registers_write(16, 83);        // [h10][h0053]set USE PLL
    usleep(1000);                      //1ms
    d5m_registers_write(152, 0);        // [h98][h1807]disble calibration
    d5m_i2c_write_reg_val(1, 65536);    // [h01][h10000]sensor_start_row
    d5m_i2c_write_reg_val(2, 131072);   // [h02][h20000]sensor_start_column
    d5m_i2c_write_reg_val(3, 480);   // [h03][h3077F]sensor_row_size
    d5m_registers_write(4, 640);       // [h04][h04FF]sensor_column_size
    d5m_i2c_write_reg_val(34, 2228241); // [h22][h220011]sensor_row_mode
    d5m_i2c_write_reg_val(35, 2293777); // [h23][h230011]sensor_column_mode
    d5m_registers_write(73, 424);       // [h49][h01A8]row black target
}
void d5m_config4() {
    int ret;
    printf("-----------------------------------------------\n");
    ret = d5m_i2c_write(32, 49152);       //Mirror Row and Columns
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(9, 1984);       //Exposure default_exposure
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(5, 0);       //H_Blanking
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(6, 25);       //V_Blanking
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(10, 32768);       //change latch
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(43, 11);       //Green 1 Gain
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(44, 15);       //Blue Gain
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(45, 15);       //Red Gain
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(46, 11);       //Green 2 Gain
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(16, 81);       //set up PLL power on
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(17, 6151);       //PLL_m_Factor<<8+PLL_n_Divider
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(18, 2);       //PLL_p1_Divider
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(16, 83);       //set USE PLL
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    printf("Done Initialized D5M PLL\n");
    ret = d5m_i2c_write(152, 0);       //disble calibration
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(1, 54);       //sensor_start_row
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(2, 16);       //sensor_start_column
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(3, 959);       //sensor_row_size
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(4, 1279);       //sensor_column_size
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(34, 0);       //sensor_row_mode
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(35, 0);       //sensor_column_mode
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(73, 424);       //row black target
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(13, 1);       //row black target
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
    ret = d5m_i2c_write(13, 0);       //row black target
    if (!ret) {
        printf("ERROR : XST_FAILURE: %d\n", ret);
    }
}
int camera_exposer(u16 exposer_value) {
    u8 buffer[3] = { SENSOR_ADDRESS, 0, 0 };
    buffer[0] = 9;
    buffer[1] = (exposer_value & 0xFF00) >> 8;
    buffer[2] = (exposer_value & 0xFF);
    if (XIic_Send(XPAR_D5M_OUTPUT_D5M_IIC_BASEADDR, SENSOR_ADDRESS, buffer, 3,
    XIIC_STOP) != 3) {
        return XST_FAILURE;
    }
    else {
        return XST_SUCCESS;
    }
}
void get_exposer() {
    pvideo.exposer = d5m_i2c_read(0x009);
    printf("Exposer :%i\n", (unsigned) pvideo.exposer);
}
void d5m_read_color_gain_expos(d5m_i2c_reg *d5m_i2c_ptr) {
    xil_printf("\n\r");
    xil_printf("\t [1943 ]-SHUTTER WIDTH LOWER    = 0x%04d\n\r",
            d5m_i2c_ptr->shutter_width_lower);
    xil_printf("\t [8    ]-GREEN1 GAIN            = 0x%04d\n\r",
            d5m_i2c_ptr->green1_gain);
    xil_printf("\t [8    ]-BLUE   GAIN            = 0x%04d\n\r",
            d5m_i2c_ptr->blue_gain);
    xil_printf("\t [8    ]-RED    GAIN            = 0x%04d\n\r",
            d5m_i2c_ptr->red_gain);
    xil_printf("\t [8    ]-GREEN2 GAIN            = 0x%04d\n\r",
            d5m_i2c_ptr->green2_gain);
}
void d5m_color_gain() {
    int ret = 0;
    u32 gr1 = 0;
    u32 gr2 = 0;
    u32 blu = 0;
    u32 red = 0;
    u32 exp = 0;
    printf("Enter Green1 Gain Value\n");
    menu_print_prompt();
    gr1 = uart_prompt_io();
    printf("Enter Blue Gain Value\n");
    menu_print_prompt();
    blu = uart_prompt_io();
    printf("Enter Red Gain Value\n");
    menu_print_prompt();
    red = uart_prompt_io();
    printf("Enter Green2 Gain Value\n");
    menu_print_prompt();
    gr2 = uart_prompt_io();
    printf("Enter Exposer Value\n");
    menu_print_prompt();
    exp = uart_prompt_io();
    ret = d5m_i2c_write(43, gr1);   //Green1 Gain
    usleep(1);
    ret |= d5m_i2c_write(44, blu);   //Blue Gain
    usleep(1);
    ret |= d5m_i2c_write(45, red); //Red Gain
    usleep(1);
    ret |= d5m_i2c_write(46, gr2);  //Green2 Gain
    usleep(1);
    ret |= d5m_i2c_write(9, exp);  //frame exposer
    usleep(1);
    //or combine check
    if (ret == 1) print("transmitted\n");
    else print("more bytes to transmit-- check error\n");
}
void set_color_gain(u16 gain, u16 exp) {
    int ret = 0;
    ret = d5m_i2c_write(43, gain); //Green1 Gain
    usleep(1);
    ret |= d5m_i2c_write(44, gain); //Blue Gain
    usleep(1);
    ret |= d5m_i2c_write(45, gain); //Red Gain
    usleep(1);
    ret |= d5m_i2c_write(46, gain); //Green2 Gain
    usleep(1);
    ret |= d5m_i2c_write(9, exp);  //frame exposer
    usleep(1);
    //or combine check
    if (ret == 1) print("Transmitted\n");
    else print("more bytes to transmit-- check error\n");
    //read registers
    d5m_regs(&d5m_i2c_ptr);
    d5m_read_color_gain_expos(&d5m_i2c_ptr);
}
void config_d5m_gen_testpattern(u16 pattern_num) {
    int ret = 0;
    ret = d5m_i2c_write(161, 50);  // Test_Pattern_Green
    ret = d5m_i2c_write(162, 100); // Test_Pattern_Red
    ret = d5m_i2c_write(163, 200); // Test_Pattern_Blue
    ret = d5m_i2c_write(164, 40);  // Test_Pattern_Bar_Width
    if (pattern_num != 0) {
        if (pattern_num == 1) {
            ret = d5m_i2c_write(160, 1);   //0000001 1  color field
        }
        if (pattern_num == 2) {
            ret |= d5m_i2c_write(160, 9);  //0001001 9  horizontal gradient
        }
        if (pattern_num == 3) {
            ret |= d5m_i2c_write(160, 17); //0010001 17 vertical gradient
        }
        if (pattern_num == 4) {
            ret |= d5m_i2c_write(160, 25); //0011001 25 diagonal
        }
        if (pattern_num == 5) {
            ret |= d5m_i2c_write(160, 33); //0100001 33 classic
        }
        if (pattern_num == 6) {
            ret |= d5m_i2c_write(160, 41); //0101001 41 marching 1's
        }
        if (pattern_num == 7) {
            ret |= d5m_i2c_write(160, 49); //0110001 49 monochrome horizontal bars
        }
        if (pattern_num == 8) {
            ret |= d5m_i2c_write(160, 57); //0111001 57 monochrome vertical bars
        }
        if (pattern_num == 9) {
            ret |= d5m_i2c_write(160, 65); //1000001 65 vertical color bars
        }
    }
    else {
        ret |= d5m_i2c_write(160, 0);  //video
    }
    //or combine check
    if (ret == 1) print("transmitted\n");
    else print("more bytes to transmit-- check error\n");
}
void camera_set_registersv1() {
    int ret = 0;
    u16 R_01_value = 0x0036; //    set start row
    u16 R_02_value = 0x0010; //    set start column
    u16 R_03_value = 0x0438; //    set row size 1920[780] 600[258]
    u16 R_04_value = 0x0780; //    set column size 1080[438] 400[190]
    u16 R_05_value = 0x0000; //    H_Blanking
    u16 R_06_value = 0x0019; //    V_Blanking
    u16 R_0A_value = 0x8000; //    change latch
    u16 R_10_value = 0x0051; //    set up PLL power on
    u16 R_11_value = 0x1807; //    PLL_m_Factor<<8+PLL_n_Divider
    u16 R_12_value = 0x0002; //    PLL_p1_Divider
    usleep(100);
    u16 R_1X_value = 0x0053; //    set USE PLL
    usleep(100);
    u16 R_20_value = 0xc000; //    Mirror Row and Columns
    u16 R_22_value = 0x0000; //    set row mode in bin mode
    u16 R_23_value = 0x0000; //    set column mode     in bin mode
    u16 R_2B_value = 0x000f; // 43[15] Green 1 Gain
    u16 R_2C_value = 0x000f; // 44[15] Blue Gain
    u16 R_2D_value = 0x000f; // 45[15] Red Gain
    u16 R_2E_value = 0x000f; // 46[15] Green 2 Gain
    u16 R_98_value = 0x0000; // 152[15] disble calibration
    u16 R_A0_value = 0x0000; // 160[15] Test pattern control
    u16 R_A1_value = 0x0000; // 161[15] Test green pattern value
    u16 R_A2_value = 0x0FFF; // 162[4095] Test red pattern value
    u16 R_49_value = 0x01A8; // 73[424] row black target
    ret |= d5m_i2c_write(0x001, R_01_value);
    printf("Row Start: H: %02x , D: %d\n", R_01_value, R_01_value);
    ret |= d5m_i2c_write(0x002, R_02_value);
    printf("Column Start: H: %02x , D: %d\n", R_02_value, R_02_value);
    ret |= d5m_i2c_write(0x003, R_03_value);
    printf("Row Size: H: %02x , D: %d\n", R_03_value, R_03_value);
    ret |= d5m_i2c_write(0x004, R_04_value);
    printf("Column Size: H: %02x , D: %d\n", R_04_value, R_04_value);
    ret |= d5m_i2c_write(0x005, R_05_value);
    printf("Horizontal Blank: H: %02x , D: %d\n", R_05_value, R_05_value);
    ret |= d5m_i2c_write(0x006, R_06_value);
    printf("Vertical Blank: H: %02x , D: %d\n", R_06_value, R_06_value);
    ret |= d5m_i2c_write(0x00A, R_0A_value);
    printf("Pixel Clock Control: H: %02x , D: %d\n", R_0A_value, R_0A_value);
    ret |= d5m_i2c_write(0x010, R_10_value);
    printf("PLL Control: H: %02x , D: %d\n", R_10_value, R_10_value);
    ret |= d5m_i2c_write(0x011, R_11_value);
    printf("PLL Config 1: H: %02x , D: %d\n", R_11_value, R_11_value);
    ret |= d5m_i2c_write(0x012, R_12_value);
    printf("PLL Config 2: H: %02x , D: %d\n", R_12_value, R_12_value);
    ret |= d5m_i2c_write(0x010, R_1X_value);
    printf("Pixel Clock Control: H: %02x , D: %d\n", R_1X_value, R_1X_value);
    ret |= d5m_i2c_write(0x020, R_20_value);
    printf("Read Mode 2: H: %02x , D: %d\n", R_20_value, R_20_value);
    ret |= d5m_i2c_write(0x022, R_22_value);
    printf("Row Address Mode: H: %02x , D: %d\n", R_22_value, R_22_value);
    ret |= d5m_i2c_write(0x023, R_23_value);
    printf("Column address mode: H: %02x , D: %d\n", R_23_value, R_23_value);
    ret |= d5m_i2c_write(0x02B, R_2B_value);
    printf("Green1 Gain: H: %02x , D: %d\n", R_2B_value, R_2B_value);
    ret |= d5m_i2c_write(0x02C, R_2C_value);
    printf("Blue Gain: H: %02x , D: %d\n", R_2C_value, R_2C_value);
    ret |= d5m_i2c_write(0x02D, R_2D_value);
    printf("Red Gain: H: %02x , D: %d\n", R_2D_value, R_2D_value);
    ret |= d5m_i2c_write(0x02E, R_2E_value);
    printf("Green2 Gain: H: %02x , D: %d\n", R_2E_value, R_2E_value);
    ret |= d5m_i2c_write(0x049, R_49_value);
    printf("TestPatBarWidth: H: %02x , D: %d\n", R_49_value, R_49_value);
    ret |= d5m_i2c_write(0x098, R_98_value);
    printf("Test Pattern Blue: H: %02x , D: %d\n", R_98_value, R_98_value);
    ret |= d5m_i2c_write(0x0A0, R_A0_value);
    printf("TestPatternControl: H: %02x , D: %d\n", R_A0_value, R_A0_value);
    ret |= d5m_i2c_write(0x0A1, R_A1_value);
    printf("Test Pattern Green: H: %02x , D: %d\n", R_A1_value, R_A1_value);
    ret |= d5m_i2c_write(0x0A2, R_A2_value);
    printf("Test Pattern Red: H: %02x , D: %d\n", R_A2_value, R_A2_value);
}
void d5m_i2c_write_reg() {
    int ret = 0;
    u16 R_01_value = 0x0036; //    set start row
    u16 R_02_value = 0x0010; //    set start column
    u16 R_03_value = 0x0438; //    set row size    1080[438] 400[190]
    u16 R_04_value = 0x0780; //    set column size 1920[780] 600[258]
    u16 R_05_value = 0x0000; //    H_Blanking
    u16 R_06_value = 0x0019; //    V_Blanking
    u16 R_0A_value = 0x8000; //    change latch
    /*
     PLL Programming Example
     The following is an example of register settings
     for input fXCLKIN = 24 MHz and output fPIXCLK = 96MHz
     REG=0, 0x10, 0x0051 // PLL_CONTROL; POWER UP PLL
     REG=0,0x11, 0x4805 // PLL_CONFIG_1; CONFIG_1; PLL_n_Divider=5,
     PLL_m_Factor=72 (N=6, M=72)
     REG=0,0x12, 0x0002 // PLL_CONFIG_2; PLL_p1_Divider=2, (P1=3)
     Delay=1, // Wait 1ms for VCO to lock
     REG=0, 0x10, 0x0053 // PLL_CONTROL; USE PLL
     DELAY=1, // Wait 1ms
     In this example,
     PIXCLK = ( fXCLKIN x M ) / ( N x P1 )
     = 24 MHz x 72 / 6 x 3 = 96 MHz
     In this example,
     PIXCLK = ( fXCLKIN x M ) / ( N x P1 )
     = 25 MHz x 36 / 6 x 3 = 50 MHz
     In this example,
     PIXCLK = ( fXCLKIN x M ) / ( N x P1 )
     = 25 MHz x 18 / 6 x 3 = 50 MHz
     */
    u16 camera_exposer_value = 0x07D0;
    u16 R_10_value = 0x0051; //    set up PLL power on
    u16 R_11_value = 0x1807; //    PLL_m_Factor<<8+PLL_n_Divider
    u16 R_12_value = 0x0002; //    PLL_p1_Divider
    usleep(100);
    u16 R_1X_value = 0x0053; //    set USE PLL
    usleep(100);
    u16 R_20_value = 0xc000; //    Mirror Row and Columns
    u16 R_22_value = 0x0000; //    set row mode in bin mode
    u16 R_23_value = 0x0000; //    set column mode     in bin mode
    u16 R_2B_value = 0x0002; // 43[15] Green 1 Gain
    u16 R_2C_value = 0x0003; // 44[15] Blue Gain
    u16 R_2D_value = 0x0003; // 45[15] Red Gain
    u16 R_2E_value = 0x0002; // 46[15] Green 2 Gain
    u16 R_98_value = 0x0000; // 152[15] disble calibration
    u16 R_A0_value = 0x0000; // 160[15] Test pattern control
    u16 R_A1_value = 0x0000; // 161[15] Test green pattern value
    u16 R_A2_value = 0x0FFF; // 162[4095] Test red pattern value
    u16 R_49_value = 0x01A8; // 73[424] row black target
    // 1 Row Start
    //printf("--------------Start Write-----------------\n");
    ret |= d5m_i2c_write(0x001, R_01_value);
    //printf("Row Start: H: %02x , D: %d\n",R_01_value);
    // 2 Column Start
    ret |= d5m_i2c_write(0x002, R_02_value);
    //printf("Column Start: H: %02x , D: %d\n",R_02_value);
    // 3 Row Size
    ret |= d5m_i2c_write(0x003, R_03_value);
    //printf("Row Size: H: %02x , D: %d\n",R_03_value);
    // 4 Column Size
    ret |= d5m_i2c_write(0x004, R_04_value);
    //printf("Column Size: H: %02x , D: %d\n",R_04_value);
    // 5 Horizontal Blank
    ret |= d5m_i2c_write(0x005, R_05_value);
    //printf("Horizontal Blank: H: %02x , D: %d\n",R_05_value);
    // 6 Vertical Blank
    ret |= d5m_i2c_write(0x006, R_06_value);
    //printf("Vertical Blank: H: %02x , D: %d\n",R_06_value);
    // 7 Output Control
    //ret |=  d5m_i2c_write(0x007,value);
    //printf("Output Control: %d\n",value);
    // 8 Shutter Width Upper
    //ret |=  d5m_i2c_write(0x008,value);
    //printf("Shutter Width Upper: %d\n",value);
    // 9 Shutter Width Lower
    ret |= d5m_i2c_write(0x009, camera_exposer_value);
    //printf("Shutter Width Lower: %d\n",value);
    // 10|Pixel Clock Control
    ret |= d5m_i2c_write(0x00A, R_0A_value);
    //printf("Pixel Clock Control: H: %02x , D: %d\n",R_0A_value);
    // 12|Shutter Delay
    //ret |=  d5m_i2c_write(0x00C,value);
    //printf("Shutter Delay: %d\n",value);
    //16|PLL Control
    ret |= d5m_i2c_write(0x010, R_10_value);
    //printf("PLL Control: H: %02x , D: %d\n",R_10_value);
    //17|PLL Config 1
    ret |= d5m_i2c_write(0x011, R_11_value);
    //printf("PLL Config 1: H: %02x , D: %d\n",R_11_value);
    //18|PLL Config 2
    ret |= d5m_i2c_write(0x012, R_12_value);
    //printf("PLL Config 2: H: %02x , D: %d\n",R_12_value);
    //10|PLL Control
    ret |= d5m_i2c_write(0x010, R_1X_value);
    //printf("Pixel Clock Control: H: %02x , D: %d\n",R_1X_value);
    // 30|Read Mode 1
    //ret |=  d5m_i2c_write(0x01E,value);
    //printf("Read Mode 1: %d\n",value);
    // 32|Read Mode 2
    ret |= d5m_i2c_write(0x020, R_20_value);
    //printf("Read Mode 2: H: %02x , D: %d\n",R_20_value);
    // 34|Row Address Mode
    ret |= d5m_i2c_write(0x022, R_22_value);
    //printf("Row Address Mode: H: %02x , D: %d\n",R_22_value);
    // 35|Column address mode
    ret |= d5m_i2c_write(0x023, R_23_value);
    //printf("Column address mode: H: %02x , D: %d\n",R_23_value);
    // 43|Green1 Gain
    ret |= d5m_i2c_write(0x02B, R_2B_value);
    //printf("Green1 Gain: H: %02x , D: %d\n",R_2B_value);
    // 44|Blue Gain
    ret |= d5m_i2c_write(0x02C, R_2C_value);
    //printf("Blue Gain: H: %02x , D: %d\n",R_2C_value);
    // 45|Red Gain
    ret |= d5m_i2c_write(0x02D, R_2D_value);
    //printf("Red Gain: H: %02x , D: %d\n",R_2D_value);
    // 46|Green2 Gain
    ret |= d5m_i2c_write(0x02E, R_2E_value);
    //printf("Green2 Gain: H: %02x , D: %d\n",R_2E_value);
    // 53|Global Gain
    //ret |=  d5m_i2c_write(0x035,value);
    //printf("Global Gain: %d\n",value);
    // 73|row black target
    ret |= d5m_i2c_write(0x049, R_49_value);
    //printf("TestPatBarWidth: H: %02x , D: %d\n",R_49_value);
    // 75|Row black offse
    //ret |=  d5m_i2c_write(0x04B,value);
    //printf("Row black offse: %d\n",value);
    // 152|disble calibration
    ret |= d5m_i2c_write(0x098, R_98_value);
    //printf("Test Pattern Blue: H: %02x , D: %d\n",R_98_value);
    // 160|TestPatternControl
    ret |= d5m_i2c_write(0x0A0, R_A0_value);
    //printf("TestPatternControl: H: %02x , D: %d\n",R_A0_value);
    // 161|Test Pattern Green
    ret |= d5m_i2c_write(0x0A1, R_A1_value);
    //printf("Test Pattern Green: H: %02x , D: %d\n",R_A1_value);
    // 162|Test Pattern Red
    ret |= d5m_i2c_write(0x0A2, R_A2_value);
    //printf("Test Pattern Red: H: %02x , D: %d\n",R_A2_value);
    // 163|Test Pattern Blue
    //ret |=  d5m_i2c_write(0x0A3,value);
    //printf("Test Pattern Blue: %d\n",value);
    // 164|TestPatBarWidth
    //ret |=  d5m_i2c_write(0x0A4,value);
    //printf("TestPatBarWidth: %d\n",value);
    //printf("--------------End Write-----------------\n");
    //or combine check
    if (ret == 1) print("All registers are transmitted to d5m\n");
    else print("more bytes to transmit-- check error\n");
}
void camera_read(d5m_i2c_reg *d5m_i2c_ptr) {
    xil_printf(
            "\t A[d000-h00]-D[d6145 -h1801]-CHIP VERSION           = (0x%04x)\n\r",
            d5m_i2c_ptr->chipversion);
    xil_printf(
            "\t A[d001-h01]-D[d54   -h0036]-ROW START              = (0x%04x)\n\r",
            d5m_i2c_ptr->row_start);
    xil_printf(
            "\t A[d002-h02]-D[d16   -h0010]-COLUMN STAR            = (0x%04x)\n\r",
            d5m_i2c_ptr->column_star);
    xil_printf(
            "\t A[d003-h03]-D[d1943 -h0797]-ROW SIZE               = (0x%04x)\n\r",
            d5m_i2c_ptr->row_size);
    xil_printf(
            "\t A[d004-h04]-D[d2591 -h0A1F]-COLUMN SIZE            = (0x%04x)\n\r",
            d5m_i2c_ptr->column_size);
    xil_printf(
            "\t A[d005-h05]-D[d0    -h0000]-HORIZONTAL BLANK       = (0x%04x)\n\r",
            d5m_i2c_ptr->horizontal_blank);
    xil_printf(
            "\t A[d006-h06]-D[d25   -h0019]-VERTICAL BLANK         = (0x%04x)\n\r",
            d5m_i2c_ptr->vertical_blank);
    xil_printf(
            "\t A[d007-h07]-D[d8066 -h1F82]-OUTPUT CONTROL         = (0x%04x)\n\r",
            d5m_i2c_ptr->output_control);
    xil_printf(
            "\t A[d008-h08]-D[d0    -h0000]-SHUTTER WIDTH UPPER    = (0x%04x)\n\r",
            d5m_i2c_ptr->shutter_width_upper);
    uart_Yellow_Text();
    xil_printf(
            "\t A[d009-h09]-D[d1943 -h0797]-SHUTTER WIDTH LOWER    = (0x%04x)\n\r",
            d5m_i2c_ptr->shutter_width_lower);
    uart_Default_Text();
    xil_printf(
            "\t A[d010-h0A]-D[d0    -h0000]-PIXEL CLOCK CONTROL    = (0x%04x)\n\r",
            d5m_i2c_ptr->pixel_clock_control);
    xil_printf(
            "\t A[d011-h0B]-D[d0    -h0000]-RESTART                = (0x%04x)\n\r",
            d5m_i2c_ptr->restart);
    xil_printf(
            "\t A[d012-h0C]-D[d0    -h0000]-SHUTTER DELAY          = (0x%04x)\n\r",
            d5m_i2c_ptr->shutter_delay);
    xil_printf(
            "\t A[d013-h0D]-D[d0    -h0000]-RESET                  = (0x%04x)\n\r",
            d5m_i2c_ptr->reset);
    xil_printf(
            "\t A[d016-h10]-D[d80   -h0050]-PLL CONTROL            = (0x%04x)\n\r",
            d5m_i2c_ptr->pll_control);
    xil_printf(
            "\t A[d017-h11]-D[d25604-h6404]-PLL CONFIG 1           = (0x%04x)\n\r",
            d5m_i2c_ptr->pll_config_1);
    xil_printf(
            "\t A[d018-h12]-D[d0    -h0000]-PLL CONFIG 2           = (0x%04x)\n\r",
            d5m_i2c_ptr->pll_config_2);
    xil_printf(
            "\t A[d030-h1E]-D[d16   -h0010]-READ MODE 1            = (0x%04x)\n\r",
            d5m_i2c_ptr->read_mode_1);
    xil_printf(
            "\t A[d032-h20]-D[d64   -h0040]-READ MODE 2            = (0x%04x)\n\r",
            d5m_i2c_ptr->read_mode_2);
    xil_printf(
            "\t A[d034-h22]-D[d0    -h0000]-ROW ADDRESS MODE       = (0x%04x)\n\r",
            d5m_i2c_ptr->row_address_mode);
    xil_printf(
            "\t A[d035-h23]-D[d0    -h0000]-COLUMN ADDRESS MODE    = (0x%04x)\n\r",
            d5m_i2c_ptr->column_address_mode);
    xil_printf(
            "\t A[d043-h2B]-D[d8    -h0008]-GREEN1 GAIN            = (0x%04x)\n\r",
            d5m_i2c_ptr->green1_gain);
    xil_printf(
            "\t A[d044-h2C]-D[d8    -h0008]-BLUE GAIN              = (0x%04x)\n\r",
            d5m_i2c_ptr->blue_gain);
    xil_printf(
            "\t A[d045-h2D]-D[d8    -h0008]-RED GAIN               = (0x%04x)\n\r",
            d5m_i2c_ptr->red_gain);
    xil_printf(
            "\t A[d046-h2E]-D[d8    -h0008]-GREEN2 GAIN            = (0x%04x)\n\r",
            d5m_i2c_ptr->green2_gain);
    xil_printf(
            "\t A[d053-h35]-D[d8    -h0008]-GLOBAL GAIN            = (0x%04x)\n\r",
            d5m_i2c_ptr->global_gain);
    xil_printf(
            "\t A[d073-h49]-D[d168  -h00A8]-ROW BLACK TARGET       = (0x%04x)\n\r",
            d5m_i2c_ptr->row_black_target);
    xil_printf(
            "\t A[d075-h4B]-D[d40   -h0028]-ROW BLACK OFFSET       = (0x%04x)\n\r",
            d5m_i2c_ptr->row_black_default_offset);
    xil_printf(
            "\t A[d091-h5B]-D[d1    -h0001]-BLC SAMPLE SIZE        = (0x%04x)\n\r",
            d5m_i2c_ptr->blc_sample_size);
    xil_printf(
            "\t A[d092-h5C]-D[d90   -h005A]-BLC TUNE 1             = (0x%04x)\n\r",
            d5m_i2c_ptr->blc_tune_1);
    xil_printf(
            "\t A[d093-h5D]-D[d11539-h2D13]-BLC DELTA THRESHOLDS   = (0x%04x)\n\r",
            d5m_i2c_ptr->blc_delta_thresholds);
    xil_printf(
            "\t A[d094-h5E]-D[d0    -h41FF]-BLC TUNE 2             = (0x%04x)\n\r",
            d5m_i2c_ptr->blc_tune_2);
    xil_printf(
            "\t A[d095-h5F]-D[d8989 -h231D]-BLC TARGET THRESHOLDS  = (0x%04x)\n\r",
            d5m_i2c_ptr->blc_target_thresholds);
    xil_printf(
            "\t A[d096-h60]-D[d32   -h0020]-GREEN1 OFFSET          = (0x%04x)\n\r",
            d5m_i2c_ptr->green1_offset);
    xil_printf(
            "\t A[d097-h61]-D[d32   -h0020]-GREEN2 OFFSET          = (0x%04x)\n\r",
            d5m_i2c_ptr->green2_offset);
    xil_printf(
            "\t A[d098-h62]-D[d0    -h0000]-BLACK LEVEL CALIBRATION= (0x%04x)\n\r",
            d5m_i2c_ptr->black_level_calibration);
    xil_printf(
            "\t A[d099-h63]-D[d32   -h0020]-RED OFFSET             = (0x%04x)\n\r",
            d5m_i2c_ptr->red_offset);
    xil_printf(
            "\t A[d100-h64]-D[d32   -h0020]-BLUE OFFSET            = (0x%04x)\n\r",
            d5m_i2c_ptr->blue_offset);
    xil_printf(
            "\t A[d160-hA0]-D[d0    -h0000]-TEST PATTERN CONTROL   = (0x%04x)\n\r",
            d5m_i2c_ptr->test_pattern_control);
    xil_printf(
            "\t A[d161-hA1]-D[d0    -h0000]-TEST PATTERN GREEN     = (0x%04x)\n\r",
            d5m_i2c_ptr->test_pattern_green);
    xil_printf(
            "\t A[d162-hA2]-D[d0    -h0000]-TEST PATTERN RED       = (0x%04x)\n\r",
            d5m_i2c_ptr->test_pattern_red);
    xil_printf(
            "\t A[d163-hA3]-D[d0    -h0000]-TEST PATTERN BLUE      = (0x%04x)\n\r",
            d5m_i2c_ptr->test_pattern_blue);
    xil_printf(
            "\t A[d164-hA4]-D[d0    -h0000]-TEST PATTERN BAR WIDTH = (0x%04x)\n\r",
            d5m_i2c_ptr->test_pattern_bar_width);
}
void read_camera_exposer(d5m_i2c_reg *d5m_i2c_ptr) {
    uart_Yellow_Text();
    xil_printf(
            "\t A[d009-h09]-D[d1943 -h0797]-SHUTTER WIDTH LOWER    = (0x%04x)\n\r",
            d5m_i2c_ptr->shutter_width_lower);
    uart_Default_Text();
}
void read_exposer_register() {
    d5m_i2c_reg d5m_i2c_ptr;
    d5m_regs(&d5m_i2c_ptr);
    read_camera_exposer(&d5m_i2c_ptr);
    printf("exposer_register\n");
}
void camera_update() {
    d5m_i2c_reg d5m_i2c_ptr;
    d5m_regs(&d5m_i2c_ptr);
    camera_read(&d5m_i2c_ptr);
    printf("D5mRegs Updated\n");
}
void d5mr_reg() {
    d5m_i2c_reg d5m_i2c_ptr;
    d5m_regs(&d5m_i2c_ptr);    //read values from d5m device
    camera_read(&d5m_i2c_ptr);
    printf("D5mRegs Updated\n");
}
void camera_hdmi_config() {
    int ret = 0;
    u16 R_01_value = 0x0036; //    set start row
    u16 R_02_value = 0x0010; //    set start column
    u16 R_03_value = 0x0437; //    set row size    1080[438] 400[190]
    u16 R_04_value = 0x0780; //    set column size 1920[780] 600[258]
    u16 R_05_value = 0x0000; //    H_Blanking
    u16 R_06_value = 0x0008; //    V_Blanking
    u16 R_0A_value = 0x8000; //    change latch
    u16 camera_exposer_value = 0x03e8; // should be less then R_03_value
    u16 R_10_value = 0x0051; //    set up PLL power on
    u16 R_11_value = 0x1409; //PLL_m_Factor<<8+PLL_n_Divider
    u16 R_12_value = 0x0001; // PLL_p1_Divider
    u16 R_1X_value = 0x0053; //    set USE PLL
    u16 R_20_value = 0xc040; //    Mirror Row and Columns
    u16 R_22_value = 0x0000; //    set row mode in bin mode
    u16 R_23_value = 0x0000; //    set column mode     in bin mode
    u16 R_2B_value = 0x0002; // 43[15] Green 1 Gain
    u16 R_2C_value = 0x0003; // 44[15] Blue Gain
    u16 R_2D_value = 0x0003; // 45[15] Red Gain
    u16 R_2E_value = 0x0002; // 46[15] Green 2 Gain
    u16 reset_h = 0x0001;
    u16 reset_l = 0x0000;
    u16 R_98_value = 0x0000; // 152[15] disble calibration
    u16 R_A0_value = 0x0000; // 160[15] Test pattern control
    u16 R_A1_value = 0x0000; // 161[15] Test green pattern value
    u16 R_A2_value = 0x0FFF; // 162[4095] Test red pattern value
    u16 R_49_value = 0x00A8; // 73[424] row black target
    u16 R_60_value = 0x0020; // Green1_Offset (RW)
    u16 R_61_value = 0x0020; // Green2_Offset (RW)
    u16 R_62_value = 0x0000; // Black_Level_Calibration (RW)
    u16 R_63_value = 0x0020; // Red_Offset (RW)
    u16 R_64_value = 0x0020; // Blue_Offset (RW)
    ret |= d5m_i2c_write(0x00D, reset_h);
    ret |= d5m_i2c_write(0x00D, reset_l);
    ret |= d5m_i2c_write(0x001, R_01_value);
    ret |= d5m_i2c_write(0x002, R_02_value);
    ret |= d5m_i2c_write(0x003, R_03_value);
    ret |= d5m_i2c_write(0x004, R_04_value);
    ret |= d5m_i2c_write(0x005, R_05_value);
    ret |= d5m_i2c_write(0x006, R_06_value);
    ret |= d5m_i2c_write(0x009, camera_exposer_value);
    ret |= d5m_i2c_write(0x00A, R_0A_value);
    ret |= d5m_i2c_write(0x020, R_20_value);
    ret |= d5m_i2c_write(0x022, R_22_value);
    ret |= d5m_i2c_write(0x023, R_23_value);
    ret |= d5m_i2c_write(0x02B, R_2B_value);
    ret |= d5m_i2c_write(0x02C, R_2C_value);
    ret |= d5m_i2c_write(0x02D, R_2D_value);
    ret |= d5m_i2c_write(0x02E, R_2E_value);
    ret |= d5m_i2c_write(0x060, R_60_value);
    ret |= d5m_i2c_write(0x061, R_61_value);
    ret |= d5m_i2c_write(0x062, R_62_value);
    ret |= d5m_i2c_write(0x063, R_63_value);
    ret |= d5m_i2c_write(0x064, R_64_value);
    ret |= d5m_i2c_write(0x049, R_49_value);
    ret |= d5m_i2c_write(0x010, R_10_value);
    ret |= d5m_i2c_write(0x011, R_11_value);
    ret |= d5m_i2c_write(0x012, R_12_value);
    usleep(100);
    ret |= d5m_i2c_write(0x010, R_1X_value);
    usleep(100);
    ret |= d5m_i2c_write(0x098, R_98_value);
    ret |= d5m_i2c_write(0x0A0, R_A0_value);
    ret |= d5m_i2c_write(0x0A1, R_A1_value);
    ret |= d5m_i2c_write(0x0A2, R_A2_value);
    //if (ret == 1)    print("All registers are transmitted to d5m\n");
    //else print("more bytes to transmit-- check error\n");
    //d5m_write_reg(D5M_BASE,REG4,0);
}
void camera_vga_config() {
    int ret = 0;
    u16 R_01_value = 0x0000; //    set start row @ 0
    u16 R_02_value = 0x0000; //    set start column @ 0
    u16 R_03_value = 0x03BF; //    set row size    480[1E0] height 03BF=959
    u16 R_04_value = 0x04FF; //    set column size 640[280] width  04FF=1279
    u16 R_05_value = 0x0000; //    H_Blanking
    u16 R_06_value = 0x0019; //    V_Blanking
    u16 R_0A_value = 0x8000; //    change latch
    u16 camera_exposer_value = 0x07D0;
    u16 R_10_value = 0x0051; //    set up PLL power on
    u16 R_11_value = 0x1807; //    PLL_m_Factor<<8+PLL_n_Divider
    u16 R_12_value = 0x0002; //    PLL_p1_Divider
    u16 R_1X_value = 0x0053; //    set USE PLL
    u16 R_20_value = 0xc000; //    Mirror Row and Columns
    u16 R_22_value = 0x0000; //    set row mode in bin mode
    u16 R_23_value = 0x0000; //    set column mode     in bin mode
    u16 R_2B_value = 0x0002; // 43[15] Green 1 Gain
    u16 R_2C_value = 0x0003; // 44[15] Blue Gain
    u16 R_2D_value = 0x0003; // 45[15] Red Gain
    u16 R_2E_value = 0x0002; // 46[15] Green 2 Gain
    u16 R_98_value = 0x0000; // 152[15] disble calibration
    u16 R_A0_value = 0x8000; // 160[15] Test pattern control
    u16 R_A1_value = 0x0000; // 161[15] Test green pattern value
    u16 R_A2_value = 0x0FFF; // 162[4095] Test red pattern value
    u16 R_49_value = 0x01A8; // 73[424] row black target
    ret |= d5m_i2c_write(0x001, R_01_value);
    ret |= d5m_i2c_write(0x002, R_02_value);
    ret |= d5m_i2c_write(0x003, R_03_value);
    ret |= d5m_i2c_write(0x004, R_04_value);
    ret |= d5m_i2c_write(0x005, R_05_value);
    ret |= d5m_i2c_write(0x006, R_06_value);
    ret |= d5m_i2c_write(0x009, camera_exposer_value);
    ret |= d5m_i2c_write(0x00A, R_0A_value);
    ret |= d5m_i2c_write(0x010, R_10_value);
    ret |= d5m_i2c_write(0x011, R_11_value);
    ret |= d5m_i2c_write(0x012, R_12_value);
    usleep(100);
    ret |= d5m_i2c_write(0x010, R_1X_value);
    usleep(100);
    ret |= d5m_i2c_write(0x020, R_20_value);
    ret |= d5m_i2c_write(0x022, R_22_value);
    ret |= d5m_i2c_write(0x023, R_23_value);
    ret |= d5m_i2c_write(0x02B, R_2B_value);
    ret |= d5m_i2c_write(0x02C, R_2C_value);
    ret |= d5m_i2c_write(0x02D, R_2D_value);
    ret |= d5m_i2c_write(0x02E, R_2E_value);
    ret |= d5m_i2c_write(0x049, R_49_value);
    ret |= d5m_i2c_write(0x098, R_98_value);
    ret |= d5m_i2c_write(0x0A0, R_A0_value);
    ret |= d5m_i2c_write(0x0A1, R_A1_value);
    ret |= d5m_i2c_write(0x0A2, R_A2_value);
    if (ret == 1) print("All registers are transmitted to d5m\n");
    else print("more bytes to transmit-- check error\n");
    //d5m_write_reg(D5M_BASE,REG4,7);
}
