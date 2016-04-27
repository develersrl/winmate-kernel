/*
 * Copyright (C) 2012-2013 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef _BOARD_MX6DL_SABRESD_H
#define _BOARD_MX6DL_SABRESD_H
#include <mach/iomux-mx6dl.h>

static iomux_v3_cfg_t mx6dl_sabresd_pads[] = {
	/* AUDMUX */
	MX6DL_PAD_CSI0_DATA_EN__GPIO_5_20,                                          /*PCIE_WAKE*/
	MX6DL_PAD_CSI0_DAT4__AUDMUX_AUD3_TXC,
	MX6DL_PAD_CSI0_DAT5__AUDMUX_AUD3_TXD,
	MX6DL_PAD_CSI0_DAT6__AUDMUX_AUD3_TXFS,
	MX6DL_PAD_CSI0_DAT7__AUDMUX_AUD3_RXD,
        MX6DL_PAD_EIM_D26__UART2_TXD,
        MX6DL_PAD_EIM_D27__UART2_RXD,

	MX6DL_PAD_KEY_ROW2__HDMI_TX_CEC_LINE,	/* HDMI_CEC_IN*/
	MX6DL_PAD_GPIO_1__GPIO_1_1,		/* WDOG_B */
	MX6DL_PAD_GPIO_2__GPIO_1_2,		/* BLON_3V_H */
	MX6DL_PAD_GPIO_7__CAN1_TXCAN,		/* CAN1_TX */
	MX6DL_PAD_GPIO_8__CAN1_RXCAN,		/* CAN1_RX */
	MX6DL_PAD_KEY_COL4__CAN2_TXCAN,      /* CAN2_TXCAN */
	MX6DL_PAD_KEY_ROW4__CAN2_RXCAN,     /* CAN2_RXCAN */
	MX6DL_PAD_GPIO_0__CCM_CLKO,		/* AUD_MCLK */
	MX6DL_PAD_KEY_COL1__GPIO_4_8,	/*over temperature indicator*/
	/* ECSPI1 NOR FLASH*/
	MX6DL_PAD_EIM_D16__ECSPI1_SCLK,
	MX6DL_PAD_EIM_D18__ECSPI1_MOSI,
	MX6DL_PAD_EIM_D17__ECSPI1_MISO,
	MX6DL_PAD_EIM_D19__GPIO_3_19,

	/* ENET */
	MX6DL_PAD_ENET_MDIO__ENET_MDIO,
	MX6DL_PAD_ENET_MDC__ENET_MDC,
	MX6DL_PAD_ENET_REF_CLK__ENET_TX_CLK,
	MX6DL_PAD_ENET_TX_EN__GPIO_1_28,		/* Micrel RGMII Phy Interrupt */
	MX6DL_PAD_RGMII_TXC__ENET_RGMII_TXC,
	MX6DL_PAD_RGMII_TD0__ENET_RGMII_TD0,
	MX6DL_PAD_RGMII_TD1__ENET_RGMII_TD1,
	MX6DL_PAD_RGMII_TD2__ENET_RGMII_TD2,
	MX6DL_PAD_RGMII_TD3__ENET_RGMII_TD3,
	MX6DL_PAD_RGMII_TX_CTL__ENET_RGMII_TX_CTL,
	//MX6DL_PAD_ENET_REF_CLK__ENET_TX_CLK,
	MX6DL_PAD_RGMII_RXC__ENET_RGMII_RXC,
	MX6DL_PAD_RGMII_RD0__ENET_RGMII_RD0,
	MX6DL_PAD_RGMII_RD1__ENET_RGMII_RD1,
	MX6DL_PAD_RGMII_RD2__ENET_RGMII_RD2,
	MX6DL_PAD_RGMII_RD3__ENET_RGMII_RD3,
	MX6DL_PAD_RGMII_RX_CTL__ENET_RGMII_RX_CTL,
	
	MX6DL_PAD_EIM_D23__GPIO_3_23,	/* LAN2_DEV_OFF# */
	
	/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_A23__GPIO_6_6,		/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_A22__GPIO_2_16,	/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_A21__GPIO_2_17,	/* J12 - Boot Mode Select */
	/* I2C1 */
	MX6DL_PAD_EIM_A20__GPIO_2_18,	/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_A19__GPIO_2_19,	/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_A18__GPIO_2_20,	/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_A17__GPIO_2_21,	/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_A16__GPIO_2_22,	/* J12 - Boot Mode Select */

