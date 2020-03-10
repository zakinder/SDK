// LAST TESTED : 12/27/2019
#include "video_num_switch.h"
#include <stdio.h>
#include <xil_types.h>
#include "../SYSTEM_CONFIG_HEADER/system_config_defines.h"
#include "../UART/uartio.h"
#include "channel.h"
void video_num_switch(int ENABLE_MENU) {
    int video_num_switch_enable = ENABLE_MENU;
    unsigned int uart_io;
    u32 video_num_state = mainmenu;
    u32 substate_status;
    u32 status_reg;
    while (video_num_switch_enable == TRUE) {
        switch (video_num_state) {
        case mainmenu:
            cmds_menu();
            video_num_state = menu_select;
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
                video_num_state = uart_io;
                break;
            }
            break;
        case clear:
            /*****************************************************************************************************************/
            menu_cls();
            video_num_state = mainmenu;
            break;
            /*****************************************************************************************************************/
            //                                                  video_feature_select
            /*****************************************************************************************************************/
        case video0:
            /*****************************************************************************************************************/
            video_feature_select(selCgain);
            substate_status = enter_value_or_quit("video0", video0);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video1:
            /*****************************************************************************************************************/
            video_feature_select(selSharp);
            substate_status = enter_value_or_quit("video1", video1);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video2:
            /*****************************************************************************************************************/
            video_feature_select(selBlur);
            substate_status = enter_value_or_quit("video2", video2);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video3:
            /*****************************************************************************************************************/
            video_feature_select(selHsl);
            substate_status = enter_value_or_quit("video3", video3);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video4:
            /*****************************************************************************************************************/
            video_feature_select(selHsv);
            substate_status = enter_value_or_quit("video4", video4);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video5:
            /*****************************************************************************************************************/
            video_feature_select(selRgb);
            substate_status = enter_value_or_quit("video5", video5);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video6:
            /*****************************************************************************************************************/
            printf("Enter edgeThreshold Value\n");
            menu_print_prompt();
            status_reg = uart_prompt_io();
            if (status_reg == clear) {
                video_num_state = mainmenu;
                break;
            }
            edge_threshold_val(status_reg);
            status_reg = 0;
            printf("Enter edge_color 1-Enable Else Disabled\n");
            menu_print_prompt();
            status_reg = uart_prompt_io();
            if (status_reg == clear) {
                video_num_state = mainmenu;
                break;
            }
            edge_color(status_reg);
            video_feature_select(selSoble);
            substate_status = enter_value_or_quit("video6", video6);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video7:
            /*****************************************************************************************************************/
            video_feature_select(selEmboss);
            substate_status = enter_value_or_quit("video7", video7);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video8:
            /*****************************************************************************************************************/
            video_feature_select(selMaskSobelLum);
            substate_status = enter_value_or_quit("video8", video8);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video9:
            /*****************************************************************************************************************/
            video_feature_select(selMaskSobelTrm);
            substate_status = enter_value_or_quit("video9", video9);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video10:
            /*****************************************************************************************************************/
            video_feature_select(selMaskSobelRgb);
            substate_status = enter_value_or_quit("video10", video10);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video11:
            /*****************************************************************************************************************/
            video_feature_select(selMaskSobelShp);
            substate_status = enter_value_or_quit("video11", video11);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video12:
            /*****************************************************************************************************************/
            video_feature_select(selMaskSobelShp);
            substate_status = enter_value_or_quit("video12", video12);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video13:
            /*****************************************************************************************************************/
            video_feature_select(selMaskSobelBlu);
            substate_status = enter_value_or_quit("video13", video13);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video14:
            /*****************************************************************************************************************/
            video_feature_select(selMaskSobelYcc);
            substate_status = enter_value_or_quit("video14", video14);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video15:
            /*****************************************************************************************************************/
            video_feature_select(selMaskSobelHsv);
            substate_status = enter_value_or_quit("video15", video15);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video16:
            /*****************************************************************************************************************/
            video_feature_select(selMaskSobelHsl);
            substate_status = enter_value_or_quit("video16", video16);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video17:
            /*****************************************************************************************************************/
            video_feature_select(selMaskSobelCga);
            substate_status = enter_value_or_quit("video17", video17);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video18:
            /*****************************************************************************************************************/
            video_feature_select(selColorTrm);
            substate_status = enter_value_or_quit("video18", video18);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video19:
            /*****************************************************************************************************************/
            video_feature_select(selColorLmp);
            substate_status = enter_value_or_quit("video19", video19);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video20:
            /*****************************************************************************************************************/
            video_feature_select(selTPattern);
            substate_status = enter_value_or_quit("video20", video20);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video21:
            /*****************************************************************************************************************/
            cgain_coef();
            video_feature_select(selCgainToCgain);
            substate_status = enter_value_or_quit("video21", video21);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video22:
            /*****************************************************************************************************************/
            video_feature_select(selCgainToHsl);
            substate_status = enter_value_or_quit("video22", video22);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video23:
            /*****************************************************************************************************************/
            video_feature_select(selCgainToHsv);
            substate_status = enter_value_or_quit("video23", video23);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video24:
            /*****************************************************************************************************************/
            video_feature_select(selCgainToYcbcr);
            substate_status = enter_value_or_quit("video24", video24);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video25:
            /*****************************************************************************************************************/
            video_feature_select(selCgainToShp);
            substate_status = enter_value_or_quit("video25", video25);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video26:
            /*****************************************************************************************************************/
            video_feature_select(selCgainToBlu);
            substate_status = enter_value_or_quit("video26", video26);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video27:
            /*****************************************************************************************************************/
            video_feature_select(selShpToCgain);
            substate_status = enter_value_or_quit("video27", video27);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video28:
            /*****************************************************************************************************************/
            video_feature_select(selShpToHsl);
            substate_status = enter_value_or_quit("video28", video28);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video29:
            /*****************************************************************************************************************/
            video_feature_select(selShpToHsv);
            substate_status = enter_value_or_quit("video29", video29);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video30:
            /*****************************************************************************************************************/
            video_feature_select(selShpToYcbcr);
            substate_status = enter_value_or_quit("video30", video30);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video31:
            /*****************************************************************************************************************/
            video_feature_select(selShpToShp);
            substate_status = enter_value_or_quit("video31", video31);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video32:
            /*****************************************************************************************************************/
            video_feature_select(selShpToBlu);
            substate_status = enter_value_or_quit("video32", video32);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video33:
            /*****************************************************************************************************************/
            video_feature_select(selbluToBlu);
            substate_status = enter_value_or_quit("video33", video33);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video34:
            /*****************************************************************************************************************/
            video_feature_select(selbluToCga);
            substate_status = enter_value_or_quit("video34", video34);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video35:
            /*****************************************************************************************************************/
            video_feature_select(selbluToShp);
            substate_status = enter_value_or_quit("video35", video35);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video36:
            /*****************************************************************************************************************/
            video_feature_select(selbluToYcc);
            substate_status = enter_value_or_quit("video36", video36);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video37:
            /*****************************************************************************************************************/
            video_feature_select(selbluToHsv);
            substate_status = enter_value_or_quit("video37", video37);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video38:
            /*****************************************************************************************************************/
            video_feature_select(selbluToHsl);
            substate_status = enter_value_or_quit("video38", video38);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video39:
            /*****************************************************************************************************************/
            video_feature_select(selbluToCgaShp);
            substate_status = enter_value_or_quit("video39", video39);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video40:
            /*****************************************************************************************************************/
            video_feature_select(selbluToCgaShpYcc);
            substate_status = enter_value_or_quit("video40", video40);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video41:
            /*****************************************************************************************************************/
            cgain_coef();
            video_feature_select(selbluToCgaShpHsv);
            substate_status = enter_value_or_quit("video41", video41);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video42:
            /*****************************************************************************************************************/
            video_feature_select(selbluToShpCga);
            substate_status = enter_value_or_quit("video42", video42);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video43:
            /*****************************************************************************************************************/
            video_feature_select(selbluToShpCgaYcc);
            substate_status = enter_value_or_quit("video43", video43);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video44:
            /*****************************************************************************************************************/
            video_feature_select(selbluToShpCgaHsv);
            substate_status = enter_value_or_quit("video44", video44);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video45:
            /*****************************************************************************************************************/
            video_feature_select(selRgbCorrect);
            substate_status = enter_value_or_quit("video45", video45);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video46:
            /*****************************************************************************************************************/
            video_feature_select(selRgbRemix);
            substate_status = enter_value_or_quit("video46", video46);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video47:
            /*****************************************************************************************************************/
            video_feature_select(selRgbDetect);
            substate_status = enter_value_or_quit("video47", video47);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video48:
            /*****************************************************************************************************************/
            video_feature_select(selRgbPoi);
            substate_status = enter_value_or_quit("video48", video48);
            video_num_state = substate_status;
            break;
            /*****************************************************************************************************************/
        case video49:
            /*****************************************************************************************************************/
            video_feature_select(selYcbcr);
            substate_status = enter_value_or_quit("video49", video49);
            video_num_state = substate_status;
            break;
        case quit:
            video_num_switch_enable = FALSE;
            break;
        default:
            printf("\r\n");
            video_num_state = mainmenu;
            break;
        }
    }
    printf("Break\r\n");
    video_num_switch_enable = TRUE;
}
