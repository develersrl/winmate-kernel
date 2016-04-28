/*winmate frank Maintenance panel parameters table*/
#ifndef _PANEL_TYEP_
#define _PANEL_TYEP_


#if	defined(CONFIG_PANEL_HANNSTAR_HSD100IFW4)
	#include <linux/ipu.h>
	#include <linux/fb.h>
	#include <linux/fsl_devices.h>
	
#else 	
	#include <ipu.h>
	#include <linux/fb.h>
	#define LDB_SPL_DI0	1
	#define LDB_SPL_DI1	2
	#define LDB_DUL_DI0	3
	#define LDB_DUL_DI1	4
	#define LDB_SIN0		5
	#define LDB_SIN1		6
	#define LDB_SEP0		7
	#define LDB_SEP1		8
#endif





enum ELvdsMode{
	eChDisable=0,
	eCh0EnableToDI0	= 0x1,
	eCh0EnableToDI1	= 0x3,
	eCh1EnableToDI0	= 0x4,
	eCh1EnableToDI1	= 0xc,
	eSplitModeEnable	= 0x10,
	eCh0_24Bit 	=0x20,
	eCh0_JEIDA	=0X40,
	eCh1_24Bit 	=0x80,
	eCh1_JEIDA	=0X100,
	eDI0_VS		=0x200,
	eDI1_VS		=0x400,
};

	#define PANEL_NAME			"HANNSTAR_HSD100IFW4"
	#define PANEL_REFRESH			60
	#define PANEL_XRES			1024
	#define PANEL_YRES			600
	#define PANEL_FREQ			65000000
	#define PANEL_PIXCLOCK			(1000000000000/ PANEL_FREQ)
	#define PANEL_LEFT_MARGIN		220
	#define PANEL_RIGHT_MARGIN		40
	#define PANEL_UPPER_MARGIN		21
	#define PANEL_LOWER_MARGIN		7
	#define PANEL_HSYNC_LEN			60
	#define PANEL_VSYNC_LEN			10
	#define PANEL_SYNC			FB_SYNC_EXT
	#define PANEL_VMODE       			FB_VMODE_NONINTERLACED
	#define PANEL_FLAG			0
	#define PANEL_PIX_FMT			IPU_PIX_FMT_RGB666
	#define PANEL_PWM_PERIOD		50000
	#define LDB_CANNEL_CHAGE			1 // 0: DI0(1) TO LVDS0(1) 1: 	DI0(1) TO LVDS1(0)
	#define PANEL_LDB_MODE			LDB_SEP1 
	#define PANEL_PS_T1			40//T1+T2=40mS panel_pwr to lvds_signal time
	#define PANEL_PS_T2			300//T5=300mS  lvds signal to BL_on time
	#define PANEL_BIT_MAPPING		0 //0    Use SPWG standard , 1    Use JEIDA standard.
	
	#if !defined(CONFIG_PANEL_HANNSTAR_HSD100IFW4)
		#define PANEL_PCLK_LDB		DI_PCLK_LDB
	#endif







#endif//#ifndef _PANEL_TYEP_