/*	MX6Q_PAD_EIM_WAIT__GPIO_5_0,*/	/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_A24__GPIO_5_4,		/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_RW__GPIO_2_26,		/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_LBA__GPIO_2_27,	/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_EB0__GPIO_2_28,	/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_EB1__GPIO_2_29,	/* J12 - Boot Mode Select */

	MX6DL_PAD_EIM_EB2__GPIO_2_30,	/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_EB3__GPIO_2_31,	/* J12 - Boot Mode Select */

	MX6DL_PAD_EIM_DA0__GPIO_3_0,		/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_DA1__GPIO_3_1,		/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_DA2__GPIO_3_2,		/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_DA3__GPIO_3_3,		/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_DA4__GPIO_3_4,		/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_DA5__GPIO_3_5,		/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_DA6__GPIO_3_6,		/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_DA7__GPIO_3_7,		/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_DA8__GPIO_3_8,		/* J12 - Boot Mode Select */
 	MX6DL_PAD_EIM_DA9__GPIO_3_9,		              /* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_DA10__GPIO_3_10,	/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_DA11__GPIO_3_11,	/* J12 - Boot Mode Select */
	MX6DL_PAD_EIM_DA12__GPIO_3_12,	/* J12 - Boot Mode Select */
 	MX6DL_PAD_EIM_DA13__GPIO_3_13,	              /* J12 - Boot Mode Select */
 	MX6DL_PAD_EIM_DA14__GPIO_3_14,	               /* J12 - Boot Mode Select */
 	MX6DL_PAD_EIM_DA15__GPIO_3_15, 	              /* J12 - Boot Mode Select */
/*ruei add DIN0~5*/
#ifdef CONFIG_WM_HW_KEY_K0
	MX6DL_PAD_SD2_CMD__GPIO_1_11,
	MX6DL_PAD_SD2_CLK__GPIO_1_10,
	MX6DL_PAD_SD2_DAT0__GPIO_1_15,
	MX6DL_PAD_SD2_DAT1__GPIO_1_14,
	MX6DL_PAD_SD3_DAT4__GPIO_7_1,
	MX6DL_PAD_SD3_DAT5__GPIO_7_0,
#endif
	MX6DL_PAD_DISP0_DAT2__GPIO_4_23,
	MX6DL_PAD_DISP0_DAT5__GPIO_4_26,
	MX6DL_PAD_DISP0_DAT4__GPIO_4_25,
	MX6DL_PAD_DISP0_DAT0__GPIO_4_21,

	MX6DL_PAD_GPIO_4__GPIO_1_4,	/* TCH_PWRON */
	MX6DL_PAD_GPIO_5__GPIO_1_5,	/* EEPROM_WP */
	MX6DL_PAD_GPIO_9__GPIO_1_9,/*HEADPHONE DET*/
	MX6DL_PAD_KEY_COL0__GPIO_4_6,	/*DIMM-*/
	MX6DL_PAD_KEY_ROW0__GPIO_4_7,	/*DIMM+*/
	MX6DL_PAD_SD3_DAT6__GPIO_6_18, /*DOUT4*/
	MX6DL_PAD_SD3_DAT7__GPIO_6_17,/*DOUT5*/
	MX6DL_PAD_EIM_D29__GPIO_3_29,	/* PWR_BTN_SNS */
	
	MX6DL_PAD_NANDF_ALE__GPIO_6_8,	/* CAP_TCH_INT0 */

	MX6DL_PAD_NANDF_RB0__GPIO_6_10, /* AUX_5V Enable      Suspend enable and exit */

	/* I2C1, WM8958 */
	MX6DL_PAD_CSI0_DAT8__I2C1_SDA,
	MX6DL_PAD_CSI0_DAT9__I2C1_SCL,

	/* I2C2, Camera, MIPI */
	MX6DL_PAD_KEY_COL3__I2C2_SCL,
	MX6DL_PAD_KEY_ROW3__I2C2_SDA,

	/* I2C3 */
	MX6DL_PAD_GPIO_3__I2C3_SCL,	/* GPIO1[3] */
	MX6DL_PAD_GPIO_6__I2C3_SDA,

	/* DISPLAY */
	MX6DL_PAD_GPIO_9__PWM1_PWMO,		/* BL_PWM */
	MX6DL_PAD_SD1_DAT3__GPIO_1_21,		//kevin test for 38 inch panel

