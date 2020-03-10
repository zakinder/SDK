// LAST TESTED : 3/10/2020

#include "I2C_D5M/i2c_d5m.h"
#include "VIDEO_CHANNEL/channel.h"
#include "VIDEO_FILTERS/filters.h"

//Set and write all filters kernal coefficients and system default configurable registers
void d5m() {
    //Write d5m camera config values on i2c bus
    camera_hdmi_config();
    //Set All Filters Kernal Coefficients
    kernel_init();
    //Set vdma and hdmi default configurable registers
    vdma_hdmi_config();
    //Set RgbLevel Registers
    vfp_rgb_level_config();
    //Set System Default Configurable Registers
    d5m_init();
    //Write System Configurable Registers to Slave devices
    write_to_vfp();
    //Initialize Vdma to hdmi
    init_vdma_hdmi();
}
