// LAST TESTED : 09/08/2019
#include "channel.h"
#include <sleep.h>
#include <stdio.h>
#include <xbasic_types.h>
#include <xil_io.h>
#include <xil_types.h>
#include <xparameters.h>
#include "../HDMI_DISPLAY/hdmi_display.h"
#include "../I2C_D5M/i2c_d5m.h"
#include "../MENU_CALLS/menu_calls.h"
#include "../SYSTEM_CONFIG_HEADER/system_config_defines.h"
void d5mInit(){
    //DEFAULT CONFIGURABLE
    pvideo.uBaseAddr_IIC_HdmiOut        = XPAR_HDMI_OUTPUT_HDMI_IIC_BASEADDR;
    pvideo.uDeviceId_VTC_HdmioGenerator = XPAR_VIDEO_PIPELINE_TIMMING_CONTROLELR_DEVICE_ID;
    pvideo.uDeviceId_VDMA_HdmiDisplay   = XPAR_AXIVDMA_0_DEVICE_ID;
    pvideo.video_address                = VIDEO_BASEADDR0;
    pvideo.uNumFrames_HdmiDisplay       = XPAR_AXIVDMA_0_NUM_FSTORES;
    pvideo.hdmio_resolution             = VIDEO_RESOLUTION_1080P;
    pvideo.hdmio_width_Stride           = SCREEN_WIDTH_HORIZONTAL*2;
    pvideo.hdmio_height                 = SCREEN_HEIGHT_VERTICAL;
    pvideo.time = (((0x0000ff & D5M_mReadReg(D5M_BASE,r_hour_reg_62))<<16)|((D5M_mReadReg(D5M_BASE,r_min_reg_61) & 0x0000ff)<<8)|(0x0000ff & D5M_mReadReg(D5M_BASE,r_sec_reg_60)));
    pvideo.exposer           = 0;
    pvideo.brightness        = 0;
    pvideo.pixelvalue        = 0;
    pvideo.sec               = D5M_mReadReg(D5M_BASE,r_sec_reg_60);
    pvideo.min               = D5M_mReadReg(D5M_BASE,r_min_reg_61);
    pvideo.hr                = D5M_mReadReg(D5M_BASE,r_hour_reg_62);
    pvideo.fRgbCoordRL       = 0x001E;//1E=30
    pvideo.fRgbCoordRH       = 0x00E6;//E6=230
    pvideo.fRgbCoordGL       = 0x001E;//1E=30
    pvideo.fRgbCoordGH       = 0x0096;//E6=230
    pvideo.fRgbCoordBL       = 0x001E;//1E=30
    pvideo.fRgbCoordBH       = 0x0096;//96=150
    //DEFAULT ALREADY CONFIG
    pStream.fDbusSelect      = 0x0004;
    pStream.fThreshold       = 0x0017;
    pStream.fVideoType       = wSobelRgbDetect;
    pStream.fRgbCoordRL      = 0x001E;//1E=30
    pStream.fRgbCoordRH      = 0x00E6;//E6=230
    pStream.fRgbCoordGL      = 0x001E;//1E=30
    pStream.fRgbCoordGH      = 0x0096;//E6=230
    pStream.fRgbCoordBL      = 0x001E;//1E=30
    pStream.fRgbCoordBH      = 0x0096;//96=150
    pStream.fPointInterest   = 0x0064;
    pStream.fFifoFixedValue  = 0x0004;
    pStream.d5mExposer       = 0x0190;//400
    pStream.d5mBrightness    = 0x0078;//120
    //  GX
    //  [-1 +0 +1]
    //  [-2 +0 +2]
    //  [-1 +0 +1]
    //  GY
    //  [+1 +2 +1]
    //  [+0 +0 +0]
    //  [-1 -2 -1]
                       //  GY  GX
    psobel.K1 = 0x01FF;//  +1  -1
    psobel.K2 = 0x0200;//  +2  +0
    psobel.K3 = 0x0101;//  +1  -1
    psobel.K4 = 0x00FE;//  +0  -2
    psobel.K5 = 0x0000;//  +0  +0
    psobel.K6 = 0x0002;//  +0  +2
    psobel.K7 = 0xFFFF;//  -1  -1
    psobel.K8 = 0xFE00;//  -2  +0
    psobel.K9 = 0xFF01;//  -1  +1
    psobel.Kc = 0x0001;
    //  GX
    //  [+1 +0 -1]
    //  [+1 +0 -1]
    //  [+1 +0 -1]
    //  GY
    //  [+1 +1 +1]
    //  [+0 +0 +0]
    //  [-1 -1 -1]
                        //  GY  GX
    pprewitt.K1 = 0x01FF;//  +1  -1
    pprewitt.K2 = 0x0100;//  +1  +0
    pprewitt.K3 = 0x0101;//  +1  -1
    pprewitt.K4 = 0x00FF;//  +0  -1
    pprewitt.K5 = 0x0000;//  +0  +0
    pprewitt.K6 = 0x0001;//  +0  -1
    pprewitt.K7 = 0xFFFF;//  -1  -1
    pprewitt.K8 = 0xFF00;//  -1  +0
    pprewitt.K9 = 0xFF01;//  -1  +1
    pprewitt.Kc = 0x0001;
    pcolor.K1   = 0x000B;//   1.375 0000 1011 4:-3 0000 1=1  011=0 + 2^2 + 2^4 = (0.25) + 0.125 = 0.375 ==1.375
    pcolor.K2   = 0x00FE;//  -0.250 1111 1110
    pcolor.K3   = 0x00FF;//  -0.125 1111 1111
    pcolor.K4   = 0x00FF;//  -0.125
    pcolor.K5   = 0x000B;//   1.375
    pcolor.K6   = 0x00FE;//  -0.250
    pcolor.K7   = 0x00FE;//  -0.250
    pcolor.K8   = 0x00FF;//  -0.125
    pcolor.K9   = 0x000B;//   1.375
    pcolor.Kc   = 0x0001;//1 for write and read else read time
    }