/*	MX6Q_PAD_SD1_DAT3__PWM1_PWMO,	*/	/* reserve BL_PWM  */
	/* UART1 for debug */
	MX6DL_PAD_CSI0_DAT10__UART1_TXD,
	MX6DL_PAD_CSI0_DAT11__UART1_RXD,

	/* UART3 for gps */
	MX6DL_PAD_EIM_D24__UART3_TXD,
	MX6DL_PAD_EIM_D25__UART3_RXD,

	/* USB */
	MX6DL_PAD_ENET_RX_ER__ANATOP_USBOTG_ID,
        //MX6DL_PAD_ENET_RX_ER__GPIO_1_24, /*set to 0 for always as host*/
	/* USB power pin */
	MX6DL_PAD_EIM_D22__GPIO_3_22,
	//MX6DL_PAD_EIM_D30__USBOH3_USBH1_OC,
	MX6DL_PAD_ENET_TXD1__GPIO_1_29,

	/* USB OC pin */
	MX6DL_PAD_EIM_D21__USBOH3_USBOTG_OC,
	MX6DL_PAD_EIM_D30__USBOH3_USBH1_OC,

	MX6DL_PAD_NANDF_D4__GPIO_2_4,	/*modified by kevin		watchdog en*/
	MX6DL_PAD_NANDF_D5__GPIO_2_5,	/*modified by kevin		EMMC_RST#*/
	MX6DL_PAD_NANDF_D6__GPIO_2_6,	/*modified by kevin		USBP2_OC*/
	MX6DL_PAD_NANDF_D7__GPIO_2_7,	/*modified by kevin		USBP2_EN*/
	MX6DL_PAD_NANDF_D2__GPIO_2_2,	/*modified by kevin		USBP1_OC*/
	MX6DL_PAD_NANDF_D3__GPIO_2_3,	/*modified by kevin		USBP1_EN*/

	MX6DL_PAD_EIM_D28__GPIO_3_28,	/*modified by kevin		USBP0_EN*/
	MX6DL_PAD_EIM_D31__GPIO_3_31,	/*modified by kevin		USBP0_OC*/

	MX6DL_PAD_NANDF_WP_B__GPIO_6_9,	/*modified by kevin		USBP3_EN*/
	MX6DL_PAD_NANDF_CLE__GPIO_6_7,	/*modified by kevin		USBP3_OC*/
	/* USDHC3 */
	MX6DL_PAD_SD3_CLK__USDHC3_CLK_50MHZ,
	MX6DL_PAD_SD3_CMD__USDHC3_CMD_50MHZ,
	MX6DL_PAD_SD3_DAT0__USDHC3_DAT0_50MHZ,
	MX6DL_PAD_SD3_DAT1__USDHC3_DAT1_50MHZ,
	MX6DL_PAD_SD3_DAT2__USDHC3_DAT2_50MHZ,
	MX6DL_PAD_SD3_DAT3__USDHC3_DAT3_50MHZ,
	//MX6DL_PAD_SD3_DAT4__USDHC3_DAT4_50MHZ,
	//MX6DL_PAD_SD3_DAT5__USDHC3_DAT5_50MHZ,
	//MX6DL_PAD_SD3_DAT6__USDHC3_DAT6_50MHZ,
	//MX6DL_PAD_SD3_DAT7__USDHC3_DAT7_50MHZ,
	/* SD3_CD */
	MX6DL_PAD_NANDF_D0__GPIO_2_0,
	/* SD3_WP */
	MX6DL_PAD_NANDF_D1__GPIO_2_1,

	/* USDHC4 */
	MX6DL_PAD_SD4_CLK__USDHC4_CLK_50MHZ,
	MX6DL_PAD_SD4_CMD__USDHC4_CMD_50MHZ,
	MX6DL_PAD_SD4_DAT0__USDHC4_DAT0_50MHZ,
	MX6DL_PAD_SD4_DAT1__USDHC4_DAT1_50MHZ,
	MX6DL_PAD_SD4_DAT2__USDHC4_DAT2_50MHZ,
	MX6DL_PAD_SD4_DAT3__USDHC4_DAT3_50MHZ,
	MX6DL_PAD_SD4_DAT4__USDHC4_DAT4_50MHZ,
	MX6DL_PAD_SD4_DAT5__USDHC4_DAT5_50MHZ,
	MX6DL_PAD_SD4_DAT6__USDHC4_DAT6_50MHZ,
	MX6DL_PAD_SD4_DAT7__USDHC4_DAT7_50MHZ,

	
	/* Charge */
	MX6DL_PAD_ENET_RXD0__GPIO_1_27, /* UOK_B */
	MX6DL_PAD_EIM_CS1__GPIO_2_24,   /* DOK_B */
	MX6DL_PAD_EIM_D23__GPIO_3_23, /* LAN2_DEV_OFF# */
	MX6DL_PAD_EIM_A25__GPIO_5_2,  /* PAD27 */

	/* Audio Codec */
	MX6DL_PAD_KEY_COL2__GPIO_4_10,		/* CODEC_PWR_EN */
	MX6DL_PAD_SD3_RST__GPIO_7_8,			/* HEADPHONE_DET */

	/* PCIE */
	MX6DL_PAD_EIM_D19__GPIO_3_19, /* PCIE_PWR_EN */
	

	MX6DL_PAD_GPIO_17__GPIO_7_12, /* PCIE_RST */
	MX6DL_PAD_GPIO_18__GPIO_7_13, /* PMIC_INT_B */


	
	MX6DL_PAD_NANDF_CS0__GPIO_6_11,	/* SPI_NOR_WP */
	MX6DL_PAD_NANDF_CS1__GPIO_6_14,	/* LAN2_EEWP */
	MX6DL_PAD_NANDF_CS2__GPIO_6_15	/* CABC_EN0 */,
	MX6DL_PAD_NANDF_CS3__GPIO_6_16,	/* CAP_TCH_RST */

	
        MX6DL_PAD_SD1_CMD__GPIO_1_18,	/*FAN_EN added by kevin*/
	MX6DL_PAD_SD1_DAT1__GPIO_1_17,	/*BUZ_INT_EN*/
	MX6DL_PAD_SD1_DAT2__GPIO_1_19,	/*BUZ_EXT_EN*/

	MX6DL_PAD_SD1_CLK__GPIO_1_20,	/*GP_LED_G*/	//kevin add for nfc_enable
	
};

