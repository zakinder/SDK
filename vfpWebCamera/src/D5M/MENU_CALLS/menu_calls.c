// LAST TESTED : 3/10/2020
#include "menu_calls.h"
#include <stdio.h>
#include <xbasic_types.h>
#include <xil_io.h>
#include <xil_printf.h>
#include <xstatus.h>
#include "../HDMI_DISPLAY/hdmi_display.h"
#include "../I2C_D5M/i2c_d5m.h"
#include "../SDCARD/sdReadWrite.h"
#include "../SYSTEM_CONFIG_HEADER/system_config_defines.h"
#include "../SYSTEM_CONFIG_HEADER/system_config_header.h"
#include "../UART/uartio.h"
#include "../VIDEO_CHANNEL/channel.h"
#include "../VIDEO_FILTERS/filters.h"
d5m_i2c_reg d5m_i2c_ptr;
void menu_calls(int ENABLE_MENU) {
    int menu_calls_enable = ENABLE_MENU;
    unsigned int uart_io;
    u32 current_state = mainmenu;
    int ret;
    u32 cmd_status_value;
    u32 cmd_status_substate;
    //u32 address;
    u32 value;
    u32 temp1Register;
    u32 temp2Register;
    u32 t1Register;
    u32 t2Register;
    u16 ditherRgb;
    u16 vChannelIndex;
    u32 t3Register;
    u32 t4Register;
    u32 t5Register;
    u32 w_rh;
    u32 w_rl;
    u32 w_gh;
    u32 w_gl;
    u32 w_bh;
    u32 w_bl;
    //u32 th_set;
    int i;
    //int error_flag = 0;
    u32 system_time;
    int Status;
    //char chRegister[8] = { 0 };
    while (menu_calls_enable == TRUE) {
        switch (current_state) {
        case mainmenu:
            temp1Register = 0x00000000;
            temp2Register = 0x00000000;
            //char charReg[8] = { 0 };
            //exposer_compansate();
            cmds_menu();
            current_state = menu_select;
            break;
        case menu_select:
            uart_io = uart_prompt_io();
            if (uart_io == 0) {
                uart_Red_Text();
                printf("Unknown command entered %x\r\n", (unsigned) uart_io);
                printf("\r>>");
                uart_Default_Text();
                break;
            }
            else {
                uart_Default_Text();
                current_state = uart_io;
                break;
            }
            break;
        case clear:
            /*****************************************************************************************************************/
            menu_cls();
            current_state = mainmenu;
            break;
        case rgbselect:
            /*****************************************************************************************************************/
            printf("Enter 3(rgb1Set) 4(rgb2Set) 5(rgbSet) values for rgb\n");
            menu_print_prompt();
            vChannelIndex = uart_prompt_io();
            if (vChannelIndex == clear) {
                current_state = mainmenu;
                break;
            }
            raw_rgb_select(vChannelIndex);
            cmd_status_substate = enter_value_or_quit("rgbselect", rgbselect);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case vchannel:
            /*****************************************************************************************************************/
            printf("Enter 1-49 values for v channels\n");
            menu_print_prompt();
            vChannelIndex = uart_prompt_io();
            if (vChannelIndex == clear) {
                current_state = mainmenu;
                break;
            }
            video_feature_select(vChannelIndex);
            cmd_status_substate = enter_value_or_quit("vchannel", vchannel);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case ltselect:
            /*****************************************************************************************************************/
            printf(
                    "Enter 0 for ditherFilter else rgb Value for all vChannels\n");
            printf(
                    "Enter 1-3 for blur level Value and set vChannel = video40 \n");
            menu_print_prompt();
            ditherRgb = uart_prompt_io();
            if (ditherRgb == clear) {
                current_state = mainmenu;
                break;
            }
            l_th_select(ditherRgb);
            printf("Enter 1-49 values for v channels\n");
            menu_print_prompt();
            vChannelIndex = uart_prompt_io();
            if (vChannelIndex == clear) {
                current_state = mainmenu;
                break;
            }
            video_feature_select(vChannelIndex);
            cmd_status_substate = enter_value_or_quit("ltselect", ltselect);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case sdcard:
            /*****************************************************************************************************************/
            xil_printf("wr_fr_data \r\n");
            Status = write_image_frame();
            if (Status != XST_SUCCESS) {
                xil_printf("READ RAW DATA failed \r\n");
            }
            cmd_status_substate = enter_value_or_quit("sdcard", sdcard);
            current_state = cmd_status_substate;
            break;
        case wsd:
            /*****************************************************************************************************************/
            xil_printf("wr_fr_data \r\n");
            Status = wr_fr_data();
            if (Status != XST_SUCCESS) {
                xil_printf("READ RAW DATA failed \r\n");
            }
            cmd_status_substate = enter_value_or_quit("wsd", wsd);
            current_state = cmd_status_substate;
            break;
        case rsd:
            /*****************************************************************************************************************/
            xil_printf("wr_fr_data \r\n");
            Status = rd_data_sd();
            if (Status != XST_SUCCESS) {
                xil_printf("READ RAW DATA failed \r\n");
            }
            cmd_status_substate = enter_value_or_quit("rsd", rsd);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case wsdcard:
            /*****************************************************************************************************************/
            xil_printf("writeData \r\n");
            write_hd_data_sd();
//            printf("2nd Line chRegister Line \r\n");
//            printf("4: %s \r\n",chRegister);
//            menu_print_prompt();
//
//            char_to_uart(charReg);
//
//            printf("5: %s \r\n",charReg);
            cmd_status_substate = enter_value_or_quit("wsdcard", wsdcard);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case rsdcard:
            /*****************************************************************************************************************/
//            printf("1: %s \r\n",charReg);
//            menu_print_prompt();
//            char charRegister[8] = { 0 };
//            char_to_uart(charRegister);
//            printf("2: %s \r\n",charRegister);
//            printf("2nd Line \r\n");
//            menu_print_prompt();
//
//            char_to_uart(chRegister);
//            printf("3: %s \r\n",chRegister);
            //charRegister = char_to_uart(b);
            //printf("charRegister: %c \n",charRegister);
            printf("write_raw_frame_data Enter height\n");
            menu_print_prompt();
            temp1Register = uart_prompt_io();
            printf("Enter width\n");
            menu_print_prompt();
            temp2Register = uart_prompt_io();
            Status = write_raw_frame_data(temp1Register, temp1Register);
            if (Status != XST_SUCCESS) {
                xil_printf("READ RAW DATA failed \r\n");
            }
            cmd_status_substate = enter_value_or_quit("rsdcard", rsdcard);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case d5mgain:
            /*****************************************************************************************************************/
            d5m_gain_select();
            cmd_status_substate = enter_value_or_quit("d5mgain", d5mgain);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case keyarrow1:
            /*****************************************************************************************************************/
            key_arrow_1_select();
            cmd_status_substate = enter_value_or_quit("keyarrow", keyarrow);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case keyarrow2:
            /*****************************************************************************************************************/
            key_arrow_2_select();
            cmd_status_substate = enter_value_or_quit("keyarrow", keyarrow);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case keyarrow3:
            /*****************************************************************************************************************/
            key_arrow_3_select();
            cmd_status_substate = enter_value_or_quit("keyarrow", keyarrow);
            current_state = cmd_status_substate;
            break;
        case kernalconfig:
            /*****************************************************************************************************************/
            kernel_config(&pCf);
            cmd_status_substate = enter_value_or_quit("kernalconfig",
            kernalconfig);
            current_state = cmd_status_substate;
            break;
        case ycbcrenable:
            /*****************************************************************************************************************/
            ycbcr_enable();
            cmd_status_substate = enter_value_or_quit("ycbcrenable",
            ycbcrenable);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case ycbcrdisable:
            /*****************************************************************************************************************/
            ycbcr_disable();
            cmd_status_substate = enter_value_or_quit("ycbcrdisable",
            ycbcrdisable);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
            // WRITE KERNEL COEFs
            /*****************************************************************************************************************/
        case wsobel:
            /*****************************************************************************************************************/
            sobel_coef_update();
            video_feature_select(selSoble);
            cmd_status_substate = enter_value_or_quit("wsobel", wsobel);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case wprewit:
            /*****************************************************************************************************************/
            prewit_coef_update();
            video_feature_select(selSoble);
            cmd_status_substate = enter_value_or_quit("wprewit", wprewit);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case wsharp:
            /*****************************************************************************************************************/
            sharp_coef();
            video_feature_select(selSharp);
            cmd_status_substate = enter_value_or_quit("wsharp", wsharp);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case wemboss:
            /*****************************************************************************************************************/
            embos_coef();
            video_feature_select(selSharp);
            cmd_status_substate = enter_value_or_quit("wemboss", wemboss);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case wblur:
            /*****************************************************************************************************************/
            blur_coef();
            video_feature_select(selSharp);            //Note:selBlur
            cmd_status_substate = enter_value_or_quit("wblur", wblur);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case wcgain1:
            /*****************************************************************************************************************/
            cgain_1_coef();
            video_feature_select(selCgain);
            cmd_status_substate = enter_value_or_quit("wcgain1", wcgain1);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case wcgain2:
            /*****************************************************************************************************************/
            cgain_2_coef();
            video_feature_select(selCgain);
            cmd_status_substate = enter_value_or_quit("wcgain2", wcgain2);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case wcgain3:
            /*****************************************************************************************************************/
            cgain_3_coef();
            video_feature_select(selCgain);
            cmd_status_substate = enter_value_or_quit("wcgain3", wcgain3);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case wcgain4:
            /*****************************************************************************************************************/
            cgain_4_coef();
            video_feature_select(selCgain);
            cmd_status_substate = enter_value_or_quit("wcgain4", wcgain4);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case wcgain5:
            /*****************************************************************************************************************/
            cgain_5_coef();
            video_feature_select(selCgain);
            cmd_status_substate = enter_value_or_quit("wcgain5", wcgain5);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case wcgain6:
            /*****************************************************************************************************************/
            cgain_6_coef();
            video_feature_select(selCgain);
            cmd_status_substate = enter_value_or_quit("wcgain6", wcgain6);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case wcgain7:
            /*****************************************************************************************************************/
            cgain_7_coef();
            video_feature_select(selCgain);
            cmd_status_substate = enter_value_or_quit("wcgain7", wcgain7);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
            // CHECK FILTER
            /*****************************************************************************************************************/
        case sobel:
            /*****************************************************************************************************************/
            video_feature_select(selSoble);
            cmd_status_substate = enter_value_or_quit("sobel", sobel);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case rgb:
            /*****************************************************************************************************************/
            video_feature_select(selRgb);
            cmd_status_substate = enter_value_or_quit("rgb", rgb);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case sharp:
            /*****************************************************************************************************************/
            video_feature_select(selSharp);
            cmd_status_substate = enter_value_or_quit("sharp", sharp);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case blur:
            /*****************************************************************************************************************/
            video_feature_select(selBlur);
            cmd_status_substate = enter_value_or_quit("blur", blur);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case emboss:
            /*****************************************************************************************************************/
            video_feature_select(selEmboss);
            cmd_status_substate = enter_value_or_quit("emboss", emboss);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case hsv:
            /*****************************************************************************************************************/
            video_feature_select(selHsv);
            cmd_status_substate = enter_value_or_quit("hsv", hsv);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case hsl:
            /*****************************************************************************************************************/
            video_feature_select(selHsl);
            cmd_status_substate = enter_value_or_quit("hsl", hsl);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case keygain:
            /*****************************************************************************************************************/
            keyCgainConfigSelect();
            cmd_status_substate = enter_value_or_quit("keygain", keygain);
            current_state = cmd_status_substate;
            break;
        case rgbremix:
            /*****************************************************************************************************************/
            video_feature_select(selRgbRemix);
            cmd_status_substate = enter_value_or_quit("rgbremix", rgbremix);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case rgbdetect:
            /*****************************************************************************************************************/
            video_feature_select(selRgbDetect);
            cmd_status_substate = enter_value_or_quit("rgbdetect", rgbdetect);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case rgbpoi:
            /*****************************************************************************************************************/
            video_feature_select(selRgbPoi);
            cmd_status_substate = enter_value_or_quit("rgbpoi", rgbpoi);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case ycbcr:
            /*****************************************************************************************************************/
            video_feature_select(selYcbcr);
            cmd_status_substate = enter_value_or_quit("hsl", hsl);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case cgain:
            /*****************************************************************************************************************/
            video_feature_select(selCgain);
            cmd_status_substate = enter_value_or_quit("cgain", cgain);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case parttern1:
            /*****************************************************************************************************************/
            //video_feature_select(selTPatter1);
            cmd_status_substate = enter_value_or_quit("parttern1", parttern1);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case parttern2:
            /*****************************************************************************************************************/
            //video_feature_select(selTPatter2);
            cmd_status_substate = enter_value_or_quit("parttern2", parttern2);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case parttern3:
            /*****************************************************************************************************************/
            //video_feature_select(selTPatter3);
            cmd_status_substate = enter_value_or_quit("parttern3", parttern3);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case parttern4:
            /*****************************************************************************************************************/
            //video_feature_select(selTPatter4);
            cmd_status_substate = enter_value_or_quit("parttern4", parttern4);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case blur1x:
            /*****************************************************************************************************************/
            video_feature_select(wBlurv1);
            cmd_status_substate = enter_value_or_quit("blur1x", blur1x);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case blur2x:
            /*****************************************************************************************************************/
            video_feature_select(wBlurv2);
            cmd_status_substate = enter_value_or_quit("blur2x", blur2x);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case blur3x:
            /*****************************************************************************************************************/
            video_feature_select(wBlurv3);
            cmd_status_substate = enter_value_or_quit("blur3x", blur3x);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case blur4x:
            /*****************************************************************************************************************/
            video_feature_select(wBlurv4);
            cmd_status_substate = enter_value_or_quit("blur4x", blur4x);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case blur5x:
            /*****************************************************************************************************************/
            video_feature_select(wBlurv4);
            cmd_status_substate = enter_value_or_quit("blur5x", blur5x);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case prewitt:
            /*****************************************************************************************************************/
            video_feature_select(selSoble);
            cmd_status_substate = enter_value_or_quit("prewitt", prewitt);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case colorcorrection:
            /*****************************************************************************************************************/
            video_feature_select(selRgbCorrect);
            cmd_status_substate = enter_value_or_quit("colorcorrection",
            colorcorrection);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case xbright:
            /*****************************************************************************************************************/
            compute_brightness();
            printf("Compute Brightness[%i]\n", (unsigned) pvideo.brightness);
            cmd_status_substate = enter_value_or_quit("xbright", xbright);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case genimage:
            /*****************************************************************************************************************/
            buffer_vdma_hdmi(&pvideo);
            cmd_status_substate = enter_value_or_quit("genimage", genimage);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case zoom:
            /*****************************************************************************************************************/
            d5m_config3();
            camera_update();
            cmd_status_substate = enter_value_or_quit("zoom", zoom);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case d5mw_exposer:
            /*****************************************************************************************************************/
            cmd_status_value = enter_value_or_quit("null", d5mw_exposer);
            camera_exposer(cmd_status_value);
            cmd_status_substate = enter_value_or_quit("d5mw_exposer",
            d5mw_exposer);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case d5mw_testpattern:
            /*****************************************************************************************************************/
            cmd_status_value = enter_value_or_quit("null", d5mw_testpattern);
            config_d5m_gen_testpattern(cmd_status_value);
            cmd_status_substate = enter_value_or_quit("d5mw_testpattern",
            d5mw_testpattern);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case d5mw_colorgain:
            /*****************************************************************************************************************/
            d5m_color_gain();
            cmd_status_substate = enter_value_or_quit("d5mw_colorgain",
            d5mw_colorgain);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case d5mr_exposer:
            /*****************************************************************************************************************/
            read_exposer_register();
            cmd_status_substate = enter_value_or_quit("d5mr_exposer",
            d5mr_exposer);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case d5mw_regs:
            /*****************************************************************************************************************/
            d5m_i2c_write_reg();
            cmd_status_substate = enter_value_or_quit("d5mw_regs", d5mw_regs);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case d5mr_regs:
            /*****************************************************************************************************************/
            d5mr_reg();
            cmd_status_substate = enter_value_or_quit("d5mr_regs", d5mr_regs);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case cmds_displaytype:
            /*****************************************************************************************************************/
            cmd_status_value = enter_value_or_quit("null", cmds_displaytype);
            d5m_write_reg(D5M_BASE, w_videochannel_reg_5, cmd_status_value);
            cmd_status_substate = enter_value_or_quit("cmds_displaytype",
            cmds_displaytype);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case colorbars:
            /*****************************************************************************************************************/
            color_bars_vdma_hdmi(&pvideo);
            cmd_status_substate = enter_value_or_quit("colorbars", colorbars);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case vga:
            /*****************************************************************************************************************/
            d5m_config2();
            camera_update();
            cmd_status_substate = enter_value_or_quit("vga", vga);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case hdmi:
            /*****************************************************************************************************************/
            camera_hdmi_config();
            cmd_status_substate = enter_value_or_quit("hdmi", hdmi);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case fullhdmi:
            /*****************************************************************************************************************/
            d5m_i2c_write_reg();
            cmd_status_substate = enter_value_or_quit("fullhdmi", fullhdmi);
            current_state = cmd_status_substate;
            break;
        case hsvpervalue:
            /*****************************************************************************************************************/
            printf("Enter Hsv Per Value\n");
            printf("0-Hsv 1-H 2-S 3-V\n");
            menu_print_prompt();
            t1Register = uart_prompt_io();
            if (t1Register == clear) {
                current_state = mainmenu;
                break;
            }
            color_hsv_per_ch(t1Register);
            /*****************************************************************************************************************/
            cmd_status_substate = enter_value_or_quit("hsvpervalue",
            hsvpervalue);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case yccpervalue:
            /*****************************************************************************************************************/
            printf("Enter YCbCr Per Value\n");
            printf("0-YCbCr 1-Y 2-Cb 3-Cr\n");
            menu_print_prompt();
            t1Register = uart_prompt_io();
            if (t1Register == clear) {
                current_state = mainmenu;
                break;
            }
            color_ycbcr_per_ch(t1Register);
            /*****************************************************************************************************************/
            cmd_status_substate = enter_value_or_quit("yccpervalue",
            yccpervalue);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case lumvalue:
            /*****************************************************************************************************************/
            printf("Enter Lum Threshold Value\n");
            menu_print_prompt();
            t1Register = uart_prompt_io();
            if (t1Register == clear) {
                current_state = mainmenu;
                break;
            }
            lum_threshold_value(t1Register);
            /*****************************************************************************************************************/
            cmd_status_substate = enter_value_or_quit("lumvalue", lumvalue);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case edgethreshold:
            /*****************************************************************************************************************/
            cmd_status_value = enter_value_or_quit("null", edgethreshold);
            edge_threshold_val(cmd_status_value);
            cmd_status_substate = enter_value_or_quit("edgethreshold",
            edgethreshold);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case version:
            /*****************************************************************************************************************/
            cmd_status_substate = enter_value_or_quit("version", version);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case cmds_gridpoint:
            /*****************************************************************************************************************/
            printf("Enter x,y location point\n");
            cmd_status_value = enter_value_or_quit("null", cmds_gridpoint);
            point_interest(cmd_status_value);
            read_fifo();
            cmd_status_substate = enter_value_or_quit("cmds_gridpoint",
            cmds_gridpoint);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case cmds_griddelta:
            /*****************************************************************************************************************/
            printf("Enter x,y location point\n");
            menu_print_prompt();
            value = uart_prompt_io();
            point_interest(value);
            read_fifo();
            printf("Done with readFifov2\n");
            current_state = mainmenu;
            break;
            /*****************************************************************************************************************/
        case cmds_fifomode:
            /*****************************************************************************************************************/
            fifo_status();
            printf("Enter '1' enable_next_read\n");
            menu_print_prompt();
            value = uart_prompt_io();
            enable_next_read(value);
            current_state = mainmenu;
            break;
        case cmds_printpixel:
            /*****************************************************************************************************************/
            printf("Enter row Address\n");
            menu_print_prompt();
            temp1Register = uart_prompt_io();
            printf("Enter coloum Address\n");
            menu_print_prompt();
            temp2Register = uart_prompt_io();
            uart_io = 1;
            //int offset;
            u32 address = pvideo.video_address;
            if (uart_io == 1) {
                int x, y;
                for (y = 0; y < temp1Register; y++) {
                    for (x = 0; x < temp2Register * 2; x++) {
                        pvideo.pixelvalue = (Xil_In16(address) & 0xffff);
                        printf("address[%x]=%i\n", (unsigned) address,
                                (unsigned) pvideo.pixelvalue);
                        address = address + 0x2;
                    }
                }
            }
            cmd_status_substate = enter_value_or_quit("cmds_printpixel",
            cmds_printpixel);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case colorgain: // need to be decoded at terminal for colorgain*** and replace back to parttern5
            /*****************************************************************************************************************/
            printf("Enter Coef location[1-9] and 0 to quit\n");
            printf("\r>>");
            uart_io = uart_prompt_io();
            if (uart_io != 0) {
                if (uart_io == 1) {
                    printf("color_filter_a11 Gain Value\n");
                    menu_print_prompt();
                    value = uart_prompt_io();
                    color_filter_a11(value);
                    break;
                }
                if (uart_io == 2) {
                    printf("color_filter_a12 Gain Value\n");
                    menu_print_prompt();
                    value = uart_prompt_io();
                    color_filter_a12(value);
                    break;
                }
                if (uart_io == 3) {
                    printf("color_filter_a13 Gain Value\n");
                    menu_print_prompt();
                    value = uart_prompt_io();
                    color_filter_a13(value);
                    break;
                }
                if (uart_io == 4) {
                    printf("color_filter_a21 Gain Value\n");
                    menu_print_prompt();
                    value = uart_prompt_io();
                    color_filter_a21(value);
                    break;
                }
                if (uart_io == 5) {
                    printf("color_filter_a22 Gain Value\n");
                    menu_print_prompt();
                    value = uart_prompt_io();
                    color_filter_a22(value);
                    break;
                }
                if (uart_io == 6) {
                    printf("color_filter_a23 Gain Value\n");
                    menu_print_prompt();
                    value = uart_prompt_io();
                    color_filter_a23(value);
                    break;
                }
                if (uart_io == 7) {
                    printf("color_filter_a31 Gain Value\n");
                    menu_print_prompt();
                    value = uart_prompt_io();
                    color_filter_a31(value);
                    break;
                }
                if (uart_io == 8) {
                    printf("color_filter_a32 Gain Value\n");
                    menu_print_prompt();
                    value = uart_prompt_io();
                    color_filter_a32(value);
                    break;
                }
                if (uart_io == 9) {
                    printf("color_filter_a33 Gain Value\n");
                    menu_print_prompt();
                    value = uart_prompt_io();
                    color_filter_a33(value);
                    break;
                }
            }
            else {
                current_state = mainmenu;
                break;
            }
            /*****************************************************************************************************************/
        case cmds_readfifo:
            /*****************************************************************************************************************/
            fifo_status();
            printf("Enter grid_lock 0 is disable else enable\n");
            menu_print_prompt();
            value = uart_prompt_io();
            grid_lock(value);
            printf("Enter cpu_ack_go_again 0 is disable else enable\n");
            menu_print_prompt();
            value = uart_prompt_io();
            cpu_ack_go_again(value);
            fifo_status();
            printf("Enter cpuGridCont value for all read cmds_readfifo\n");
            menu_print_prompt();
            value = uart_prompt_io();
            for (i = 0; i < value; i++) {
                read_data_byte(i);
            }
            cmd_status_substate = enter_value_or_quit("cmds_readfifo",
            cmds_readfifo);
            current_state = cmd_status_substate;
            break;
//        case cmds_readfifo:
//            /*****************************************************************************************************************/
//          //frameReadData();
//            printf("keyArrowSelect test\n");
//            keyArrowSelect();
//            cmd_status_substate = enter_value_or_quit("cmds_readfifo",cmds_readfifo);current_state = cmd_status_substate;break;
            /*****************************************************************************************************************/
        case d5mw_update:
            /*****************************************************************************************************************/
            d5m_regs(&d5m_i2c_ptr);
            camera_read(&d5m_i2c_ptr);
            printf("CMD\n");
            printf("updated5m\n");
            printf("Enter Address and Value\n");
            menu_print_prompt();
            address = uart_prompt_io();
            if (address == cmds_quit || address == 0) {
                current_state = mainmenu;
                break;
            }
            menu_print_prompt();
            value = uart_prompt_io();
            if (value == cmds_quit || address == 0) {
                current_state = mainmenu;
                break;
            }
            printf("address[%x],value[%x]\n", (unsigned) address,
                    (unsigned) value);
            printf("Confirm Enter To Send Else Quit\n");
            printf("\r>>");
            uart_io = uart_prompt_io();
            if (uart_io == 0) {
                ret = d5m_i2c_write(address, value);
                if (!ret) {
                    printf("ERROR : XST_FAILURE: %d\n", ret);
                }
                camera_update();
                break;
            }
            else {
                current_state = mainmenu;
                break;
            }
            /*****************************************************************************************************************/
        case d5mw_config:
            /*****************************************************************************************************************/
            printf("Config Num\n");
            printf("Enter 1 for Config1\n");
            printf("Enter 2 for Config2 VGA_640x480p60\n");
            printf("Enter 3 for Config3 VGA_640x480p60 iZOOM_MODE\n");
            menu_print_prompt();
            value = uart_prompt_io();
            if (value == cmds_quit) {
                current_state = mainmenu;
                break;
            }
            printf("Config Value[%d]\n", (unsigned) value);
            printf("Confirm Enter To Send Else Quit\n");
            printf("\r>>");
            uart_io = uart_prompt_io();
            if (uart_io == 0) {
                if (value == 1) {
                    d5m_config1();
                    camera_update();
                    break;
                }
                if (value == 2) {
                    d5m_config2();
                    camera_update();
                    break;
                }
                if (value == 3) {
                    d5m_config3();
                    camera_update();
                    break;
                }
            }
            else {
                current_state = mainmenu;
                break;
            }
        case cmds_videochannel:
            /*****************************************************************************************************************/
            printf("Enter edge_type Value\n");
            printf("1-sobel 2-prewitt\n");
            menu_print_prompt();
            t1Register = uart_prompt_io();
            if (t1Register == clear) {
                current_state = mainmenu;
                break;
            }
            edge_type(t1Register);
            /*****************************************************************************************************************/
            printf("Enter edgeThreshold Value\n");
            menu_print_prompt();
            t2Register = uart_prompt_io();
            if (t2Register == clear) {
                current_state = mainmenu;
                break;
            }
            edge_threshold_val(t2Register);
            /*****************************************************************************************************************/
            printf("Enter ycbcr_select Value\n");
            menu_print_prompt();
            t3Register = uart_prompt_io();
            if (t3Register == clear) {
                current_state = mainmenu;
                break;
            }
            ycbcr_select(t3Register);
            /*****************************************************************************************************************/
            printf("Enter color_correction Value\n");
            menu_print_prompt();
            t4Register = uart_prompt_io();
            if (t4Register == clear) {
                current_state = mainmenu;
                break;
            }
            color_correction(t4Register);
            /*****************************************************************************************************************/
            printf("Enter videoType Value\n");
            menu_print_prompt();
            t5Register = uart_prompt_io();
            if (t5Register == clear) {
                current_state = mainmenu;
                break;
            }
            else {
                video_feature_select(t5Register);
                t1Register = 0x00000000;
                t2Register = 0x00000000;
                t3Register = 0x00000000;
                t4Register = 0x00000000;
                t5Register = 0x00000000;
                current_state = cmds_videochannel;
                break;
            }
            /*****************************************************************************************************************/
        case coord: //new case name: color_detect_range
            /*****************************************************************************************************************/
            printf("Enter w_rh value\n");
            menu_print_prompt();
            w_rh = uart_prompt_io();
            printf("Enter w_rl value\n");
            menu_print_prompt();
            w_rl = uart_prompt_io();
            printf("Enter w_gh value\n");
            menu_print_prompt();
            w_gh = uart_prompt_io();
            printf("Enter w_gl value\n");
            menu_print_prompt();
            w_gl = uart_prompt_io();
            printf("Enter w_bh value\n");
            menu_print_prompt();
            w_bh = uart_prompt_io();
            printf("Enter w_bl value\n");
            menu_print_prompt();
            w_bl = uart_prompt_io();
            //---------------------
            color_detect_range(w_rh, w_rl, w_gh, w_gl, w_bh, w_bl);
            //---------------------
            cmd_status_substate = enter_value_or_quit("coord", coord);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case threshold:
            /*****************************************************************************************************************/
            printf("Enter edgeThreshold Value\n");
            menu_print_prompt();
            t2Register = uart_prompt_io();
            if (t2Register == clear) {
                current_state = mainmenu;
                break;
            }
            edge_threshold_val(t2Register);
            cmd_status_substate = enter_value_or_quit("threshold", threshold);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case timex:
            /*****************************************************************************************************************/
            pvideo.sec = d5m_read_reg(D5M_BASE, r_sec_reg_60);
            pvideo.min = d5m_read_reg(D5M_BASE, r_min_reg_61);
            pvideo.hr = d5m_read_reg(D5M_BASE, r_hour_reg_62);
            printf("%d:%d:%d\n\r", (unsigned) pvideo.hr, (unsigned) pvideo.min,
                    (unsigned) pvideo.sec);
            system_time = (((0x0000ff & d5m_read_reg(D5M_BASE, r_hour_reg_62))
                    << 16)
                    | ((d5m_read_reg(D5M_BASE,r_min_reg_61) & 0x0000ff) << 8)
                    | (0x0000ff & d5m_read_reg(D5M_BASE, r_sec_reg_60)));
            pvideo.time = (((0x0000ff & d5m_read_reg(D5M_BASE, r_hour_reg_62))
                    << 16)
                    | ((d5m_read_reg(D5M_BASE,r_min_reg_61) & 0x0000ff) << 8)
                    | (0x0000ff & d5m_read_reg(D5M_BASE, r_sec_reg_60)));
            uart_Yellow_Text();
            uart_Default_Text();
            cmd_status_substate = enter_value_or_quit("timex", timex);
            current_state = cmd_status_substate;
            break;
            /*****************************************************************************************************************/
        case quit:
            menu_calls_enable = FALSE;
            break;
        case cmds_uart:
            uart_value();
            current_state = uart_cmd(mainmenu, cmds_uart);
            break;
        default:
            printf("\r\n");
            current_state = mainmenu;
            break;
        }
    }
    printf("Break\r\n");
    menu_calls_enable = TRUE;
}