void d5mInitCall(){
    //buffer_vdma_hdmi(&pvideo);
    //colorBars_vdma_hdmi(&pvideo);
    //bars(&pvideo);
    videoFeatureSelect(pStream.fVideoType);
    colorFilterFixed();
    d5m_vdma_hdmi(&pvideo);
    VdmaInit();
    selected_channel();
    pointInterestFixed();
    //d5mtestpattern(0x0004);
    //exposerCompansate();
}
u16 rColorFilterA11()
{
    return D5M_mReadReg(D5M_BASE,w_a11fl_reg_21);
}
u16 rColorFilterA12()
{
    return D5M_mReadReg(D5M_BASE,w_a12fl_reg_22);
}
u16 rColorFilterA13()
{
    return D5M_mReadReg(D5M_BASE,w_a13fl_reg_23);
}
u16 rColorFilterA21()
{
    return D5M_mReadReg(D5M_BASE,w_a21fl_reg_24);
}
u16 rColorFilterA22()
{
    return D5M_mReadReg(D5M_BASE,w_a22fl_reg_25);
}
u16 rColorFilterA23()
{
    return D5M_mReadReg(D5M_BASE,w_a23fl_reg_26);
}
u16 rColorFilterA31()
{
    return D5M_mReadReg(D5M_BASE,w_a31fl_reg_27);
}
u16 rColorFilterA32()
{
    return D5M_mReadReg(D5M_BASE,w_a32fl_reg_28);
}
u16 rColorFilterA33()
{
    return D5M_mReadReg(D5M_BASE,w_a33fl_reg_29);
}
void d5mInitPrint(){
    printf("Firmware Revision Number %x\n\r",(unsigned)D5M_mReadReg(D5M_BASE,r_revisionnumber_reg_63));
	printf("Current Time\n\r");
	//printf("%d:%d:%d\n\r",(unsigned) pvideo.hr,(unsigned) pvideo.min,(unsigned) pvideo.sec);
	printf("Color Filters Kernels Time\n\r");
    colorFilterKc(pcolor.Kc);//writeRead ColorFilter
    printf("%d:%d:%d\n\r",(unsigned) rColorFilterA11,(unsigned) rColorFilterA12,(unsigned) rColorFilterA13);
    printf("%d:%d:%d\n\r",(unsigned) rColorFilterA21,(unsigned) rColorFilterA22,(unsigned) rColorFilterA23);
    printf("%d:%d:%d\n\r",(unsigned) rColorFilterA31,(unsigned) rColorFilterA32,(unsigned) rColorFilterA33);
    colorFilterKc(0x0000);//open else read time
	printf("Edge Kernels Time\n\r");
    printf("%d:%d:%d\n\r",(unsigned) D5M_mReadReg(D5M_BASE,w_kernal_1_reg_08),(unsigned) D5M_mReadReg(D5M_BASE,w_kernal_2_reg_09),(unsigned) D5M_mReadReg(D5M_BASE,w_kernal_3_reg_10));
    printf("%d:%d:%d\n\r",(unsigned) D5M_mReadReg(D5M_BASE,w_kernal_4_reg_11),(unsigned) D5M_mReadReg(D5M_BASE,w_kernal_5_reg_12),(unsigned) D5M_mReadReg(D5M_BASE,w_kernal_6_reg_13));
    printf("%d:%d:%d\n\r",(unsigned) D5M_mReadReg(D5M_BASE,w_kernal_7_reg_14),(unsigned) D5M_mReadReg(D5M_BASE,w_kernal_8_reg_15),(unsigned) D5M_mReadReg(D5M_BASE,w_kernal_9_reg_16));
	printf("w_kSet_reg_17\n\r");
    printf("%d\n\r",(unsigned) D5M_mReadReg(D5M_BASE,w_kSet_reg_17));
}
void edgeThresholdVal(u16 thresholdValue)
{
    D5M_mWriteReg(D5M_BASE,w_threshold_reg_4,thresholdValue);
}
void videoFeatureSelect(u16 videoType)
{
    D5M_mWriteReg(D5M_BASE,w_videochannel_reg_5,videoType);
}
void ycbcrSelect(u16 videoType)
{
    D5M_mWriteReg(D5M_BASE,w_dchannel_reg_6,videoType);
}
void edgeColor(u16 edgeColorVal)
{
    D5M_mWriteReg(D5M_BASE,w_oEdgeType_reg_1,edgeColorVal);
}
void ycbcr_Enable()
{
    D5M_mWriteReg(D5M_BASE,w_dchannel_reg_6,0x0000);
}
void ycbcr_Disable()
{
    D5M_mWriteReg(D5M_BASE,w_dchannel_reg_6,0x0001);
}
void color_correction(u16 videoType)
{
    D5M_mWriteReg(D5M_BASE,w_cchannel_reg_7,videoType);
}
void point_Interest(u16 videoType)
{
    D5M_mWriteReg(D5M_BASE,w_pointinterest_reg_31,videoType);
    pStream.fPointInterest = D5M_mReadReg(D5M_BASE,r_pointinterest_reg_31);
    printf("pStream.fPointInterest %d\n\r",(unsigned) (pStream.fPointInterest));
}
void pointInterestFixed()
{
    D5M_mWriteReg(D5M_BASE,w_pointinterest_reg_31,pStream.fPointInterest);
}
void colorHsvPerCh(u16 PerChValue)
{
    if (PerChValue == 1) {
    	colorHsvPerChH();
    }else if (PerChValue == 2){
    	colorHsvPerChS();
    }else if (PerChValue == 3){  
    	colorHsvPerChV();
    }else{
    	colorHsvPerChHsv();
    }
}
void colorHsvPerChHsv()
{
    D5M_mWriteReg(D5M_BASE,w_hsvPerCh_reg_57,hsvPerChHsv);
}
void colorHsvPerChH()
{
    D5M_mWriteReg(D5M_BASE,w_hsvPerCh_reg_57,hsvPerChH);
}
void colorHsvPerChS()
{
    D5M_mWriteReg(D5M_BASE,w_hsvPerCh_reg_57,hsvPerChS);
}
void colorHsvPerChV()
{
    D5M_mWriteReg(D5M_BASE,w_hsvPerCh_reg_57,hsvPerChV);
}
void colorYCbCrPerCh(u16 PerChValue)
{
    if (PerChValue == 1) {
    	colorYCbCrPerChY();
    }else if (PerChValue == 2){
    	colorYCbCrPerChCb();
    }else if (PerChValue == 3){  
    	colorYCbCrPerChCr();
    }else{
    	colorYCbCrCh();
    }
}
void colorYCbCrCh()
{
    D5M_mWriteReg(D5M_BASE,w_yccPerCh_reg_58,YccPerChYCbCr);
}
void colorYCbCrPerChY()
{
    D5M_mWriteReg(D5M_BASE,w_yccPerCh_reg_58,YccPerChY);
}
void colorYCbCrPerChCb()
{
    D5M_mWriteReg(D5M_BASE,w_yccPerCh_reg_58,YccPerChCb);
}
void colorYCbCrPerChCr()
{
    D5M_mWriteReg(D5M_BASE,w_yccPerCh_reg_58,YccPerChCr);
}
void lum_ThresholdValue(u16 lumThresholdValue)
{
    D5M_mWriteReg(D5M_BASE,w_lumTh_reg_56,lumThresholdValue);
}
void ReadDataByte(u16 Value)
{
	D5M_mWriteReg(D5M_BASE,w_gridlockaddress_reg_36,(Value | 0x00010000));
    u16 AdrValue = D5M_mReadReg(D5M_BASE,r_readaddress_reg_36);
    printf("%d %d %d %d\n\r",(unsigned) AdrValue ,(unsigned) ((D5M_mReadReg(D5M_BASE,r_gridlockdatai_reg_38)) & 0xff),((unsigned) ((D5M_mReadReg(D5M_BASE,r_gridlockdatai_reg_38)) & 0xff00)>>8),(unsigned) ((D5M_mReadReg(D5M_BASE,r_gridlockdatai_reg_38)) & 0xff0000)>>16);
}
void fifoStatus()
{
    pvideo.fifoEmptyh  = D5M_mReadReg(D5M_BASE,r_fifoemptyh_reg_40);
    pvideo.fifoFullh   = D5M_mReadReg(D5M_BASE,r_fifofullh_reg_39);
    pvideo.cpuGridCont = D5M_mReadReg(D5M_BASE,r_cpugridcont_reg_42);
    printf("pvideo.fifoEmptyh %d\n\r",(unsigned) (pvideo.fifoEmptyh));
    printf("pvideo.fifoFullh %d\n\r",(unsigned) (pvideo.fifoFullh));
    printf("pvideo.cpuGridCont %d\n\r",(unsigned) (pvideo.cpuGridCont));
}
void readGDataContinueMode()
{
    D5M_mWriteReg(D5M_BASE,w_cpuwgridlock_reg_34,1);
    pvideo.fifoFullh   = D5M_mReadReg(D5M_BASE,r_fifofullh_reg_39);
    if (pvideo.fifoFullh == 0x1) 
    {
        readGData(0x1);
    }
    else
    {
        printf("pvideo.fifoEmptyh %d\n\r",(unsigned) (pvideo.fifoEmptyh));
        printf("pvideo.fifoFullh %d\n\r",(unsigned) (pvideo.fifoFullh));
        printf("pvideo.cpuGridCont %d\n\r",(unsigned) (pvideo.cpuGridCont));
    }
}
void readGDataStopMode()
{
    gridlock(0x1);
    pvideo.fifoFullh   = D5M_mReadReg(D5M_BASE,r_fifofullh_reg_39);
    if (pvideo.fifoFullh == 0x1) {
        readGData(0x0);
    }else{
    printf("pvideo.fifoEmptyh %d\n\r",(unsigned) (pvideo.fifoEmptyh));
    printf("pvideo.fifoFullh %d\n\r",(unsigned) (pvideo.fifoFullh));
    printf("pvideo.cpuGridCont %d\n\r",(unsigned) (pvideo.cpuGridCont));
    }
}
void readGData(u16 edgeTypeValue)
{
    pStream.fMAXgridcont  = D5M_mReadReg(D5M_BASE,r_cpugridcont_reg_42);
    Xuint16 y;
      for ( y = 0; y < pStream.fMAXgridcont; y++ )
      {
          D5M_mWriteReg(D5M_BASE,w_gridlockaddress_reg_36,(y | 0x00010000));
            printf("%i - %d %d %d\n\r",(unsigned) y,(unsigned) ((D5M_mReadReg(D5M_BASE,r_gridlockdatai_reg_38)) & 0xff),((unsigned) ((D5M_mReadReg(D5M_BASE,r_gridlockdatai_reg_38)) & 0xff00)>>8),(unsigned) ((D5M_mReadReg(D5M_BASE,r_gridlockdatai_reg_38)) & 0xff0000)>>16);
      }
      pvideo.fifoEmptyh  = D5M_mReadReg(D5M_BASE,r_fifoemptyh_reg_40);
      if (pvideo.fifoEmptyh == 0x1){
    	  printf("fifoEmptyh was Empty so set to w_cpuackgoagain_reg_33\n\r");
          cpuackgoagain(edgeTypeValue);
      }
}
void cpuackgoagain(u16 valueIn)
{
    D5M_mWriteReg(D5M_BASE,w_cpuackgoagain_reg_33,valueIn);
}
void gridlock(u16 valueIn)
{
	D5M_mWriteReg(D5M_BASE,w_cpuwgridlock_reg_34,valueIn);
}
void enState()
{
    D5M_mWriteReg(D5M_BASE,w_cpuwgridlock_reg_34,0x1);//enState
    D5M_mWriteReg(D5M_BASE,w_cpuwgridlock_reg_34,0x0);//disableState
    usleep(1);
}
void setState()
{
    pvideo.fifoEmptyh  = D5M_mReadReg(D5M_BASE,r_fifoemptyh_reg_40);
    D5M_mWriteReg(D5M_BASE,w_gridlockaddress_reg_36,(0x00000000));//disable read Enable.set address to 0
    D5M_mWriteReg(D5M_BASE,w_cpuackgoagain_reg_33,pvideo.fifoEmptyh);
}
/*****************************************************************************************************************/
void readFifo()
{
    int i = 0;
    enState();
    pvideo.fifoFullh   = D5M_mReadReg(D5M_BASE,r_fifofullh_reg_39);
    if (pvideo.fifoFullh == 0x1) {
        pvideo.cpuGridCont = D5M_mReadReg(D5M_BASE,r_cpugridcont_reg_42);
        printf("Address Red Green Blue\n\r");
        while ((D5M_mReadReg(D5M_BASE,r_fifoemptyh_reg_40)) != 0x1)
        {
          ReadDataByte(i);
          i++;
        }
        setState();//autoset on
        printf("pvideo.fifoEmptyh %d\n\r",(unsigned) (pvideo.fifoEmptyh));
        printf("pvideo.fifoEmptyh %d\n\r",(unsigned) (pvideo.fifoEmptyh));
        printf("pvideo.cpuGridCont %d\n\r",(unsigned) (pvideo.cpuGridCont));
    }
}
void enableNextRead(u16 eValue)
{
  D5M_mWriteReg(D5M_BASE,w_cpuackgoagain_reg_33,eValue);	
}
/*****************************************************************************************************************/
void edgeType(u16 edgeTypeValue)
{
    if (edgeTypeValue == 1) {
    	SobelCoefsUpdate();
    }else{
    	PrewitCoefsUpdate();
    }
}
void selected_channel()
{
    // 0 - RGB Video
    // 1 - RGB-Vertical PATTERN
    // 2 - RGB-Horizonat PATTERN
    // 0 - Edge BLACK WHITE
    // 1 - Video-EDGE ALONG RGB
    // 2 - Detector-Video
    // 3 - Sharp Video
    // 4 - normal Video
    //    u16 RedRange =0x0000;
    //    u16 GreenRange =0x0000;
    //    u16 BlueRange =0x0000;
    D5M_mWriteReg(D5M_BASE,w_rh_reg_50,pStream.fRgbCoordRL);//cord
    D5M_mWriteReg(D5M_BASE,w_rl_reg_51,pStream.fRgbCoordRH);//cord
    D5M_mWriteReg(D5M_BASE,w_gh_reg_52,pStream.fRgbCoordGL);//cord
    D5M_mWriteReg(D5M_BASE,w_gl_reg_53,pStream.fRgbCoordGH);//cord
    D5M_mWriteReg(D5M_BASE,w_bh_reg_54,pStream.fRgbCoordBL);//cord
    D5M_mWriteReg(D5M_BASE,w_bl_reg_55,pStream.fRgbCoordBH);//cord
    D5M_mWriteReg(D5M_BASE,w_abusselect_reg_3,pStream.fDbusSelect);
    edgeThresholdVal(pStream.fThreshold);
    //prewittWrite();
    videoFeatureSelect(pStream.fVideoType);
}
void colorDetectRange(u16 fRgbCoordRL,u16 fRgbCoordRH,u16 fRgbCoordGL,u16 fRgbCoordGH,u16 fRgbCoordBL,u16 fRgbCoordBH)
{
    D5M_mWriteReg(D5M_BASE,w_rh_reg_50,fRgbCoordRL);//cord
    D5M_mWriteReg(D5M_BASE,w_rl_reg_51,fRgbCoordRH);//cord
    D5M_mWriteReg(D5M_BASE,w_gh_reg_52,fRgbCoordGL);//cord
    D5M_mWriteReg(D5M_BASE,w_gl_reg_53,fRgbCoordGH);//cord
    D5M_mWriteReg(D5M_BASE,w_bh_reg_54,fRgbCoordBL);//cord
    D5M_mWriteReg(D5M_BASE,w_bl_reg_55,fRgbCoordBH);//cord
}
/****************************************************************
KERNEL COEF UPDATES
*****************************************************************/
//SOBEL
void SobelCoefsUpdate()
{
    SobelCoefX();
    usleep(1);
    SobelCoefY();
}
void SobelCoefX()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoefXSobel_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoefXSobel_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoefXSobel_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoefXSobel_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoefXSobel_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoefXSobel_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoefXSobel_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoefXSobel_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoefXSobel_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoefXSobel_kSet);
}
void SobelCoefY()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoefYSobel_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoefYSobel_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoefYSobel_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoefYSobel_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoefYSobel_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoefYSobel_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoefYSobel_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoefYSobel_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoefYSobel_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoefYSobel_kSet);
}
//PREWIT
void PrewitCoefsUpdate()
{
    PrewitCoefX();
    usleep(1);
    PrewitCoefY();
}
void PrewitCoefX()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoefXPrewit_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoefXPrewit_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoefXPrewit_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoefXPrewit_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoefXPrewit_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoefXPrewit_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoefXPrewit_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoefXPrewit_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoefXPrewit_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoefXPrewit_kSet);
}
void PrewitCoefY()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoefYPrewit_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoefYPrewit_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoefYPrewit_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoefYPrewit_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoefYPrewit_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoefYPrewit_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoefYPrewit_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoefYPrewit_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoefYPrewit_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoefYPrewit_kSet);
}
//EMBOS
void EmbosCoef()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoeffEmbos_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoeffEmbos_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoeffEmbos_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoeffEmbos_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoeffEmbos_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoeffEmbos_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoeffEmbos_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoeffEmbos_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoeffEmbos_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoeffSharp_kSet);
    usleep(1);
}
//CGAIN
void CgainCoef()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoef1Cgain_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoef1Cgain_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoef1Cgain_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoef1Cgain_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoef1Cgain_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoef1Cgain_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoef1Cgain_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoef1Cgain_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoef1Cgain_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoef2Cgain_kSet);
    usleep(1);
}
//CGAIN
void Cgain1Coef()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoef1Cgain_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoef1Cgain_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoef1Cgain_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoef1Cgain_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoef1Cgain_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoef1Cgain_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoef1Cgain_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoef1Cgain_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoef1Cgain_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoeffCgain_kSet);
    usleep(1);
}
//CGAIN
void Cgain2Coef()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoef2Cgain_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoef2Cgain_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoef2Cgain_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoef2Cgain_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoef2Cgain_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoef2Cgain_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoef2Cgain_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoef2Cgain_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoef2Cgain_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoeffCgain_kSet);
    usleep(1);
}
void Cgain3Coef()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoef3Cgain_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoef3Cgain_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoef3Cgain_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoef3Cgain_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoef3Cgain_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoef3Cgain_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoef3Cgain_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoef3Cgain_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoef3Cgain_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoeffCgain_kSet);
    usleep(1);
}
void Cgain4Coef()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoef4Cgain_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoef4Cgain_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoef4Cgain_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoef4Cgain_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoef4Cgain_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoef4Cgain_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoef4Cgain_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoef4Cgain_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoef4Cgain_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoeffCgain_kSet);
    usleep(1);
}
void Cgain5Coef()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoef5Cgain_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoef5Cgain_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoef5Cgain_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoef5Cgain_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoef5Cgain_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoef5Cgain_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoef5Cgain_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoef5Cgain_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoef5Cgain_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoeffCgain_kSet);
    usleep(1);
}
void Cgain6Coef()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoef6Cgain_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoef6Cgain_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoef6Cgain_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoef6Cgain_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoef6Cgain_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoef6Cgain_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoef6Cgain_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoef6Cgain_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoef6Cgain_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoeffCgain_kSet);
    usleep(1);
}
void Cgain7Coef()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoef7Cgain_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoef7Cgain_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoef7Cgain_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoef7Cgain_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoef7Cgain_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoef7Cgain_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoef7Cgain_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoef7Cgain_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoef7Cgain_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoeffCgain_kSet);
    usleep(1);
}
//YCBCR
void YcbcrCoef()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoeffYcbcr_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoeffYcbcr_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoeffYcbcr_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoeffYcbcr_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoeffYcbcr_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoeffYcbcr_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoeffYcbcr_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoeffYcbcr_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoeffYcbcr_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoeffYcbcr_kSet);
    usleep(1);
}
//SHARP
void SharpCoef()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoeffSharp_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoeffSharp_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoeffSharp_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoeffSharp_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoeffSharp_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoeffSharp_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoeffSharp_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoeffSharp_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoeffSharp_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoeffSharp_kSet);
    usleep(1);
}
//BLURE
void BlureCoef()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,kCoefVals_kCoeffBlure_k1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,kCoefVals_kCoeffBlure_k2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,kCoefVals_kCoeffBlure_k3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,kCoefVals_kCoeffBlure_k4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,kCoefVals_kCoeffBlure_k5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,kCoefVals_kCoeffBlure_k6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,kCoefVals_kCoeffBlure_k7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,kCoefVals_kCoeffBlure_k8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,kCoefVals_kCoeffBlure_k9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,kCoefVals_kCoeffSharp_kSet);//Note:kCoefVals_kCoeffBlure_kSet
    usleep(1);
}
/****************************************************************
KERNEL COEF UPDATES END
*****************************************************************/
void sobelWrite()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,psobel.K1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,psobel.K2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,psobel.K3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,psobel.K4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,psobel.K5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,psobel.K6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,psobel.K7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,psobel.K8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,psobel.K9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,psobel.Kc);
}
void prewittWrite()
{
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,pprewitt.K1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,pprewitt.K2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,pprewitt.K3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,pprewitt.K4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,pprewitt.K5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,pprewitt.K6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,pprewitt.K7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,pprewitt.K8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,pprewitt.K9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,pprewitt.Kc);
}
void prewittEnable()
{
    u32 KernelEnable = 0x01FF;
    //  GX
    //  [+1 +0 -1]
    //  [+1 +0 -1]
    //  [+1 +0 -1]
    //  GY
    //  [+1 +1 +1]
    //  [+0 +0 +0]
    //  [-1 -1 -1]
                           //  GY  GX
    u16 KernelPv1 = 0x01FF;//  +1  -1
    u16 KernelPv2 = 0x0100;//  +1  +0
    u16 KernelPv3 = 0x0101;//  +1  -1
    u16 KernelPv4 = 0x00FF;//  +0  -1
    u16 KernelPv5 = 0x0000;//  +0  +0
    u16 KernelPv6 = 0x0001;//  +0  -1
    u16 KernelPv7 = 0xFFFF;//  -1  -1
    u16 KernelPv8 = 0xFF00;//  -1  +0
    u16 KernelPv9 = 0xFF01;//  -1  +1
    D5M_mWriteReg(D5M_BASE,w_kernal_1_reg_08,KernelPv1);
    D5M_mWriteReg(D5M_BASE,w_kernal_2_reg_09,KernelPv2);
    D5M_mWriteReg(D5M_BASE,w_kernal_3_reg_10,KernelPv3);
    D5M_mWriteReg(D5M_BASE,w_kernal_4_reg_11,KernelPv4);
    D5M_mWriteReg(D5M_BASE,w_kernal_5_reg_12,KernelPv5);
    D5M_mWriteReg(D5M_BASE,w_kernal_6_reg_13,KernelPv6);
    D5M_mWriteReg(D5M_BASE,w_kernal_7_reg_14,KernelPv7);
    D5M_mWriteReg(D5M_BASE,w_kernal_8_reg_15,KernelPv8);
    D5M_mWriteReg(D5M_BASE,w_kernal_9_reg_16,KernelPv9);
    D5M_mWriteReg(D5M_BASE,w_kSet_reg_17,KernelEnable);
}
void colorFilterA11(u16 Axx)
{
    D5M_mWriteReg(D5M_BASE,w_a11fl_reg_21,Axx);
}
void colorFilterA12(u16 Axx)
{
    D5M_mWriteReg(D5M_BASE,w_a12fl_reg_22,Axx);
}
void colorFilterA13(u16 Axx)
{
    D5M_mWriteReg(D5M_BASE,w_a13fl_reg_23,Axx);
}
void colorFilterA21(u16 Axx)
{
    D5M_mWriteReg(D5M_BASE,w_a21fl_reg_24,Axx);
}
void colorFilterA22(u16 Axx)
{
    D5M_mWriteReg(D5M_BASE,w_a22fl_reg_25,Axx);
}
void colorFilterA23(u16 Axx)
{
    D5M_mWriteReg(D5M_BASE,w_a23fl_reg_26,Axx);
}
void colorFilterA31(u16 Axx)
{
    D5M_mWriteReg(D5M_BASE,w_a31fl_reg_27,Axx);
}
void colorFilterA32(u16 Axx)
{
    D5M_mWriteReg(D5M_BASE,w_a32fl_reg_28,Axx);
}
void colorFilterA33(u16 Axx)
{
    D5M_mWriteReg(D5M_BASE,w_a33fl_reg_29,Axx);
}
void colorFilterKc(u16 Axx)
{
    D5M_mWriteReg(D5M_BASE,w_aconfig_reg_30,Axx);
}
void colorFilterFixed()
{
    colorFilterA11(pcolor.K1);
    colorFilterA12(pcolor.K2);
    colorFilterA13(pcolor.K3);
    colorFilterA21(pcolor.K4);
    colorFilterA22(pcolor.K5);
    colorFilterA23(pcolor.K6);
    colorFilterA31(pcolor.K7);
    colorFilterA32(pcolor.K8);
    colorFilterA33(pcolor.K9);
    colorFilterKc(pcolor.Kc);//writeRead
    colorFilterKc(0x0000);//open
}
void computeBrightness() {
    u32 address = VIDEO_BASEADDR0;
    pvideo.brightness = 0;
    int buffer_error_colms = 8;
    int x,y;
      for ( y = 8; y < SCREEN_HEIGHT_VERTICAL; y++ )
      {
         for ( x = 0; x < SCREEN_WIDTH_HORIZONTAL*2; x++ )
         {
            pvideo.pixelvalue = (Xil_In16(address) & 0xffff);
            address = address + 0x2;
            if(y>buffer_error_colms)
            {
                if (pvideo.pixelvalue > pvideo.brightness)
                {
                    pvideo.brightness = pvideo.pixelvalue;
                }
            }
         }
        // printf("address[data]:%i     %i    %i\n",y,(unsigned)pvideo.pixelvalue, (unsigned)pvideo.brightness);
      }
}
void pRbrightness(){
    computeBrightness();
    printf("computedBrightnes :%i\n",(unsigned)pvideo.brightness);
}
void exposerCompansate(){
    pRexposer();
    pRbrightness();
        if(pvideo.brightness > pStream.d5mBrightness)//120
        {
            camera_exposer(pStream.d5mExposer);//400
        }
        else if(pvideo.brightness > pStream.d5mBrightness - 0x000A)//110
        {
            pStream.d5mExposer  = 0x0258;//600
            camera_exposer(pStream.d5mExposer);
        }
        else
        {
            pStream.d5mExposer  = 0x0320;//800
            camera_exposer(pStream.d5mExposer);
        }
        pRexposer();
        pRbrightness();
}