static iomux_v3_cfg_t mx6dl_sabresd_csi0_sensor_pads[] = {
	/* IPU1 Camera */
	MX6DL_PAD_CSI0_DAT12__IPU1_CSI0_D_12,
	MX6DL_PAD_CSI0_DAT13__IPU1_CSI0_D_13,
	MX6DL_PAD_CSI0_DAT14__IPU1_CSI0_D_14,
	MX6DL_PAD_CSI0_DAT15__IPU1_CSI0_D_15,
	MX6DL_PAD_CSI0_DAT16__IPU1_CSI0_D_16,
	MX6DL_PAD_CSI0_DAT17__IPU1_CSI0_D_17,
	MX6DL_PAD_CSI0_DAT18__IPU1_CSI0_D_18,
	MX6DL_PAD_CSI0_DAT19__IPU1_CSI0_D_19,
	MX6DL_PAD_CSI0_DATA_EN__IPU1_CSI0_DATA_EN,
	MX6DL_PAD_CSI0_MCLK__IPU1_CSI0_HSYNC,
	MX6DL_PAD_CSI0_PIXCLK__IPU1_CSI0_PIXCLK,
	MX6DL_PAD_CSI0_VSYNC__IPU1_CSI0_VSYNC,

	MX6DL_PAD_GPIO_0__CCM_CLKO,		/* camera clk */

	MX6DL_PAD_SD1_DAT0__GPIO_1_16,		/* camera PWDN */
	MX6DL_PAD_SD1_DAT1__GPIO_1_17,		/* camera RESET */
};

static iomux_v3_cfg_t mx6dl_sabresd_mipi_sensor_pads[] = {
	MX6DL_PAD_GPIO_0__CCM_CLKO,		/* camera clk */

	MX6DL_PAD_SD1_DAT2__GPIO_1_19,		/* camera PWDN */
	MX6DL_PAD_SD1_CLK__GPIO_1_20,		/* camera RESET */
};

static iomux_v3_cfg_t mx6dl_sabresd_epdc_enable_pads[] = {
	/* EPDC */
	/*MX6DL_PAD_EIM_A16__EPDC_SDDO_0,
	MX6DL_PAD_EIM_DA10__EPDC_SDDO_1,
	MX6DL_PAD_EIM_DA12__EPDC_SDDO_2,
	MX6DL_PAD_EIM_DA11__EPDC_SDDO_3,
	MX6DL_PAD_EIM_LBA__EPDC_SDDO_4,
	MX6DL_PAD_EIM_EB2__EPDC_SDDO_5,
	MX6DL_PAD_EIM_CS0__EPDC_SDDO_6,
	MX6DL_PAD_EIM_RW__EPDC_SDDO_7,
	MX6DL_PAD_EIM_A21__EPDC_GDCLK,
	MX6DL_PAD_EIM_A22__EPDC_GDSP,
	MX6DL_PAD_EIM_A23__EPDC_GDOE,
	MX6DL_PAD_EIM_A24__EPDC_GDRL,
	MX6DL_PAD_EIM_D31__EPDC_SDCLK,
	MX6DL_PAD_EIM_D27__EPDC_SDOE,
	MX6DL_PAD_EIM_DA1__EPDC_SDLE,
	MX6DL_PAD_EIM_EB1__EPDC_SDSHR,
	MX6DL_PAD_EIM_DA2__EPDC_BDR_0,
	MX6DL_PAD_EIM_DA4__EPDC_SDCE_0,
	MX6DL_PAD_EIM_DA5__EPDC_SDCE_1,
	MX6DL_PAD_EIM_DA6__EPDC_SDCE_2,
*/
	/* EPD PMIC (Maxim 17135) pins */
//	MX6DL_PAD_EIM_A17__GPIO_2_21, /* EPDC_PWRSTAT */
//	MX6DL_PAD_EIM_D17__GPIO_3_17, /* EPDC_VCOM0 */
//	MX6DL_PAD_EIM_A18__GPIO_2_20, /* EPDC_PWRCTRL0 */
//	MX6DL_PAD_EIM_D20__GPIO_3_20, /* EPDC_PMIC_WAKEUP */
	/*
	 * Depopulate R121, R123, R133, R138, R139,
	 * R167, R168, and R627 when using E-BOOK
	 * Card in 16-bit Data Mode. Meanwhile, comments
	 * the conflict PIN configurations in above tables
	 */
	/*
	MX6DL_PAD_EIM_CS1__EPDC_SDDO_8,
	MX6DL_PAD_EIM_DA15__EPDC_SDDO_9,
	MX6DL_PAD_EIM_D16__EPDC_SDDO_10,
	MX6DL_PAD_EIM_D23__EPDC_SDDO_11
	MX6DL_PAD_EIM_D19__EPDC_SDDO_12,
	MX6DL_PAD_EIM_DA13__EPDC_SDDO_13,
	MX6DL_PAD_EIM_DA14__EPDC_SDDO_14,
	MX6DL_PAD_EIM_A25__EPDC_SDDO_15,
	 */
};

static iomux_v3_cfg_t mx6dl_sabresd_epdc_disable_pads[] = {
	/* EPDC */
/*	MX6DL_PAD_EIM_A16__GPIO_2_22,
	MX6DL_PAD_EIM_DA10__GPIO_3_10,
	MX6DL_PAD_EIM_DA12__GPIO_3_12,
	MX6DL_PAD_EIM_DA11__GPIO_3_11,
	MX6DL_PAD_EIM_LBA__GPIO_2_27,
	MX6DL_PAD_EIM_EB2__GPIO_2_30,
	MX6DL_PAD_EIM_CS0__GPIO_2_23,
	MX6DL_PAD_EIM_RW__GPIO_2_26,
	MX6DL_PAD_EIM_A21__GPIO_2_17,
	MX6DL_PAD_EIM_A22__GPIO_2_16,
	MX6DL_PAD_EIM_A23__GPIO_6_6,
	MX6DL_PAD_EIM_A24__GPIO_5_4,
	MX6DL_PAD_EIM_D31__GPIO_3_31,
	MX6DL_PAD_EIM_D27__GPIO_3_27,
	MX6DL_PAD_EIM_DA1__GPIO_3_1,
	MX6DL_PAD_EIM_EB1__GPIO_2_29,
	MX6DL_PAD_EIM_DA2__GPIO_3_2,
	MX6DL_PAD_EIM_DA4__GPIO_3_4,
	MX6DL_PAD_EIM_DA5__GPIO_3_5,
	MX6DL_PAD_EIM_DA6__GPIO_3_6,
*/
	/*
	 * Depopulate R121, R123, R133, R138, R139,
	 * R167, R168, and R627 when using E-BOOK
	 * Card in 16-bit Data Mode. Meanwhile, comments
	 * the conflict PIN configurations in above tables
	 */
	/*
	MX6DL_PAD_EIM_CS1__EPDC_SDDO_8,
	MX6DL_PAD_EIM_DA15__EPDC_SDDO_9,
	MX6DL_PAD_EIM_D16__EPDC_SDDO_10,
	MX6DL_PAD_EIM_D23__EPDC_SDDO_11
	MX6DL_PAD_EIM_D19__EPDC_SDDO_12,
	MX6DL_PAD_EIM_DA13__EPDC_SDDO_13,
	MX6DL_PAD_EIM_DA14__EPDC_SDDO_14,
	MX6DL_PAD_EIM_A25__EPDC_SDDO_15,
	 */
};

static iomux_v3_cfg_t mx6dl_arm2_elan_pads[] = {
	//MX6DL_PAD_EIM_A20__GPIO_2_18,
	//MX6DL_PAD_EIM_DA8__GPIO_3_8,
	//MX6DL_PAD_EIM_D28__GPIO_3_28,
};
#ifdef CONFIG_PN544_NFC
static iomux_v3_cfg_t mx6dl_sabresd_hdmi_ddc_pads[] = {
      MX6DL_PAD_EIM_EB2__HDMI_TX_DDC_SCL, /* HDMI DDC SCL */
      MX6DL_PAD_EIM_D16__HDMI_TX_DDC_SDA, /* HDMI DDC SDA */
};
#else
static iomux_v3_cfg_t mx6dl_sabresd_hdmi_ddc_pads[] = {
	MX6DL_PAD_KEY_COL3__HDMI_TX_DDC_SCL, /* HDMI DDC SCL */
	MX6DL_PAD_KEY_ROW3__HDMI_TX_DDC_SDA, /* HDMI DDC SDA */
};
#endif

static iomux_v3_cfg_t mx6dl_sabresd_i2c2_pads[] = {
	MX6DL_PAD_KEY_COL3__I2C2_SCL,	/* I2C2 SCL */
	MX6DL_PAD_KEY_ROW3__I2C2_SDA,	/* I2C2 SDA */
};
#endif
