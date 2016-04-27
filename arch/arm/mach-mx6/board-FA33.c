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
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/nodemask.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/fsl_devices.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/i2c.h>
#include <linux/i2c/pca953x.h>
#include <linux/ata.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/partitions.h>
#include <linux/regulator/consumer.h>
#include <linux/pmic_external.h>
#include <linux/pmic_status.h>
#include <linux/ipu.h>
#include <linux/mxcfb.h>
#include <linux/pwm_backlight.h>
#include <linux/fec.h>
#include <linux/memblock.h>
#include <linux/gpio.h>
#include <linux/ion.h>
#include <linux/etherdevice.h>
#include <linux/power/sabresd_battery.h>
#include <linux/regulator/anatop-regulator.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/fixed.h>
#include <linux/mfd/max17135.h>
#include <linux/mfd/wm8994/pdata.h>
#include <linux/mfd/wm8994/gpio.h>
#include <sound/wm8962.h>
#include <linux/mfd/mxc-hdmi-core.h>

#include <mach/common.h>
#include <mach/hardware.h>
#include <mach/mxc_dvfs.h>
#include <mach/memory.h>
#include <mach/iomux-mx6q.h>
#include <mach/imx-uart.h>
#include <mach/viv_gpu.h>
#include <mach/ahci_sata.h>
#include <mach/ipu-v3.h>
#include <mach/mxc_hdmi.h>
#include <mach/mxc_asrc.h>
#include <mach/mipi_dsi.h>

#include <asm/irq.h>
#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>

#include "usb.h"
#include "devices-imx6q.h"
#include "crm_regs.h"
#include "cpu_op-mx6.h"
#include "board-FA33.h"
#include "board-FA33_DL.h"
/* ruei comment out
#include "board-mx6dl_sabresd.h"
*/
#include <linux/i2c/at24.h>
#include <linux/leds.h>
#include <mach/imx_rfkill.h>

#include <../drivers/staging/iio/light/tsl2563.h>
#include "../../../../include/paneltype.h" //winmate frank add Maintenance panel parameters

#define SABRESD_BLON_3V_H	IMX_GPIO_NR(1, 2)
#define SABRESD_MICROPHONE_DET	IMX_GPIO_NR(1, 9)

#define SABRESD_BUZZER_INTERNAL    IMX_GPIO_NR(1, 17)

#define SABRESD_BUZZER_EXTERNAL    IMX_GPIO_NR(1, 19)

#ifdef CONFIG_PANEL_LITEMAX_SSF_SSH3825
#define SABRESD_BL_PWM          IMX_GPIO_NR(1, 21)      //kevin test for 38 inch panel
#endif

#define SABRESD_PAD31		IMX_GPIO_NR(1, 20)
#define SABRESD_USB_H1_PWR	IMX_GPIO_NR(1, 29)

#define SABRESD_USBP0_EN	IMX_GPIO_NR(3, 28)   
#define SABRESD_USBP1_EN	IMX_GPIO_NR(2, 3)
#define SABRESD_USBP2_EN	IMX_GPIO_NR(2, 7)
#define SABRESD_USBP3_EN	IMX_GPIO_NR(6, 9)

#define SABRESD_SD3_CD		IMX_GPIO_NR(2, 0)
#define SABRESD_SD3_WP		IMX_GPIO_NR(2, 1)
#define SABRESD_USBP1_OC		IMX_GPIO_NR(2, 2)
#define SABRESD_USBP1_EN		IMX_GPIO_NR(2, 3)
#define SABRESD_EPDC_PWRCTRL0	IMX_GPIO_NR(2, 20)
#define SABRESD_EPDC_PWRSTAT	IMX_GPIO_NR(2, 21)
#define SABRESD_EPDC_PMIC_INT	IMX_GPIO_NR(2, 25)
#define SABRESD_SENSOR_EN       	IMX_GPIO_NR(2, 31)

#define SABRESD_EPDC_VCOM	IMX_GPIO_NR(3, 17)
#define SABRESD_CSPI1_CS0	IMX_GPIO_NR(3, 19)
#define SABRESD_PCIE_PWR_EN     	IMX_GPIO_NR(3, 19)

#define SABRESD_USB_OTG_PWR	IMX_GPIO_NR(3, 22)
#define SABRESD_TS_INT		IMX_GPIO_NR(3, 26)
#define SABRESD_DISP0_RD		IMX_GPIO_NR(3, 28)
#define SABRESD_POWER_OFF	IMX_GPIO_NR(3, 29)
#define SABRESD_CAN1_STBY       	IMX_GPIO_NR(4, 5)
#define SABRESD_DIMMING_DN	IMX_GPIO_NR(4, 6)
#define SABRESD_DIMMING_UP	IMX_GPIO_NR(4, 7)
#define SABRESD_CODEC_PWR_EN	IMX_GPIO_NR(4, 10)
#define SABRESD_HDMI_CEC_IN	IMX_GPIO_NR(4, 11)
#define SABRESD_PCIE_DIS_B      IMX_GPIO_NR(4, 14)
#define SABRESD_PCIE_WAKE_B	IMX_GPIO_NR(5, 20)
#define SABRESD_USBP3_OC		IMX_GPIO_NR(6, 7)
#define SABRESD_CAP_TCH_INT0	IMX_GPIO_NR(6, 8)
#define SABRESD_USBP3_EN		IMX_GPIO_NR(6, 9)
#define SABRESD_AUX_5V_EN	IMX_GPIO_NR(6, 10)
#define SABRESD_DISP_RST_B	IMX_GPIO_NR(6, 11)
#define SABRESD_DISP_PWR_EN	IMX_GPIO_NR(6, 14)
#define SABRESD_SPI_NOR_WP	IMX_GPIO_NR(6, 11)
#define SABRESD_LAN2_EEWP	IMX_GPIO_NR(6, 14)
#define SABRESD_CABC_EN0		IMX_GPIO_NR(6, 15)
#define SABRESD_CAP_TCH_RST	IMX_GPIO_NR(6, 16)

//#define rudolph_SD_WP			IMX_GPIO_NR(2, 1)
//#define WM_SET_AS_HOST		        IMX_GPIO_NR(1, 24) //rudolph add for set host

#define SABRESD_HEADPHONE_DET	IMX_GPIO_NR(7, 8)
#define SABRESD_PCIE_RST_B_REVB	IMX_GPIO_NR(7, 12)
#define SABRESD_PMIC_INT_B	IMX_GPIO_NR(7, 13)
#define SABRESD_PFUZE_INT	IMX_GPIO_NR(7, 13)

#define SABRESD_EPDC_SDDO_0	IMX_GPIO_NR(2, 22)
#define SABRESD_EPDC_SDDO_1	IMX_GPIO_NR(3, 10)
#define SABRESD_EPDC_SDDO_2	IMX_GPIO_NR(3, 12)
#define SABRESD_EPDC_SDDO_3	IMX_GPIO_NR(3, 11)
#define SABRESD_EPDC_SDDO_4	IMX_GPIO_NR(2, 27)
#define SABRESD_EPDC_SDDO_5	IMX_GPIO_NR(2, 30)
#define SABRESD_EPDC_SDDO_6	IMX_GPIO_NR(2, 23)
#define SABRESD_EPDC_SDDO_7	IMX_GPIO_NR(2, 26)
#define SABRESD_EPDC_SDDO_8	IMX_GPIO_NR(2, 24)
#define SABRESD_EPDC_SDDO_9	IMX_GPIO_NR(3, 15)
#define SABRESD_EPDC_SDDO_10	IMX_GPIO_NR(3, 16)
#define SABRESD_EPDC_SDDO_11	IMX_GPIO_NR(3, 23)
#define SABRESD_EPDC_SDDO_12	IMX_GPIO_NR(3, 19)
#define SABRESD_EPDC_SDDO_13	IMX_GPIO_NR(3, 13)
#define SABRESD_EPDC_SDDO_14	IMX_GPIO_NR(3, 14)
#define SABRESD_EPDC_SDDO_15	IMX_GPIO_NR(5, 2)
#define SABRESD_EPDC_GDCLK	IMX_GPIO_NR(2, 17)
#define SABRESD_EPDC_GDSP	IMX_GPIO_NR(2, 16)
#define SABRESD_EPDC_GDOE	IMX_GPIO_NR(6, 6)
#define SABRESD_EPDC_GDRL	IMX_GPIO_NR(5, 4)
#define SABRESD_EPDC_SDCLK	IMX_GPIO_NR(3, 31)
#define SABRESD_EPDC_SDOEZ	IMX_GPIO_NR(3, 30)
#define SABRESD_EPDC_SDOED	IMX_GPIO_NR(3, 26)
#define SABRESD_EPDC_SDOE	IMX_GPIO_NR(3, 27)
#define SABRESD_EPDC_SDLE	IMX_GPIO_NR(3, 1)
#define SABRESD_EPDC_SDCLKN	IMX_GPIO_NR(3, 0)
#define SABRESD_EPDC_SDSHR	IMX_GPIO_NR(2, 29)
#define SABRESD_EPDC_PWRCOM	IMX_GPIO_NR(2, 28)
#define SABRESD_EPDC_PWRSTAT	IMX_GPIO_NR(2, 21)
#define SABRESD_EPDC_PWRCTRL0	IMX_GPIO_NR(2, 20)
#define SABRESD_EPDC_PWRCTRL1	IMX_GPIO_NR(2, 19)
#define SABRESD_EPDC_PWRCTRL2	IMX_GPIO_NR(2, 18)
#define SABRESD_EPDC_PWRCTRL3	IMX_GPIO_NR(3, 28)
#define SABRESD_EPDC_BDR0	IMX_GPIO_NR(3, 2)
#define SABRESD_EPDC_BDR1	IMX_GPIO_NR(3, 3)
#define SABRESD_EPDC_SDCE0	IMX_GPIO_NR(3, 4)
#define SABRESD_EPDC_SDCE1	IMX_GPIO_NR(3, 5)
#define SABRESD_EPDC_SDCE2	IMX_GPIO_NR(3, 6)
#define SABRESD_EPDC_SDCE3	IMX_GPIO_NR(3, 7)
#define SABRESD_EPDC_SDCE4	IMX_GPIO_NR(3, 8)
#define SABRESD_EPDC_PMIC_WAKE	IMX_GPIO_NR(3, 20)
#define SABRESD_EPDC_PMIC_INT	IMX_GPIO_NR(2, 25)
#define SABRESD_EPDC_VCOM	IMX_GPIO_NR(3, 17)
#define SABRESD_ELAN_CE		IMX_GPIO_NR(2, 18)
#define SABRESD_ELAN_RST	IMX_GPIO_NR(3, 8)
#define SABRESD_ELAN_INT	IMX_GPIO_NR(3, 28)
#define SABRESD_EEPROM_RW	IMX_GPIO_NR(1, 5)

#define SABRESD_ECSPI3_SCLK	IMX_GPIO_NR(4, 21)	/* DISP0_DAT0 117 */
#define SABRESD_ECSPI3_MISO	IMX_GPIO_NR(4, 23)	/* DISP0_DAT2 119 */
#define SABRESD_ECSPI3_CS1	IMX_GPIO_NR(4, 25)	/* DISP0_DAT4 121 */
#define SABRESD_ECSPI3_MOSI	IMX_GPIO_NR(4, 26)	/* DISP0_DAT5 122 */

#ifdef CONFIG_MX6_ENET_IRQ_TO_GPIO
#define MX6_ENET_IRQ		IMX_GPIO_NR(1, 6)
#define IOMUX_OBSRV_MUX1_OFFSET	0x3c
#define OBSRV_MUX1_MASK			0x3f
#define OBSRV_MUX1_ENET_IRQ		0x9
#endif

#define LINE_SW		IMX_GPIO_NR(3, 30)	/* 94 */
#define DIN0		IMX_GPIO_NR(1, 11)	/* 11 */
#define DIN1		IMX_GPIO_NR(1, 10)	/* 10 */
#define DIN2		IMX_GPIO_NR(1, 15)	/* 15 */
#define DIN3		IMX_GPIO_NR(1, 14)	/* 14 */
#define DIN4		IMX_GPIO_NR(7, 1)       /*193*/
#define DIN5            IMX_GPIO_NR(7, 0)	/*192*/
#define DOUT0		IMX_GPIO_NR(4, 6)	/* 102 */
#define DOUT1		IMX_GPIO_NR(4, 7)	/* 103 */
#define DOUT2		IMX_GPIO_NR(4, 8)	/* 104 */
#define DOUT3		IMX_GPIO_NR(4, 9)	/* 105 */
#define DOUT4           IMX_GPIO_NR(6, 18)      /*178*/
#define DOUT5           IMX_GPIO_NR(6, 17)      /*177*/
#define NFC_IRQ		IMX_GPIO_NR(1, 18)	/* 18 */	//kevin add
#define NFC_ENABLE	IMX_GPIO_NR(1, 20)	/* 20 */	//kevin add
#define GP_LED_B	IMX_GPIO_NR(1, 16)	/* 16 */
#define I2C_RST		IMX_GPIO_NR(6, 16)	/*176*/		//willy add
//rudolph add a global variable for MAC ADDRESS
unsigned char MAC_Address_For_eth0[17];
//rudolph add a global variable for MAC ADDRESS
static struct clk *sata_clk;
static struct clk *clko;

static int uart5_enabled;

extern char *gp_reg_id;
extern char *soc_reg_id;
extern char *pu_reg_id;
extern int epdc_enabled;
extern bool enet_to_gpio_6;

static int max17135_regulator_init(struct max17135 *max17135);


static const struct esdhc_platform_data mx6q_sabresd_sd3_data __initconst = {
	.cd_gpio = SABRESD_SD3_CD,
	.wp_gpio = SABRESD_SD3_WP,
	.keep_power_at_suspend = 1,
	.support_8bit = 1,
	.delay_line = 0,
	.cd_type = ESDHC_CD_CONTROLLER,
	.runtime_pm = 1,
};


static const struct esdhc_platform_data mx6q_sabresd_sd4_data __initconst = {
	.always_present = 1,
	.keep_power_at_suspend = 1,
	.support_8bit = 1,
	.delay_line = 0,
	.cd_type = ESDHC_CD_PERMANENT,
};


static const struct anatop_thermal_platform_data
	mx6q_sabresd_anatop_thermal_data __initconst = {
		.name = "anatop_thermal",
};

static const struct imxuart_platform_data mx6q_sd_uart5_data __initconst = {
	.flags      = IMXUART_HAVE_RTSCTS,
	.dma_req_rx = MX6Q_DMA_REQ_UART5_RX,
	.dma_req_tx = MX6Q_DMA_REQ_UART5_TX,
};

//no need this for setting UART2
/*static const struct imxuart_platform_data mx6q_sd_uart2_data __initconst = {
	.flags      = IMXUART_HAVE_RTSCTS,
	.dma_req_rx = MX6Q_DMA_REQ_UART2_RX,
	.dma_req_tx = MX6Q_DMA_REQ_UART2_TX,
};*/

//coby
/* uart2 pins */
static iomux_v3_cfg_t mx6q_uart2_pads[] = {
 /* UART2 for debug */
	MX6Q_PAD_EIM_D26__UART2_TXD,
	MX6Q_PAD_EIM_D27__UART2_RXD,

};

static iomux_v3_cfg_t mx6q_uart3_pads[] = {
 /* UART2 for debug */
        MX6Q_PAD_EIM_D24__UART3_TXD,
        MX6Q_PAD_EIM_D25__UART3_RXD,

};

static iomux_v3_cfg_t mx6dl_uart2_pads[] = {
 /* UART2 for debug */
        MX6DL_PAD_EIM_D26__UART2_TXD,
        MX6DL_PAD_EIM_D27__UART2_RXD,

};

static iomux_v3_cfg_t mx6dl_uart3_pads[] = {
 /* UART2 for debug */
        MX6DL_PAD_EIM_D24__UART3_TXD,
        MX6DL_PAD_EIM_D25__UART3_RXD,

};



static inline void mx6q_sabresd_init_uart(void)
{

	if(cpu_is_mx6dl())
	{
	 	printk(KERN_INFO "uart init  is added 6DL\n");	
		mxc_iomux_v3_setup_multiple_pads(mx6dl_uart2_pads,
                        ARRAY_SIZE(mx6dl_uart2_pads));

        	mxc_iomux_v3_setup_multiple_pads(mx6dl_uart3_pads,
                        ARRAY_SIZE(mx6dl_uart3_pads));
	}
	else if(cpu_is_mx6q())
	{
		printk(KERN_INFO "uart init  is added 6Q\n");
		mxc_iomux_v3_setup_multiple_pads(mx6q_uart2_pads,
			ARRAY_SIZE(mx6q_uart2_pads));
	
		mxc_iomux_v3_setup_multiple_pads(mx6q_uart3_pads,
                        ARRAY_SIZE(mx6q_uart3_pads));
	}
	imx6q_add_imx_uart(2, NULL);//UART3 ??
	imx6q_add_imx_uart(1, NULL);//UART2
	imx6q_add_imx_uart(0, NULL);
}
// rudolph add read eeprom start

static int eeprom_read(struct memory_accessor *mem_acc, unsigned char *buf, int offset, int size, const char* objname)
{
        ssize_t ret;
	int i;
        ret = mem_acc->read(mem_acc, buf, offset, size);
        if (ret != size) {
                pr_warn("CM-FX6: EEPROM %s read failed: %d\n", objname, ret);
                return ret;
        }
	i=0;
	while(i<size)
	{
		MAC_Address_For_eth0[i]=buf[i];
		i++;
	}
        return 0;
}
static void eeprom_read_mac_address(struct memory_accessor *mem_acc,
                    unsigned char *mac)
{
        char *objname = "FEC_MAC_ADDR";
	int read_number=17;
        if (eeprom_read(mem_acc, mac, 0, read_number, objname))
                memset(mac, 0, ETH_ALEN);
}

static int mx6q_sabresd_fec_phy_init(struct phy_device *phydev)
{
        unsigned short val;
	int i;//rudolph add
	struct sockaddr sa;//rudolph add 
	u8 mac_temp[17];//rudolph add
        phy_write(phydev, 0xd, 0x3);
        phy_write(phydev, 0xe, 0x805d);
        phy_write(phydev, 0xd, 0x4003);
        val = phy_read(phydev, 0xe);
        val &= ~(0x1 << 8);
        phy_write(phydev, 0xe, val);

        /* To enable AR8031 ouput a 125MHz clk from CLK_25M */
        phy_write(phydev, 0xd, 0x7);
        phy_write(phydev, 0xe, 0x8016);
        phy_write(phydev, 0xd, 0x4007);
        val = phy_read(phydev, 0xe);

        val &= 0xffe3;
        val |= 0x18;
        phy_write(phydev, 0xe, val);

        /* Introduce tx clock delay */
        phy_write(phydev, 0x1d, 0x5);
        val = phy_read(phydev, 0x1e);
        val |= 0x0100;
        phy_write(phydev, 0x1e, val);

        //rudolph add sat the lan MAC ADDRESS start
	i=0;
	while(i<17)
        {
		if((MAC_Address_For_eth0[i] > 47) && (MAC_Address_For_eth0[i] < 58))
		{
			mac_temp[i]=MAC_Address_For_eth0[i]-48; //change number to 4 bits
		}
		else if((MAC_Address_For_eth0[i] > 64) && (MAC_Address_For_eth0[i] < 71)) 
		{
			mac_temp[i]=MAC_Address_For_eth0[i]-64+9;//change A~F to 4 bits 
		}else if ((MAC_Address_For_eth0[i] > 96) && (MAC_Address_For_eth0[i] < 103))
		{
			mac_temp[i]=MAC_Address_For_eth0[i]-96+9;//change a~f to 4 bits
		}
		else
		{
			if(MAC_Address_For_eth0[i]!=58)
				printk("The unavilable MAC Address\n");
		}
		i++;
        }
	sa.sa_data[0]=((mac_temp[0] << 4) | mac_temp[1]);
        sa.sa_data[1]=((mac_temp[3] << 4) | mac_temp[4]);;
        sa.sa_data[2]=((mac_temp[6] << 4) | mac_temp[7]);;
        sa.sa_data[3]=((mac_temp[9] << 4) | mac_temp[10]);;
        sa.sa_data[4]=((mac_temp[12] << 4) | mac_temp[13]);;
        sa.sa_data[5]=((mac_temp[15] << 4) | mac_temp[16]);;
        phydev->attached_dev->netdev_ops->ndo_set_mac_address(phydev->attached_dev, &sa);//call the set MAC address function
        //rudolph add sat the lan MAC ADDRESS end

        /*check phy power*/
        val = phy_read(phydev, 0x0);

        if (val & BMCR_PDOWN)
                phy_write(phydev, 0x0, (val & ~BMCR_PDOWN));

        return 0;
}

static struct fec_platform_data fec_data __initdata = {
        .init = mx6q_sabresd_fec_phy_init,
        .phy = PHY_INTERFACE_MODE_RGMII,
#ifdef CONFIG_MX6_ENET_IRQ_TO_GPIO
        .gpio_irq = MX6_ENET_IRQ,
#endif
};

static void mx6q_sabresd_eeprom_setup(struct memory_accessor *mem_acc, void *context)
{
        eeprom_read_mac_address(mem_acc, fec_data.mac);
}

static struct at24_platform_data eeprom = {
        .byte_len       = (16*1024)/8,
        .page_size      = 8,
        .setup          = mx6q_sabresd_eeprom_setup,
};

//rudolph add read eerpom end


/* 
static struct at24_platform_data cm_fx6_eeprom_pdata = {
        .byte_len       = 256,
        .page_size      = 16,
    .setup        = cm_fx6_eeprom_setup,
};
*/
/*
static struct fec_platform_data fec_data __initdata = {
	.init = mx6q_sabresd_fec_phy_init,
	.phy = PHY_INTERFACE_MODE_RGMII,        
#ifdef CONFIG_MX6_ENET_IRQ_TO_GPIO
	.gpio_irq = MX6_ENET_IRQ,
#endif
};
*/
static int mx6q_sabresd_spi_cs[] = {
	SABRESD_CSPI1_CS0,
};

static const struct spi_imx_master mx6q_sabresd_spi_data __initconst = {
	.chipselect     = mx6q_sabresd_spi_cs,
	.num_chipselect = ARRAY_SIZE(mx6q_sabresd_spi_cs),
};

#if defined(CONFIG_MTD_M25P80) || defined(CONFIG_MTD_M25P80_MODULE)
static struct mtd_partition imx6_sabresd_spi_nor_partitions[] = {
	{
	 .name = "bootloader",
	 .offset = 0,
	 .size = 0x00100000,
	},
	{
	 .name = "kernel",
	 .offset = MTDPART_OFS_APPEND,
	 .size = MTDPART_SIZ_FULL,
	},
};

static struct flash_platform_data imx6_sabresd__spi_flash_data = {
	.name = "m25p80",
	.parts = imx6_sabresd_spi_nor_partitions,
	.nr_parts = ARRAY_SIZE(imx6_sabresd_spi_nor_partitions),
	.type = "sst25vf016b",
};
#endif

static struct spi_board_info imx6_sabresd_spi_nor_device[] __initdata = {
#if defined(CONFIG_MTD_M25P80)
	{
		.modalias = "m25p80",
		.max_speed_hz = 20000000, /* max spi clock (SCK) speed in HZ */
		.bus_num = 0,
		.chip_select = 0,
		.platform_data = &imx6_sabresd__spi_flash_data,
	},
#endif
};

static void spi_device_init(void)
{
	spi_register_board_info(imx6_sabresd_spi_nor_device,
				ARRAY_SIZE(imx6_sabresd_spi_nor_device));

}
static struct imx_ssi_platform_data mx6_sabresd_ssi_pdata = {
	.flags = IMX_SSI_DMA | IMX_SSI_SYN,
};


static struct platform_device mx6_sabresd_audio_wm8962_device = {
	.name = "imx-wm8962",
};

static struct mxc_audio_platform_data wm8962_data;

static int wm8962_clk_enable(int enable)
{
	if (enable)
		clk_enable(clko);
	else
		clk_disable(clko);

	return 0;
}

static int mxc_wm8962_init(void)
{
	int rate;

	clko = clk_get(NULL, "clko_clk");
	if (IS_ERR(clko)) {
		pr_err("can't get CLKO clock.\n");
		return PTR_ERR(clko);
	}
	/* both audio codec and comera use CLKO clk*/
	rate = clk_round_rate(clko, 24000000);
	clk_set_rate(clko, rate);

	wm8962_data.sysclk = rate;

	return 0;
}

static struct wm8962_pdata wm8962_config_data = {
	.gpio_init = {
		[2] = WM8962_GPIO_FN_DMICCLK,
		[4] = 0x8000 | WM8962_GPIO_FN_DMICDAT,
	},
};

static struct mxc_audio_platform_data wm8962_data = {
	.ssi_num = 1,
	.src_port = 2,
	.ext_port = 3,
	.hp_gpio = SABRESD_HEADPHONE_DET,
	.hp_active_low = 1,
	.mic_gpio = SABRESD_MICROPHONE_DET,
	.mic_active_low = 1,
	.init = mxc_wm8962_init,
	.clock_enable = wm8962_clk_enable,
};

static struct regulator_consumer_supply sabresd_vwm8962_consumers[] = {
	REGULATOR_SUPPLY("SPKVDD1", "0-001a"),
	REGULATOR_SUPPLY("SPKVDD2", "0-001a"),
};

static struct regulator_init_data sabresd_vwm8962_init = {
	.constraints = {
		.name = "SPKVDD",
		.valid_ops_mask =  REGULATOR_CHANGE_STATUS,
		.boot_on = 1,
	},
	.num_consumer_supplies = ARRAY_SIZE(sabresd_vwm8962_consumers),
	.consumer_supplies = sabresd_vwm8962_consumers,
};

static struct fixed_voltage_config sabresd_vwm8962_reg_config = {
	.supply_name	= "SPKVDD",
	.microvolts		= 4200000,
	.gpio			= SABRESD_CODEC_PWR_EN,
	.enable_high	= 1,
	.enabled_at_boot = 1,
	.init_data		= &sabresd_vwm8962_init,
};

static struct platform_device sabresd_vwm8962_reg_devices = {
	.name	= "reg-fixed-voltage",
	.id		= 4,
	.dev	= {
		.platform_data = &sabresd_vwm8962_reg_config,
	},
};

#define mV_to_uV(mV) (mV * 1000)
#define uV_to_mV(uV) (uV / 1000)
#define V_to_uV(V) (mV_to_uV(V * 1000))
#define uV_to_V(uV) (uV_to_mV(uV) / 1000)

static struct regulator_consumer_supply display_consumers[] = {
	{
		/* MAX17135 */
		.supply = "DISPLAY",
	},
};

static struct regulator_consumer_supply vcom_consumers[] = {
	{
		/* MAX17135 */
		.supply = "VCOM",
	},
};

static struct regulator_consumer_supply v3p3_consumers[] = {
	{
		/* MAX17135 */
		.supply = "V3P3",
	},
};

static struct regulator_init_data max17135_init_data[] = {
	{
		.constraints = {
			.name = "DISPLAY",
			.valid_ops_mask =  REGULATOR_CHANGE_STATUS,
		},
		.num_consumer_supplies = ARRAY_SIZE(display_consumers),
		.consumer_supplies = display_consumers,
	}, {
		.constraints = {
			.name = "GVDD",
			.min_uV = V_to_uV(20),
			.max_uV = V_to_uV(20),
		},
	}, {
		.constraints = {
			.name = "GVEE",
			.min_uV = V_to_uV(-22),
			.max_uV = V_to_uV(-22),
		},
	}, {
		.constraints = {
			.name = "HVINN",
			.min_uV = V_to_uV(-22),
			.max_uV = V_to_uV(-22),
		},
	}, {
		.constraints = {
			.name = "HVINP",
			.min_uV = V_to_uV(20),
			.max_uV = V_to_uV(20),
		},
	}, {
		.constraints = {
			.name = "VCOM",
			.min_uV = mV_to_uV(-4325),
			.max_uV = mV_to_uV(-500),
			.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
			REGULATOR_CHANGE_STATUS,
		},
		.num_consumer_supplies = ARRAY_SIZE(vcom_consumers),
		.consumer_supplies = vcom_consumers,
	}, {
		.constraints = {
			.name = "VNEG",
			.min_uV = V_to_uV(-15),
			.max_uV = V_to_uV(-15),
		},
	}, {
		.constraints = {
			.name = "VPOS",
			.min_uV = V_to_uV(15),
			.max_uV = V_to_uV(15),
		},
	}, {
		.constraints = {
			.name = "V3P3",
			.valid_ops_mask =  REGULATOR_CHANGE_STATUS,
		},
		.num_consumer_supplies = ARRAY_SIZE(v3p3_consumers),
		.consumer_supplies = v3p3_consumers,
	},
};

static struct platform_device max17135_sensor_device = {
	.name = "max17135_sensor",
	.id = 0,
};

static struct max17135_platform_data max17135_pdata __initdata = {
	.vneg_pwrup = 1,
	.gvee_pwrup = 1,
	.vpos_pwrup = 2,
	.gvdd_pwrup = 1,
	.gvdd_pwrdn = 1,
	.vpos_pwrdn = 2,
	.gvee_pwrdn = 1,
	.vneg_pwrdn = 1,
	.gpio_pmic_pwrgood = SABRESD_EPDC_PWRSTAT,
	.gpio_pmic_vcom_ctrl = SABRESD_EPDC_VCOM,
	.gpio_pmic_wakeup = SABRESD_EPDC_PMIC_WAKE,
	.gpio_pmic_v3p3 = SABRESD_EPDC_PWRCTRL0,
	.gpio_pmic_intr = SABRESD_EPDC_PMIC_INT,
	.regulator_init = max17135_init_data,
	.init = max17135_regulator_init,
};

static int __init max17135_regulator_init(struct max17135 *max17135)
{
	struct max17135_platform_data *pdata = &max17135_pdata;
	int i, ret;

	if (!epdc_enabled) {
		printk(KERN_DEBUG
			"max17135_regulator_init abort: EPDC not enabled\n");
		return 0;
	}

	max17135->gvee_pwrup = pdata->gvee_pwrup;
	max17135->vneg_pwrup = pdata->vneg_pwrup;
	max17135->vpos_pwrup = pdata->vpos_pwrup;
	max17135->gvdd_pwrup = pdata->gvdd_pwrup;
	max17135->gvdd_pwrdn = pdata->gvdd_pwrdn;
	max17135->vpos_pwrdn = pdata->vpos_pwrdn;
	max17135->vneg_pwrdn = pdata->vneg_pwrdn;
	max17135->gvee_pwrdn = pdata->gvee_pwrdn;

	max17135->max_wait = pdata->vpos_pwrup + pdata->vneg_pwrup +
		pdata->gvdd_pwrup + pdata->gvee_pwrup;

	max17135->gpio_pmic_pwrgood = pdata->gpio_pmic_pwrgood;
	max17135->gpio_pmic_vcom_ctrl = pdata->gpio_pmic_vcom_ctrl;
	max17135->gpio_pmic_wakeup = pdata->gpio_pmic_wakeup;
	max17135->gpio_pmic_v3p3 = pdata->gpio_pmic_v3p3;
	max17135->gpio_pmic_intr = pdata->gpio_pmic_intr;

	gpio_request(max17135->gpio_pmic_wakeup, "epdc-pmic-wake");
	gpio_direction_output(max17135->gpio_pmic_wakeup, 0);

	gpio_request(max17135->gpio_pmic_vcom_ctrl, "epdc-vcom");
	gpio_direction_output(max17135->gpio_pmic_vcom_ctrl, 0);

	gpio_request(max17135->gpio_pmic_v3p3, "epdc-v3p3");
	gpio_direction_output(max17135->gpio_pmic_v3p3, 0);

	gpio_request(max17135->gpio_pmic_intr, "epdc-pmic-int");
	gpio_direction_input(max17135->gpio_pmic_intr);

	gpio_request(max17135->gpio_pmic_pwrgood, "epdc-pwrstat");
	gpio_direction_input(max17135->gpio_pmic_pwrgood);

	max17135->vcom_setup = false;
	max17135->init_done = false;

	for (i = 0; i < MAX17135_NUM_REGULATORS; i++) {
		ret = max17135_register_regulator(max17135, i,
			&pdata->regulator_init[i]);
		if (ret != 0) {
			printk(KERN_ERR"max17135 regulator init failed: %d\n",
				ret);
			return ret;
		}
	}

	/*
	 * TODO: We cannot enable full constraints for now, since
	 * it results in the PFUZE regulators being disabled
	 * at the end of boot, which disables critical regulators.
	 */
	/*regulator_has_full_constraints();*/

	return 0;
}
static struct imxi2c_platform_data mx6q_sabresd_i2c_data = {
	.bitrate = 100000,					
};

static struct imxi2c_platform_data wm_i2c_data = {
        .bitrate = 400000,                                      //kevin this speed is for nfc
};

#ifdef CONFIG_SENSORS_TSL2563
static struct tsl2563_platform_data rx51_tsl2563_platform_data={
	.cover_comp_gain=16,
};
#endif

static struct i2c_board_info mxc_i2c0_board_info[] __initdata = {
	{
		I2C_BOARD_INFO("wm89**", 0x1a),
	},
};

static struct i2c_board_info mxc_i2c1_board_info[] __initdata = {

	{
		I2C_BOARD_INFO("mxc_hdmi_i2c", 0x50),
	},

	{
		/*winmate frank fixed  
		    HDMI DDC seat because of a conflict with the eeprom seat into 0x70, 
		    part of the hardware will eeprom (24c164) A2 Low -> High
		*/
                		I2C_BOARD_INFO("24c16", 0x70),
		.platform_data = &eeprom,//rudolph add for read eeprom function
        	},

	{
		I2C_BOARD_INFO("max17135", 0x48),
		.platform_data = &max17135_pdata,
	},
#ifdef CONFIG_SENSORS_TSL2563
	{
		I2C_BOARD_INFO("tsl2563", 0x49),
		.platform_data = &rx51_tsl2563_platform_data,
		.irq=0,
	},
#endif

#ifdef CONFIG_PN544_NFC
	{
		I2C_BOARD_INFO("pn544", 0x28),
		.irq = gpio_to_irq(NFC_IRQ),
	},
#endif
};


static struct i2c_board_info mxc_i2c2_board_info[] __initdata = {

#ifdef TOUCHSCREEN_EGALAX_7200
	{
		I2C_BOARD_INFO("egalax_i2c_7200", 0x4),
		.irq = gpio_to_irq(SABRESD_CAP_TCH_INT0),
	},
#endif
#ifdef CONFIG_TOUCHSCREEN_FT5X06_TS	
	{
		I2C_BOARD_INFO("ft5x06_ts", 0x38),
		.irq = gpio_to_irq(SABRESD_CAP_TCH_INT0),
	},
#endif
	{
		I2C_BOARD_INFO("mxc_ldb_i2c", 0x50),
		.platform_data = (void *)0,	/* lvds port0 */
	},
	{//winmate Wich add for RTC
		I2C_BOARD_INFO("m41t82", 0x68),
	},
};

static int epdc_get_pins(void)
{
	int ret = 0;

	/* Claim GPIOs for EPDC pins - used during power up/down */
	ret |= gpio_request(SABRESD_EPDC_SDDO_0, "epdc_d0");
	ret |= gpio_request(SABRESD_EPDC_SDDO_1, "epdc_d1");
	ret |= gpio_request(SABRESD_EPDC_SDDO_2, "epdc_d2");
	ret |= gpio_request(SABRESD_EPDC_SDDO_3, "epdc_d3");
	ret |= gpio_request(SABRESD_EPDC_SDDO_4, "epdc_d4");
	ret |= gpio_request(SABRESD_EPDC_SDDO_5, "epdc_d5");
	ret |= gpio_request(SABRESD_EPDC_SDDO_6, "epdc_d6");
	ret |= gpio_request(SABRESD_EPDC_SDDO_7, "epdc_d7");
	ret |= gpio_request(SABRESD_EPDC_GDCLK, "epdc_gdclk");
	ret |= gpio_request(SABRESD_EPDC_GDSP, "epdc_gdsp");
	ret |= gpio_request(SABRESD_EPDC_GDOE, "epdc_gdoe");
	ret |= gpio_request(SABRESD_EPDC_GDRL, "epdc_gdrl");
	ret |= gpio_request(SABRESD_EPDC_SDCLK, "epdc_sdclk");
	ret |= gpio_request(SABRESD_EPDC_SDOE, "epdc_sdoe");
	ret |= gpio_request(SABRESD_EPDC_SDLE, "epdc_sdle");
	ret |= gpio_request(SABRESD_EPDC_SDSHR, "epdc_sdshr");
	ret |= gpio_request(SABRESD_EPDC_BDR0, "epdc_bdr0");
	ret |= gpio_request(SABRESD_EPDC_SDCE0, "epdc_sdce0");
	ret |= gpio_request(SABRESD_EPDC_SDCE1, "epdc_sdce1");
	ret |= gpio_request(SABRESD_EPDC_SDCE2, "epdc_sdce2");

	return ret;
}

static void epdc_put_pins(void)
{
	gpio_free(SABRESD_EPDC_SDDO_0);
	gpio_free(SABRESD_EPDC_SDDO_1);
	gpio_free(SABRESD_EPDC_SDDO_2);
	gpio_free(SABRESD_EPDC_SDDO_3);
	gpio_free(SABRESD_EPDC_SDDO_4);
	gpio_free(SABRESD_EPDC_SDDO_5);
	gpio_free(SABRESD_EPDC_SDDO_6);
	gpio_free(SABRESD_EPDC_SDDO_7);
	gpio_free(SABRESD_EPDC_GDCLK);
	gpio_free(SABRESD_EPDC_GDSP);
	gpio_free(SABRESD_EPDC_GDOE);
	gpio_free(SABRESD_EPDC_GDRL);
	gpio_free(SABRESD_EPDC_SDCLK);
	gpio_free(SABRESD_EPDC_SDOE);
	gpio_free(SABRESD_EPDC_SDLE);
	gpio_free(SABRESD_EPDC_SDSHR);
	gpio_free(SABRESD_EPDC_BDR0);
	gpio_free(SABRESD_EPDC_SDCE0);
	gpio_free(SABRESD_EPDC_SDCE1);
	gpio_free(SABRESD_EPDC_SDCE2);
}

static void epdc_enable_pins(void)
{
	/* Configure MUX settings to enable EPDC use */
	mxc_iomux_v3_setup_multiple_pads(mx6dl_sabresd_epdc_enable_pads, \
				ARRAY_SIZE(mx6dl_sabresd_epdc_enable_pads));

	gpio_direction_input(SABRESD_EPDC_SDDO_0);
	gpio_direction_input(SABRESD_EPDC_SDDO_1);
	gpio_direction_input(SABRESD_EPDC_SDDO_2);
	gpio_direction_input(SABRESD_EPDC_SDDO_3);
	gpio_direction_input(SABRESD_EPDC_SDDO_4);
	gpio_direction_input(SABRESD_EPDC_SDDO_5);
	gpio_direction_input(SABRESD_EPDC_SDDO_6);
	gpio_direction_input(SABRESD_EPDC_SDDO_7);
	gpio_direction_input(SABRESD_EPDC_GDCLK);
	gpio_direction_input(SABRESD_EPDC_GDSP);
	gpio_direction_input(SABRESD_EPDC_GDOE);
	gpio_direction_input(SABRESD_EPDC_GDRL);
	gpio_direction_input(SABRESD_EPDC_SDCLK);
	gpio_direction_input(SABRESD_EPDC_SDOE);
	gpio_direction_input(SABRESD_EPDC_SDLE);
	gpio_direction_input(SABRESD_EPDC_SDSHR);
	gpio_direction_input(SABRESD_EPDC_BDR0);
	gpio_direction_input(SABRESD_EPDC_SDCE0);
	gpio_direction_input(SABRESD_EPDC_SDCE1);
	gpio_direction_input(SABRESD_EPDC_SDCE2);
}

static void epdc_disable_pins(void)
{
	/* Configure MUX settings for EPDC pins to
	 * GPIO and drive to 0. */
	mxc_iomux_v3_setup_multiple_pads(mx6dl_sabresd_epdc_disable_pads, \
				ARRAY_SIZE(mx6dl_sabresd_epdc_disable_pads));

	gpio_direction_output(SABRESD_EPDC_SDDO_0, 0);
	gpio_direction_output(SABRESD_EPDC_SDDO_1, 0);
	gpio_direction_output(SABRESD_EPDC_SDDO_2, 0);
	gpio_direction_output(SABRESD_EPDC_SDDO_3, 0);
	gpio_direction_output(SABRESD_EPDC_SDDO_4, 0);
	gpio_direction_output(SABRESD_EPDC_SDDO_5, 0);
	gpio_direction_output(SABRESD_EPDC_SDDO_6, 0);
	gpio_direction_output(SABRESD_EPDC_SDDO_7, 0);
	gpio_direction_output(SABRESD_EPDC_GDCLK, 0);
	gpio_direction_output(SABRESD_EPDC_GDSP, 0);
	gpio_direction_output(SABRESD_EPDC_GDOE, 0);
	gpio_direction_output(SABRESD_EPDC_GDRL, 0);
	gpio_direction_output(SABRESD_EPDC_SDCLK, 0);
	gpio_direction_output(SABRESD_EPDC_SDOE, 0);
	gpio_direction_output(SABRESD_EPDC_SDLE, 0);
	gpio_direction_output(SABRESD_EPDC_SDSHR, 0);
	gpio_direction_output(SABRESD_EPDC_BDR0, 0);
	gpio_direction_output(SABRESD_EPDC_SDCE0, 0);
	gpio_direction_output(SABRESD_EPDC_SDCE1, 0);
	gpio_direction_output(SABRESD_EPDC_SDCE2, 0);
}

static struct fb_videomode e60_v110_mode = {
	.name = "E60_V110",
	.refresh = 50,
	.xres = 800,
	.yres = 600,
	.pixclock = 18604700,
	.left_margin = 8,
	.right_margin = 178,
	.upper_margin = 4,
	.lower_margin = 10,
	.hsync_len = 20,
	.vsync_len = 4,
	.sync = 0,
	.vmode = FB_VMODE_NONINTERLACED,
	.flag = 0,
};
static struct fb_videomode e60_v220_mode = {
	.name = "E60_V220",
	.refresh = 85,
	.xres = 800,
	.yres = 600,
	.pixclock = 30000000,
	.left_margin = 8,
	.right_margin = 164,
	.upper_margin = 4,
	.lower_margin = 8,
	.hsync_len = 4,
	.vsync_len = 1,
	.sync = 0,
	.vmode = FB_VMODE_NONINTERLACED,
	.flag = 0,
	.refresh = 85,
	.xres = 800,
	.yres = 600,
};
static struct fb_videomode e060scm_mode = {
	.name = "E060SCM",
	.refresh = 85,
	.xres = 800,
	.yres = 600,
	.pixclock = 26666667,
	.left_margin = 8,
	.right_margin = 100,
	.upper_margin = 4,
	.lower_margin = 8,
	.hsync_len = 4,
	.vsync_len = 1,
	.sync = 0,
	.vmode = FB_VMODE_NONINTERLACED,
	.flag = 0,
};
static struct fb_videomode e97_v110_mode = {
	.name = "E97_V110",
	.refresh = 50,
	.xres = 1200,
	.yres = 825,
	.pixclock = 32000000,
	.left_margin = 12,
	.right_margin = 128,
	.upper_margin = 4,
	.lower_margin = 10,
	.hsync_len = 20,
	.vsync_len = 4,
	.sync = 0,
	.vmode = FB_VMODE_NONINTERLACED,
	.flag = 0,
};

static struct imx_epdc_fb_mode panel_modes[] = {
	{
		&e60_v110_mode,
		4,      /* vscan_holdoff */
		10,     /* sdoed_width */
		20,     /* sdoed_delay */
		10,     /* sdoez_width */
		20,     /* sdoez_delay */
		428,    /* gdclk_hp_offs */
		20,     /* gdsp_offs */
		0,      /* gdoe_offs */
		1,      /* gdclk_offs */
		1,      /* num_ce */
	},
	{
		&e60_v220_mode,
		4,      /* vscan_holdoff */
		10,     /* sdoed_width */
		20,     /* sdoed_delay */
		10,     /* sdoez_width */
		20,     /* sdoez_delay */
		465,    /* gdclk_hp_offs */
		20,     /* gdsp_offs */
		0,      /* gdoe_offs */
		9,      /* gdclk_offs */
		1,      /* num_ce */
	},
	{
		&e060scm_mode,
		4,      /* vscan_holdoff */
		10,     /* sdoed_width */
		20,     /* sdoed_delay */
		10,     /* sdoez_width */
		20,     /* sdoez_delay */
		419,    /* gdclk_hp_offs */
		20,     /* gdsp_offs */
		0,      /* gdoe_offs */
		5,      /* gdclk_offs */
		1,      /* num_ce */
	},
	{
		&e97_v110_mode,
		8,      /* vscan_holdoff */
		10,     /* sdoed_width */
		20,     /* sdoed_delay */
		10,     /* sdoez_width */
		20,     /* sdoez_delay */
		632,    /* gdclk_hp_offs */
		20,     /* gdsp_offs */
		0,      /* gdoe_offs */
		1,      /* gdclk_offs */
		3,      /* num_ce */
	}
};

static struct imx_epdc_fb_platform_data epdc_data = {
	.epdc_mode = panel_modes,
	.num_modes = ARRAY_SIZE(panel_modes),
	.get_pins = epdc_get_pins,
	.put_pins = epdc_put_pins,
	.enable_pins = epdc_enable_pins,
	.disable_pins = epdc_disable_pins,
};

static void imx6q_sabresd_usbotg_vbus(bool on)
{
	if (on)
		gpio_set_value(SABRESD_USB_OTG_PWR, 1);
	else
		gpio_set_value(SABRESD_USB_OTG_PWR, 0);
}

static void imx6q_sabresd_host1_vbus(bool on)
{
	if (on){
		gpio_set_value(SABRESD_USB_H1_PWR, 1);
		
		gpio_set_value(SABRESD_USBP0_EN, 1);
		gpio_set_value(SABRESD_USBP1_EN, 1);
		gpio_set_value(SABRESD_USBP2_EN, 1);
		gpio_set_value(SABRESD_USBP3_EN, 1);
	}
	else{
		gpio_set_value(SABRESD_USB_H1_PWR, 0);
		
		gpio_set_value(SABRESD_USBP0_EN, 0);
		gpio_set_value(SABRESD_USBP1_EN, 0);
		gpio_set_value(SABRESD_USBP2_EN, 0);
		gpio_set_value(SABRESD_USBP3_EN, 0);
	}
}

static void __init imx6q_sabresd_init_usb(void)
{
	int ret = 0;

	imx_otg_base = MX6_IO_ADDRESS(MX6Q_USB_OTG_BASE_ADDR); 	
	/* disable external charger detect,
	 * or it will affect signal quality at dp .
	 */
	ret = gpio_request(SABRESD_USB_OTG_PWR, "usb-pwr");
	if (ret) {
		pr_err("failed to get GPIO SABRESD_USB_OTG_PWR: %d\n",
			ret);
		return;
	}
	gpio_direction_output(SABRESD_USB_OTG_PWR, 1);                   /*USB_OTG_PWR is output*/
	/* keep USB host1 VBUS always on */
	ret = gpio_request(SABRESD_USB_H1_PWR, "usb-h1-pwr");

	ret = gpio_request(SABRESD_USBP0_EN, "USBP0");
	ret = gpio_request(SABRESD_USBP1_EN, "USBP1");
	ret = gpio_request(SABRESD_USBP2_EN, "USBP2");
	ret = gpio_request(SABRESD_USBP3_EN, "USBP3");
	
	if (ret) {
		pr_err("failed to get GPIO SABRESD_USB_H1_PWR: %d\n",
			ret);
		return;
	}
	gpio_direction_output(SABRESD_USB_H1_PWR, 1);                     /*USB_H1_PWR is output*/

	gpio_direction_output(SABRESD_USBP0_EN, 1);
	gpio_direction_output(SABRESD_USBP1_EN, 1);
	gpio_direction_output(SABRESD_USBP2_EN, 1);
	gpio_direction_output(SABRESD_USBP3_EN, 1);
	
	mxc_iomux_set_gpr_register(1, 13, 1, 0);

	mx6_set_otghost_vbus_func(imx6q_sabresd_usbotg_vbus);
	mx6_set_host1_vbus_func(imx6q_sabresd_host1_vbus);

}

/* HW Initialization, if return 0, initialization is successful. */
static int mx6q_sabresd_sata_init(struct device *dev, void __iomem *addr)
{
	u32 tmpdata;
	int ret = 0;
	struct clk *clk;

	sata_clk = clk_get(dev, "imx_sata_clk");

	if (IS_ERR(sata_clk)) {
		dev_err(dev, "no sata clock.\n");
		return PTR_ERR(sata_clk);
	}
	ret = clk_enable(sata_clk);
	if (ret) {
		dev_err(dev, "can't enable sata clock.\n");
		goto put_sata_clk;
	}

	/* Set PHY Paremeters, two steps to configure the GPR13,
	 * one write for rest of parameters, mask of first write is 0x07FFFFFD,
	 * and the other one write for setting the mpll_clk_off_b
	 *.rx_eq_val_0(iomuxc_gpr13[26:24]),
	 *.los_lvl(iomuxc_gpr13[23:19]),
	 *.rx_dpll_mode_0(iomuxc_gpr13[18:16]),
	 *.sata_speed(iomuxc_gpr13[15]),
	 *.mpll_ss_en(iomuxc_gpr13[14]),
	 *.tx_atten_0(iomuxc_gpr13[13:11]),
	 *.tx_boost_0(iomuxc_gpr13[10:7]),
	 *.tx_lvl(iomuxc_gpr13[6:2]),
	 *.mpll_ck_off(iomuxc_gpr13[1]),
	 *.tx_edgerate_0(iomuxc_gpr13[0]),
	 */
	tmpdata = readl(IOMUXC_GPR13);
	writel(((tmpdata & ~0x07FFFFFD) | 0x0593A044), IOMUXC_GPR13);

	/* enable SATA_PHY PLL */
	tmpdata = readl(IOMUXC_GPR13);
	writel(((tmpdata & ~0x2) | 0x2), IOMUXC_GPR13);

	/* Get the AHB clock rate, and configure the TIMER1MS reg later */
	clk = clk_get(NULL, "ahb");
	if (IS_ERR(clk)) {
		dev_err(dev, "no ahb clock.\n");
		ret = PTR_ERR(clk);
		goto release_sata_clk;
	}
	tmpdata = clk_get_rate(clk) / 1000;
	clk_put(clk);

#ifdef CONFIG_SATA_AHCI_PLATFORM

	ret = sata_init(addr, tmpdata);
	if (ret == 0)
		return ret;
#else

	usleep_range(1000, 2000);
	/* AHCI PHY enter into PDDQ mode if the AHCI module is not enabled */
	tmpdata = readl(addr + PORT_PHY_CTL);
	writel(tmpdata | PORT_PHY_CTL_PDDQ_LOC, addr + PORT_PHY_CTL);
	pr_info("No AHCI save PWR: PDDQ %s\n", ((readl(addr + PORT_PHY_CTL)
					>> 20) & 1) ? "enabled" : "disabled");
#endif

release_sata_clk:
	/* disable SATA_PHY PLL */
	writel((readl(IOMUXC_GPR13) & ~0x2), IOMUXC_GPR13);
	clk_disable(sata_clk);
put_sata_clk:
	clk_put(sata_clk);

	return ret;
}

#ifdef CONFIG_SATA_AHCI_PLATFORM
static void mx6q_sabresd_sata_exit(struct device *dev)
{
	clk_disable(sata_clk);
	clk_put(sata_clk);
}

static struct ahci_platform_data mx6q_sabresd_sata_data = {
	.init = mx6q_sabresd_sata_init,
	.exit = mx6q_sabresd_sata_exit,
};
#endif

static void mx6q_sabresd_flexcan0_switch(int enable)
{
	if (enable) {
		/*gpio_set_value(SABRESD_CAN1_STBY, 1);*/
	

		
	} else {
		/*gpio_set_value(SABRESD_CAN1_STBY, 0);*/
	}
}

static const struct flexcan_platform_data
	mx6q_sabresd_flexcan0_pdata __initconst = {
	.transceiver_switch = mx6q_sabresd_flexcan0_switch,
};

static const struct flexcan_platform_data
	mx6q_sabresd_flexcan1_pdata __initconst = {
	.transceiver_switch = mx6q_sabresd_flexcan0_switch,
};

static struct viv_gpu_platform_data imx6q_gpu_pdata __initdata = {
	.reserved_mem_size = SZ_128M + SZ_64M - SZ_16M,
};

static struct imx_asrc_platform_data imx_asrc_data = {
	.channel_bits = 4,
	.clk_map_ver = 2,
};

static void mx6_reset_mipi_dsi(void)
{
	gpio_set_value(SABRESD_DISP_PWR_EN, 1);
	gpio_set_value(SABRESD_DISP_RST_B, 1);
	udelay(10);
	gpio_set_value(SABRESD_DISP_RST_B, 0);
	udelay(50);
	gpio_set_value(SABRESD_DISP_RST_B, 1);

	msleep(120);
	/*
	 * it needs to delay 120ms minimum for reset complete
	 */
}    


static struct mipi_dsi_platform_data mipi_dsi_pdata = {
	.ipu_id		= 0,
	.disp_id	= 1,
	.lcd_panel	= "TRULY-WVGA",
	.reset		= mx6_reset_mipi_dsi,
};

static struct ipuv3_fb_platform_data sabresd_fb_data[] = {
	{ /*fb0*/
	.disp_dev = "ldb",
	.interface_pix_fmt = PANEL_PIX_FMT,//winmate frank add	  /*rudolph modify 24 to 666*/
	.mode_str = "LDB-XGA",
	.default_bpp = 32,// wiinmate frank 24->32 bpp=32����32���m
	.int_clk = false,
	.late_init = false,
	},{
        .disp_dev = "ldb",
        .interface_pix_fmt = IPU_PIX_FMT_RGB24,
        .mode_str = "LDB-XGA",
        .default_bpp = 24,
        .int_clk = false,
        .late_init = false,
        },{
	.disp_dev = "hdmi",
	.interface_pix_fmt = IPU_PIX_FMT_RGB24,
	.mode_str = "1920x1080M@60",
	.default_bpp = 32,
	.int_clk = false,
	.late_init = false,
	},
#if 0	
	{
	.disp_dev = "ldb",
	.interface_pix_fmt = IPU_PIX_FMT_RGB24,
	.mode_str = "LDB-XGA",
	.default_bpp = 24,
	.int_clk = false,
	.late_init = false,
	},
#endif
};

static void hdmi_init(int ipu_id, int disp_id)
{
	int hdmi_mux_setting;

	if ((ipu_id > 1) || (ipu_id < 0)) {
		pr_err("Invalid IPU select for HDMI: %d. Set to 0\n", ipu_id);
		ipu_id = 0;
	}

	if ((disp_id > 1) || (disp_id < 0)) {
		pr_err("Invalid DI select for HDMI: %d. Set to 0\n", disp_id);
		disp_id = 0;
	}

	/* Configure the connection between IPU1/2 and HDMI */
	hdmi_mux_setting = 2*ipu_id + disp_id;

	/* GPR3, bits 2-3 = HDMI_MUX_CTL */
	mxc_iomux_set_gpr_register(3, 2, 2, hdmi_mux_setting);

	/* Set HDMI event as SDMA event2 while Chip version later than TO1.2 */
	if (hdmi_SDMA_check())
		mxc_iomux_set_gpr_register(0, 0, 1, 1);
}

/* On mx6x sabresd board i2c2 iomux with hdmi ddc,
 * the pins default work at i2c2 function,
 when hdcp enable, the pins should work at ddc function */

static void hdmi_enable_ddc_pin(void)
{
	if (cpu_is_mx6dl())
		mxc_iomux_v3_setup_multiple_pads(mx6dl_sabresd_hdmi_ddc_pads,
			ARRAY_SIZE(mx6dl_sabresd_hdmi_ddc_pads));
	else
		mxc_iomux_v3_setup_multiple_pads(mx6q_sabresd_hdmi_ddc_pads,
			ARRAY_SIZE(mx6q_sabresd_hdmi_ddc_pads));
}

static void hdmi_disable_ddc_pin(void)
{
	if (cpu_is_mx6dl())
		mxc_iomux_v3_setup_multiple_pads(mx6dl_sabresd_i2c2_pads,
			ARRAY_SIZE(mx6dl_sabresd_i2c2_pads));
	else
		mxc_iomux_v3_setup_multiple_pads(mx6q_sabresd_i2c2_pads,
			ARRAY_SIZE(mx6q_sabresd_i2c2_pads));
}

static struct fsl_mxc_hdmi_platform_data hdmi_data = {
	.init = hdmi_init,
	.enable_pins = hdmi_enable_ddc_pin,
	.disable_pins = hdmi_disable_ddc_pin,
};

static struct fsl_mxc_hdmi_core_platform_data hdmi_core_data = {
	.ipu_id = 1,
	.disp_id = 0,
};

static struct fsl_mxc_lcd_platform_data lcdif_data = {
	.ipu_id = 0,
	.disp_id = 0,
	.default_ifmt = IPU_PIX_FMT_RGB565,
};

static struct fsl_mxc_ldb_platform_data ldb_data = {
	.ipu_id = 0,
	.disp_id = 1,
	.ext_ref = 1,
	.mode =  PANEL_LDB_MODE,//winmate frank   //modified by kevin      original:LDB_SEP1  controlled lvds channel by ldb.c
	.sec_ipu_id = 0,
	.sec_disp_id = 0,
};

static struct imx_ipuv3_platform_data ipu_data[] = {
	{
	.rev = 4,
	.csi_clk[0] = "clko_clk",
	.bypass_reset = false,
	}, {
	.rev = 4,
	.csi_clk[0] = "clko_clk",
	.bypass_reset = false,
	},
};

static struct ion_platform_data imx_ion_data = {
	.nr = 1,
	.heaps = {
		{
		.id = 0,
		.type = ION_HEAP_TYPE_CARVEOUT,
		.name = "vpu_ion",
		.size = SZ_16M,
		.cacheable = 0,
		},
	},
};

static struct fsl_mxc_capture_platform_data capture_data[] = {
	{
		.csi = 0,
		.ipu = 0,
		.mclk_source = 0,
		.is_mipi = 0,
	}, {
		.csi = 1,
		.ipu = 0,
		.mclk_source = 0,
		.is_mipi = 1,
	},
};

static void sabresd_suspend_enter(void)
{
	/* suspend preparation */
	/* Disable AUX 5V */
	gpio_set_value(SABRESD_AUX_5V_EN, 0);
}

static void sabresd_suspend_exit(void)
{
	/* resume restore */
	/* Enable AUX 5V */
	gpio_set_value(SABRESD_AUX_5V_EN, 1);
}
static const struct pm_platform_data mx6q_sabresd_pm_data __initconst = {
	.name = "imx_pm",
	.suspend_enter = sabresd_suspend_enter,
	.suspend_exit = sabresd_suspend_exit,
};

static struct regulator_consumer_supply sabresd_vmmc_consumers[] = {
	REGULATOR_SUPPLY("vmmc", "sdhci-esdhc-imx.1"),
	REGULATOR_SUPPLY("vmmc", "sdhci-esdhc-imx.2"),
	REGULATOR_SUPPLY("vmmc", "sdhci-esdhc-imx.3"),
};

static struct regulator_init_data sabresd_vmmc_init = {
	.num_consumer_supplies = ARRAY_SIZE(sabresd_vmmc_consumers),
	.consumer_supplies = sabresd_vmmc_consumers,
};

static struct fixed_voltage_config sabresd_vmmc_reg_config = {
	.supply_name		= "vmmc",
	.microvolts		= 3300000,
	.gpio			= -1,
	.init_data		= &sabresd_vmmc_init,
};

static struct platform_device sabresd_vmmc_reg_devices = {
	.name	= "reg-fixed-voltage",
	.id	= 3,
	.dev	= {
		.platform_data = &sabresd_vmmc_reg_config,
	},
};

static int __init imx6q_init_audio(void)
{

		platform_device_register(&sabresd_vwm8962_reg_devices);

		mxc_register_device(&mx6_sabresd_audio_wm8962_device,
				    &wm8962_data);
		imx6q_add_imx_ssi(1, &mx6_sabresd_ssi_pdata);

		mxc_wm8962_init();

	

	return 0;
}


#if defined(CONFIG_KEYBOARD_GPIO) || defined(CONFIG_KEYBOARD_GPIO_MODULE)
#define GPIO_BUTTON(gpio_num, ev_code, act_low, descr, wake, debounce)	\
{								\
	.gpio		= gpio_num,				\
	.type		= EV_KEY,				\
	.code		= ev_code,				\
	.active_low	= act_low,				\
	.desc		= "btn " descr,				\
	.wakeup		= wake,					\
	.debounce_interval = debounce,				\
}

static struct gpio_keys_button new_sabresd_buttons[] = {
	GPIO_BUTTON(SABRESD_POWER_OFF, KEY_POWER, 1, "power-key", 1, 1),
#ifdef CONFIG_WM_HW_KEY_K5
	GPIO_BUTTON(DIN0, KEY_VOLUMEUP, 0, "volume-up", 0, 1),
	GPIO_BUTTON(DIN1, KEY_VOLUMEDOWN, 0, "volume-down", 0, 1),
	GPIO_BUTTON(DIN2, KEY_HOME, 0, "key-home", 1, 1),
	GPIO_BUTTON(DIN3, KEY_MENU, 0, "key-menu", 0, 1),
#endif
	//GPIO_BUTTON(NULL, KEY_BTN_X, 1, "fun_btn", 0, 1),
	//GPIO_BUTTON(NULL, KEY_BUTTON_R2, 1, "fun_btn", 0, 1),
	//GPIO_BUTTON(NULL, KEY_BUTTON_L2, 1, "fun_btn", 0, 1),
};

static struct gpio_keys_platform_data new_sabresd_button_data = {
	.buttons	= new_sabresd_buttons,
	.nbuttons	= ARRAY_SIZE(new_sabresd_buttons),
};

static struct platform_device sabresd_button_device = {
	.name		= "gpio-keys",
	.id		= -1,
	.num_resources  = 0,
};

//kevin add
static struct platform_device wm_nfc_pn544 = {
	.name   = "pn544",
    	.id     = -1,
	//.num_resources  = 0, 		//kevin test
};

static int __init wm_nfc_pn544_init(void)
{
	int retval = 0;	

	retval = platform_device_register(&wm_nfc_pn544);
        printk("wm_nfc_pn544 register ret %d \n", retval);
        if (retval != 0){
                return retval;
        }

	return retval;
}
//kevin end

//willy add
static struct platform_device motion_sensor = {
       .name   = "motion_sensor",
       .id     = -1,
	.dev    = {
                        .platform_data = 0,
                  },

};

static int __init MotionSensorInit(void)
{
       int retval = 0;

       retval = platform_device_register(&motion_sensor);
       printk("motion_sensor register ret %d \n", retval);
       if (retval != 0){
       		return retval;
       }

       return retval;
}
//willy end

static void __init imx6q_add_device_buttons(void)
{
	/* fix me */
	/* For new sabresd(RevB4 ane above) change the
	 * ONOFF key(SW1) design, the SW1 now connect
	 * to GPIO_3_29, it can be use as a general power
	 * key that Android reuired. But those old sabresd
	 * such as RevB or older could not support this
	 * change, so it needs a way to distinguish different
	 * boards. Before board id/rev are defined cleary,
	 * there is a simple way to achive this, that is using
	 * SOC revison to identify differnt board revison.
	 *
	 * With the new sabresd change and SW mapping the
	 * SW1 as power key, below function related to power
	 * key are OK on new sabresd board(B4 or above).
	 * 	1 Act as power button to power on the device when device is power off
	 * 	2 Act as power button to power on the device(need keep press SW1 >5s)
	 *	3 Act as power key to let device suspend/resume
	 *	4 Act screenshort(hold power key and volume down key for 2s)
	 */

		platform_device_add_data(&sabresd_button_device,
				&new_sabresd_button_data,
				sizeof(new_sabresd_button_data));


	platform_device_register(&sabresd_button_device);

}
#else
static void __init imx6q_add_device_buttons(void) {}
#endif

static struct platform_pwm_backlight_data mx6_sabresd_pwm_backlight_data = {
	.pwm_id = 0,
	.lth_brightness = 30,
	.max_brightness = 248,
	.dft_brightness = 128,
	.pwm_period_ns = PANEL_PWM_PERIOD,//winmate frank
};

static struct mxc_dvfs_platform_data sabresd_dvfscore_data = {
	.reg_id = "VDDCORE",
	.soc_id	= "VDDSOC",
	.clk1_id = "cpu_clk",
	.clk2_id = "gpc_dvfs_clk",
	.gpc_cntr_offset = MXC_GPC_CNTR_OFFSET,
	.ccm_cdcr_offset = MXC_CCM_CDCR_OFFSET,
	.ccm_cacrr_offset = MXC_CCM_CACRR_OFFSET,
	.ccm_cdhipr_offset = MXC_CCM_CDHIPR_OFFSET,
	.prediv_mask = 0x1F800,
	.prediv_offset = 11,
	.prediv_val = 3,
	.div3ck_mask = 0xE0000000,
	.div3ck_offset = 29,
	.div3ck_val = 2,
	.emac_val = 0x08,
	.upthr_val = 25,
	.dnthr_val = 9,
	.pncthr_val = 33,
	.upcnt_val = 10,
	.dncnt_val = 10,
	.delay_time = 80,
};

static void __init fixup_mxc_board(struct machine_desc *desc, struct tag *tags,
				   char **cmdline, struct meminfo *mi)
{
	char *str;
	struct tag *t;
	int i = 0;
	struct ipuv3_fb_platform_data *pdata_fb = sabresd_fb_data;

	for_each_tag(t, tags) {
		if (t->hdr.tag == ATAG_CMDLINE) {
			str = t->u.cmdline.cmdline;
			str = strstr(str, "fbmem=");
			if (str != NULL) {
				str += 6;
				pdata_fb[i++].res_size[0] = memparse(str, &str);
				while (*str == ',' &&
					i < ARRAY_SIZE(sabresd_fb_data)) {
					str++;
					pdata_fb[i++].res_size[0] = memparse(str, &str);
				}
			}
			/* ION reserved memory */
			str = t->u.cmdline.cmdline;
			str = strstr(str, "ionmem=");
			if (str != NULL) {
				str += 7;
				imx_ion_data.heaps[0].size = memparse(str, &str);
			}
			/* Primary framebuffer base address */
			str = t->u.cmdline.cmdline;
			str = strstr(str, "fb0base=");
			if (str != NULL) {
				str += 8;
				pdata_fb[0].res_base[0] =
						simple_strtol(str, &str, 16);
			}
			/* GPU reserved memory */
			str = t->u.cmdline.cmdline;
			str = strstr(str, "gpumem=");
			if (str != NULL) {
				str += 7;
				imx6q_gpu_pdata.reserved_mem_size = memparse(str, &str);
			}
			break;
		}
	}
}


#define SNVS_LPCR 0x38
static void mx6_snvs_poweroff(void)
{
	void __iomem *mx6_snvs_base =  MX6_IO_ADDRESS(MX6Q_SNVS_BASE_ADDR);
	u32 value;
	value = readl(mx6_snvs_base + SNVS_LPCR);
	//system power off set register.
	//printk("rudolph add for value=%d \n",value);
	/*set TOP and DP_EN bit*/
	writel(value | 0x60, mx6_snvs_base + SNVS_LPCR);
}

static const struct imx_pcie_platform_data mx6_sabresd_pcie_data __initconst = {
	.pcie_pwr_en	= SABRESD_PCIE_PWR_EN,
	.pcie_rst	= SABRESD_PCIE_RST_B_REVB,
	.pcie_wake_up	= SABRESD_PCIE_WAKE_B,
	.pcie_dis	= SABRESD_PCIE_DIS_B,
	.pcie_power_always_on = 1,
};

#ifdef CONFIG_ANDROID_RAM_CONSOLE
static struct resource ram_console_resource = {
	.name = "android ram console",
	.flags = IORESOURCE_MEM,
};

static struct platform_device android_ram_console = {
	.name = "ram_console",
	.num_resources = 1,
	.resource = &ram_console_resource,
};

static int __init imx6x_add_ram_console(void)
{
	return platform_device_register(&android_ram_console);
}
#else
#define imx6x_add_ram_console() do {} while (0)
#endif

static iomux_v3_cfg_t mx6q_uart5_pads[] = {
	MX6Q_PAD_KEY_ROW1__UART5_RXD,
	MX6Q_PAD_KEY_COL1__UART5_TXD,
	MX6Q_PAD_KEY_COL4__UART5_RTS,
	MX6Q_PAD_KEY_ROW4__UART5_CTS,
	/* gpio for reset */
	MX6Q_PAD_GPIO_2__GPIO_1_2,
};

static iomux_v3_cfg_t mx6dl_uart5_pads[] = {
	MX6DL_PAD_KEY_ROW1__UART5_RXD,
	MX6DL_PAD_KEY_COL1__UART5_TXD,
	MX6DL_PAD_KEY_COL4__UART5_RTS,
	MX6DL_PAD_KEY_ROW4__UART5_CTS,
	/* gpio for reset */
	MX6DL_PAD_GPIO_2__GPIO_1_2,
};
static int __init uart5_setup(char * __unused)
{
	uart5_enabled = 1;
	return 1;
}
__setup("bluetooth", uart5_setup);

static void __init uart5_init(void)
{
	printk(KERN_INFO "uart5 is added\n");
	if (cpu_is_mx6q())
		mxc_iomux_v3_setup_multiple_pads(mx6q_uart5_pads,
				ARRAY_SIZE(mx6q_uart5_pads));
	else if (cpu_is_mx6dl())
		mxc_iomux_v3_setup_multiple_pads(mx6dl_uart5_pads,
				ARRAY_SIZE(mx6dl_uart5_pads));
	imx6q_add_imx_uart(4, &mx6q_sd_uart5_data);
}
//rudolph add for WinmateKeyGen start
static struct platform_device WinmateKeyGen_device = {
        .name   = "WinmateKeyGen",
        .id         = -1,
        .dev    = {
                       .platform_data = 0,
                },
};

static struct platform_device WinmateKeyGen_device1 = {
        .name   = "WinmateKeyGen1",
        .id         = -1,
        .dev    = {
                      .platform_data = 0,
                },
};

static void WinmateKeyGen_Init(void)
{
        platform_device_register(&WinmateKeyGen_device);
        platform_device_register(&WinmateKeyGen_device1);
}

//rudolph add for WinmateKeyGen end

const struct gpio_led indicator_led[] = {
	{.name = "red",		.gpio = 18,		.default_trigger = "none",	.active_low = 0 },
	{.name = "green",	.gpio = 20,		.default_trigger = "none",	.active_low = 0 },
	{.name = "blue",	.gpio = 16,		.default_trigger = "none",	.active_low = 0 }
};

struct gpio_led_platform_data indicator_led_pdata = {
    .num_leds   = ARRAY_SIZE(indicator_led),
    .leds       = indicator_led,
};

static void led_init(void)
{
    gpio_led_register_device(0, &indicator_led_pdata);
}

/*!
 * Board specific initialization.
 */
static void __init mx6_sabresd_board_init(void)
{
	int i;
	int ret;
	struct clk *clko, *clko2;
	struct clk *new_parent;
	int rate;
	struct platform_device *voutdev;

	if (cpu_is_mx6q()) {
		mxc_iomux_v3_setup_multiple_pads(mx6q_sabresd_pads,
			ARRAY_SIZE(mx6q_sabresd_pads));
		gpio_set_value(I2C_RST, 1);	//willy add
                /*
		if (enet_to_gpio_6) {
			iomux_v3_cfg_t enet_gpio_pad =
				MX6Q_PAD_GPIO_6__ENET_IRQ_TO_GPIO_6;
			mxc_iomux_v3_setup_pad(enet_gpio_pad);
		} else {
			iomux_v3_cfg_t i2c3_pad =
				MX6Q_PAD_GPIO_6__I2C3_SDA;
			mxc_iomux_v3_setup_pad(i2c3_pad);
		}*/
	} else if (cpu_is_mx6dl()) {
		mxc_iomux_v3_setup_multiple_pads(mx6dl_sabresd_pads,
			ARRAY_SIZE(mx6dl_sabresd_pads));
		gpio_set_value(I2C_RST, 1);     //willy add
		if (enet_to_gpio_6) {
			iomux_v3_cfg_t enet_gpio_pad =
				MX6DL_PAD_GPIO_6__ENET_IRQ_TO_GPIO_6;
			mxc_iomux_v3_setup_pad(enet_gpio_pad);
		} else {
			iomux_v3_cfg_t i2c3_pad =
				MX6DL_PAD_GPIO_6__I2C3_SDA;
			mxc_iomux_v3_setup_pad(i2c3_pad);
		}
	}


#ifdef CONFIG_FEC_1588
	/* Set GPIO_16 input for IEEE-1588 ts_clk and RMII reference clock
	 * For MX6 GPR1 bit21 meaning:
	 * Bit21:       0 - GPIO_16 pad output
	 *              1 - GPIO_16 pad input
	 */
	 mxc_iomux_set_gpr_register(1, 21, 1, 1);
#endif

	gp_reg_id = sabresd_dvfscore_data.reg_id;
	soc_reg_id = sabresd_dvfscore_data.soc_id;
	mx6q_sabresd_init_uart();
	imx6x_add_ram_console();


	/*
	 * MX6DL/Solo only supports single IPU
	 * The following codes are used to change ipu id
	 * and display id information for MX6DL/Solo. Then
	 * register 1 IPU device and up to 2 displays for
	 * MX6DL/Solo
	 */
	if (cpu_is_mx6dl()) {
		ldb_data.ipu_id = 0;
		ldb_data.disp_id = 1;
		hdmi_core_data.ipu_id = 0;
		hdmi_core_data.disp_id = 0;
		mipi_dsi_pdata.ipu_id = 0;
		mipi_dsi_pdata.disp_id = 1;
		ldb_data.sec_ipu_id = 0;
	}
	imx6q_add_mxc_hdmi_core(&hdmi_core_data);

	imx6q_add_ipuv3(0, &ipu_data[0]);
	if (cpu_is_mx6q()) {
		imx6q_add_ipuv3(1, &ipu_data[1]);
		for (i = 0; i < 4 && i < ARRAY_SIZE(sabresd_fb_data); i++)
			imx6q_add_ipuv3fb(i, &sabresd_fb_data[i]);
	} else
		//for (i = 0; i < 2 && i < ARRAY_SIZE(sabresd_fb_data); i++)
		for (i = 0; i < 3 && i < ARRAY_SIZE(sabresd_fb_data); i++)//peterchiu modify add fb2	
			imx6q_add_ipuv3fb(i, &sabresd_fb_data[i]);

	imx6q_add_vdoa();
	imx6q_add_mipi_dsi(&mipi_dsi_pdata);
	imx6q_add_lcdif(&lcdif_data);
	imx6q_add_ldb(&ldb_data);
	imx6q_add_v4l2_output(0);
	//imx6q_add_v4l2_capture(0, &capture_data[0]);
	//imx6q_add_v4l2_capture(1, &capture_data[1]);

	imx6q_add_imx_snvs_rtc();

		strcpy(mxc_i2c0_board_info[0].type, "wm8962");
		mxc_i2c0_board_info[0].platform_data = &wm8962_config_data;
	


	imx6q_add_imx_i2c(0, &mx6q_sabresd_i2c_data);
#ifdef CONFIG_PN544_NFC
	imx6q_add_imx_i2c(1, &wm_i2c_data);		//kevin modified this for nfc
#else
	imx6q_add_imx_i2c(1, &mx6q_sabresd_i2c_data);	//kevin this orig
#endif
	imx6q_add_imx_i2c(2, &mx6q_sabresd_i2c_data);
	if (cpu_is_mx6dl())
		imx6q_add_imx_i2c(3, &mx6q_sabresd_i2c_data);
	i2c_register_board_info(0, mxc_i2c0_board_info,
			ARRAY_SIZE(mxc_i2c0_board_info));
	i2c_register_board_info(1, mxc_i2c1_board_info,
			ARRAY_SIZE(mxc_i2c1_board_info));
	i2c_register_board_info(2, mxc_i2c2_board_info,
			ARRAY_SIZE(mxc_i2c2_board_info));
	ret = gpio_request(SABRESD_PFUZE_INT, "pFUZE-int");
	if (ret) {
		printk(KERN_ERR"request pFUZE-int error!!\n");
		return;
	} else {
		gpio_direction_input(SABRESD_PFUZE_INT);
		mx6q_sabresd_init_pfuze100(SABRESD_PFUZE_INT);
	}
	/* SPI */
	imx6q_add_ecspi(0, &mx6q_sabresd_spi_data);
	spi_device_init();

	imx6q_add_mxc_hdmi(&hdmi_data);

	imx6q_add_anatop_thermal_imx(1, &mx6q_sabresd_anatop_thermal_data);
	imx6_init_fec(fec_data); //rudolph commamd out
#ifdef CONFIG_MX6_ENET_IRQ_TO_GPIO
	/* Make sure the IOMUX_OBSRV_MUX1 is set to ENET_IRQ. */
	mxc_iomux_set_specialbits_register(IOMUX_OBSRV_MUX1_OFFSET,
		OBSRV_MUX1_ENET_IRQ, OBSRV_MUX1_MASK);
#endif

	imx6q_add_pm_imx(0, &mx6q_sabresd_pm_data);

	/* Move sd4 to first because sd4 connect to emmc.
	   Mfgtools want emmc is mmcblk0 and other sd card is mmcblk1.
	*/
//rudolph modify for the storage device mount order(first is eMMC) 20150302. 	
//	imx6q_add_sdhci_usdhc_imx(2, &mx6q_sabresd_sd3_data);
//	imx6q_add_sdhci_usdhc_imx(3, &mx6q_sabresd_sd4_data);
	imx6q_add_sdhci_usdhc_imx(3, &mx6q_sabresd_sd4_data);
        imx6q_add_sdhci_usdhc_imx(2, &mx6q_sabresd_sd3_data);

	
	imx_add_viv_gpu(&imx6_gpu_data, &imx6q_gpu_pdata);
	imx6q_sabresd_init_usb();
	/* SATA is not supported by MX6DL/Solo */
	if (cpu_is_mx6q()) {
#ifdef CONFIG_SATA_AHCI_PLATFORM
		imx6q_add_ahci(0, &mx6q_sabresd_sata_data);
#else
		mx6q_sabresd_sata_init(NULL,
			(void __iomem *)ioremap(MX6Q_SATA_BASE_ADDR, SZ_4K));		
#endif
	}
	imx6q_add_vpu();
	imx6q_init_audio();//rudolph revert for FA33 audio
	platform_device_register(&sabresd_vmmc_reg_devices);
	imx_asrc_data.asrc_core_clk = clk_get(NULL, "asrc_clk");
	imx_asrc_data.asrc_audio_clk = clk_get(NULL, "asrc_serial_clk");
	imx6q_add_asrc(&imx_asrc_data);

	/*
	 * Disable HannStar touch panel CABC function,
	 * this function turns the panel's backlight automatically
	 * according to the content shown on the panel which
	 * may cause annoying unstable backlight issue.
	 */
	gpio_request(SABRESD_CABC_EN0, "cabc-en0");/*CABC_EN0*/                                 
	gpio_direction_output(SABRESD_CABC_EN0, 1);
	
	gpio_request(SABRESD_BLON_3V_H, "blon-3v");/*BLON_3V_H*/                   
	gpio_direction_output(SABRESD_BLON_3V_H, 1);
	
	gpio_request(SABRESD_AUX_5V_EN, "aux-5v");/*AUX_5V_EN*/
	gpio_direction_output(SABRESD_AUX_5V_EN, 1);


//	gpio_request(SABRESD_FAN_EN, "Fan-control");    /* External Fan */
//	gpio_direction_output(SABRESD_FAN_EN, 1);
//	gpio_export(SABRESD_FAN_EN,0);


//coby test uart to qpio mode 26 27
//*#define SABRESD_EPDC_SDOED	IMX_GPIO_NR(3, 26)
//*#define SABRESD_EPDC_SDOE	IMX_GPIO_NR(3, 27)
//        gpio_request(SABRESD_EPDC_SDOED, "qpio3_26");    /* gpio3_26 */
//        gpio_direction_output(SABRESD_EPDC_SDOED, 1);
 //       gpio_export(SABRESD_EPDC_SDOED,0);
  //      gpio_request(SABRESD_EPDC_SDOE, "qpio3_27");    /* gpio3_27 */
   //     gpio_direction_output(SABRESD_EPDC_SDOE, 1);
    //    gpio_export(SABRESD_EPDC_SDOE,0);


	gpio_request(SABRESD_BUZZER_INTERNAL, "Internal-Buzzer");    /* Internal Buzzer */
	gpio_direction_output(SABRESD_BUZZER_INTERNAL, 1);
	gpio_export(SABRESD_BUZZER_INTERNAL,0);

	gpio_request(SABRESD_BUZZER_EXTERNAL, "External-Buzzer");    /* External Buzzer */
	gpio_direction_output(SABRESD_BUZZER_EXTERNAL, 1);
	gpio_export(SABRESD_BUZZER_EXTERNAL,0);

	gpio_request(SABRESD_EEPROM_RW, "EEPROM_RW");    /* Rudolph add EEPROM can write */
	gpio_direction_output(SABRESD_EEPROM_RW, 0);
        gpio_export(SABRESD_EEPROM_RW,0);

	gpio_request(SABRESD_ECSPI3_MISO, "ECSPI3_MISO");	/* wich add for sip test */
	gpio_direction_input(SABRESD_ECSPI3_MISO);
	gpio_export(SABRESD_ECSPI3_MISO,0);

	gpio_request(SABRESD_ECSPI3_MOSI, "ECSPI3_MOSI");
	gpio_direction_output(SABRESD_ECSPI3_MOSI, 1);
	gpio_export(SABRESD_ECSPI3_MOSI,0);
	
	gpio_request(NFC_ENABLE, "NFC_ENABLE"); 	//kevin add nfc_enable
        gpio_direction_output(NFC_ENABLE, 1);		//kevin this part is reverse
        gpio_export(NFC_ENABLE,0);

	gpio_request(NFC_IRQ, "NFC_IRQ");		//kevin add nfc_irq
	gpio_direction_input(NFC_IRQ);

	gpio_request(SABRESD_ECSPI3_CS1, "ECSPI3_CS1");    
	gpio_direction_input(SABRESD_ECSPI3_CS1);
	gpio_export(SABRESD_ECSPI3_CS1,0);
	
	gpio_request(SABRESD_ECSPI3_SCLK, "ECSPI3_SCLK");    
	gpio_direction_output(SABRESD_ECSPI3_SCLK, 1);
	gpio_export(SABRESD_ECSPI3_SCLK,0);

	
#ifdef CONFIG_WM_HW_KEY_K0
	gpio_request(DIN0, "DIN0");    /* ruei add DIN0 */
	gpio_direction_input(DIN0);
	gpio_export(DIN0,0);

	gpio_request(DIN1, "DIN1");    /* ruei add DIN1 */
	gpio_direction_input(DIN1);
	gpio_export(DIN1,0);
	
	gpio_request(DIN2, "DIN2");    /* ruei add DIN2 */
	gpio_direction_input(DIN2);
	gpio_export(DIN2,0);
	
	gpio_request(DIN3, "DIN3");    /* ruei add DIN3 */
	gpio_direction_input(DIN3);
	gpio_export(DIN3,0);
	

	gpio_request(DIN4, "DIN4");    /* ruei add DIN4 */
        gpio_direction_input(DIN4);
        gpio_export(DIN4,0);

	gpio_request(DIN5, "DIN5");    /* ruei add DIN5 */
        gpio_direction_input(DIN5);
        gpio_export(DIN5,0);
	
	
#endif


	gpio_request(DOUT0, "DOUT0");    /* Rudolph add DOUT0 */
        gpio_direction_output(DOUT0, 0);
        gpio_export(DOUT0,0);

        gpio_request(DOUT1, "DOUT1");    /* Rudolph add DOUT1 */
        gpio_direction_output(DOUT1, 0);
        gpio_export(DOUT1,0);

	gpio_request(DOUT2, "DOUT2");    /* Rudolph add DOUT2 */
        gpio_direction_output(DOUT2, 0);
        gpio_export(DOUT2,0);

	gpio_request(DOUT3, "DOUT3");    /* Rudolph add DOUT3 */
        gpio_direction_output(DOUT3, 0);
        gpio_export(DOUT3,0);
	
	gpio_request(DOUT4, "DOUT4");    /* Rudolph add DOUT4 */
        gpio_direction_output(DOUT4, 0);
        gpio_export(DOUT4,0);

	gpio_request(DOUT5, "DOUT5");    /* Rudolph add DOUT5 */
        gpio_direction_output(DOUT5, 0);
        gpio_export(DOUT5,0);

	gpio_request(LINE_SW, "LINE_SW");    /* ruei add OMFA line out switch */
        gpio_direction_output(LINE_SW, 0);
        gpio_export(LINE_SW,0);

#ifdef CONFIG_PANEL_LITEMAX_SSF_SSH3825
	gpio_request(SABRESD_BL_PWM, "BL_PWM");         /*BL_PWM*/      //kevin test for 38 inch panel
        gpio_direction_output(SABRESD_BL_PWM, 1);
#endif
//	led_init();

	/*
	gpio_request(rudolph_SD_WP, "SD3_WP");   // Rudolph add SD3_WP
        gpio_direction_input(rudolph_SD_WP);
        gpio_export(rudolph_SD_WP,0);
	*/
	/*
	gpio_request(WM_SET_AS_HOST, "SET_HOST");     //rudolph add for set as host 
        gpio_direction_output(WM_SET_AS_HOST, 0);
        gpio_export(WM_SET_AS_HOST,0);
	*/

	imx6q_add_mxc_pwm(0);
	imx6q_add_mxc_pwm(1);
	imx6q_add_mxc_pwm(2);
	imx6q_add_mxc_pwm(3);
	imx6q_add_mxc_pwm_backlight(0, &mx6_sabresd_pwm_backlight_data);

	imx6q_add_otp();
	imx6q_add_viim();
	imx6q_add_imx2_wdt(0, NULL);                                   /* watch dog   */
	imx6q_add_dma();

	imx6q_add_dvfs_core(&sabresd_dvfscore_data);

	if (imx_ion_data.heaps[0].size)
		imx6q_add_ion(0, &imx_ion_data,
			sizeof(imx_ion_data) + sizeof(struct ion_platform_heap));

	imx6q_add_device_buttons();

	/* enable sensor 3v3 and 1v8 */
	gpio_request(SABRESD_SENSOR_EN, "sensor-en");
	gpio_direction_output(SABRESD_SENSOR_EN, 1);

#ifdef CONFIG_PN544_NFC
	wm_nfc_pn544_init();	//kevin add
#endif
	MotionSensorInit();	//willy add

	imx6q_add_hdmi_soc();
	imx6q_add_hdmi_soc_dai();
	//rudolph add for WinmateKeyGen
	WinmateKeyGen_Init();
	/*ret = gpio_request_array(mx6q_sabresd_flexcan_gpios,
			ARRAY_SIZE(mx6q_sabresd_flexcan_gpios));*/
	if (ret)
		pr_err("failed to request flexcan1-gpios: %d\n", ret);
	else
	{
		imx6q_add_flexcan0(&mx6q_sabresd_flexcan0_pdata);
		imx6q_add_flexcan1(&mx6q_sabresd_flexcan1_pdata);
		}

	clko2 = clk_get(NULL, "clko2_clk");
	if (IS_ERR(clko2))
		pr_err("can't get CLKO2 clock.\n");

	new_parent = clk_get(NULL, "osc_clk");
	if (!IS_ERR(new_parent)) {
		clk_set_parent(clko2, new_parent);
		clk_put(new_parent);
	}
	rate = clk_round_rate(clko2, 24000000);		
	clk_set_rate(clko2, rate);
	clk_enable(clko2);

	/* Camera and audio use osc clock */
	clko = clk_get(NULL, "clko_clk");
	if (!IS_ERR(clko))
		clk_set_parent(clko, clko2);

	/* Enable Aux_5V */
	gpio_request(SABRESD_AUX_5V_EN, "aux_5v_en");
	gpio_direction_output(SABRESD_AUX_5V_EN, 1);
	gpio_set_value(SABRESD_AUX_5V_EN, 1);

	pm_power_off = mx6_snvs_poweroff;
	imx6q_add_busfreq();

	/* Add PCIe RC interface support
	 * uart5 has pin mux with pcie. or you will use uart5 or use pcie
	 */
	if (!uart5_enabled)
		imx6q_add_pcie(&mx6_sabresd_pcie_data);

	imx6_add_armpmu();
	imx6q_add_perfmon(0);
	imx6q_add_perfmon(1);
	imx6q_add_perfmon(2);
}

extern void __iomem *twd_base;
static void __init mx6_sabresd_timer_init(void)
{
	struct clk *uart_clk;
#ifdef CONFIG_LOCAL_TIMERS
	twd_base = ioremap(LOCAL_TWD_ADDR, SZ_256);
	BUG_ON(!twd_base);
#endif
	mx6_clocks_init(32768, 24000000, 0, 0);

	uart_clk = clk_get_sys("imx-uart.0", NULL);
	early_console_setup(UART1_BASE_ADDR, uart_clk);
}

static struct sys_timer mx6_sabresd_timer = {
	.init   = mx6_sabresd_timer_init,
};

static void __init mx6q_sabresd_reserve(void)
{
	phys_addr_t phys;
	int i, fb0_reserved = 0, fb_array_size;

	/*
	 * Reserve primary framebuffer memory if its base address
	 * is set by kernel command line.
	 */
	fb_array_size = ARRAY_SIZE(sabresd_fb_data);
	if (fb_array_size > 0 && sabresd_fb_data[0].res_base[0] &&
	    sabresd_fb_data[0].res_size[0]) {
		memblock_reserve(sabresd_fb_data[0].res_base[0],
				 sabresd_fb_data[0].res_size[0]);
		memblock_remove(sabresd_fb_data[0].res_base[0],
				sabresd_fb_data[0].res_size[0]);
		sabresd_fb_data[0].late_init = true;
		ipu_data[ldb_data.ipu_id].bypass_reset = true;
		fb0_reserved = 1;
	}
	for (i = fb0_reserved; i < fb_array_size; i++)
		if (sabresd_fb_data[i].res_size[0]) {
			/* Reserve for other background buffer. */
			phys = memblock_alloc(sabresd_fb_data[i].res_size[0],
						SZ_4K);
			memblock_remove(phys, sabresd_fb_data[i].res_size[0]);
			sabresd_fb_data[i].res_base[0] = phys;
		}

#ifdef CONFIG_ANDROID_RAM_CONSOLE
	phys = memblock_alloc_base(SZ_128K, SZ_4K, SZ_1G);
	memblock_remove(phys, SZ_128K);
	memblock_free(phys, SZ_128K);
	ram_console_resource.start = phys;
	ram_console_resource.end   = phys + SZ_128K - 1;
#endif

#if defined(CONFIG_MXC_GPU_VIV) || defined(CONFIG_MXC_GPU_VIV_MODULE)
	if (imx6q_gpu_pdata.reserved_mem_size) {
		phys = memblock_alloc_base(imx6q_gpu_pdata.reserved_mem_size,
					   SZ_4K, SZ_1G);
		memblock_remove(phys, imx6q_gpu_pdata.reserved_mem_size);
		imx6q_gpu_pdata.reserved_mem_base = phys;
	}
#endif

#if defined(CONFIG_ION)
	if (imx_ion_data.heaps[0].size) {
		phys = memblock_alloc(imx_ion_data.heaps[0].size, SZ_4K);
		memblock_remove(phys, imx_ion_data.heaps[0].size);
		imx_ion_data.heaps[0].base = phys;
	}
#endif
}

/*
 * initialize __mach_desc_MX6Q_SABRESD data structure.
 */
MACHINE_START(FA33, "FA33")
	/* Maintainer: Freescale Semiconductor, Inc. */
	.boot_params = MX6_PHYS_OFFSET + 0x100,
	.fixup = fixup_mxc_board,
	.map_io = mx6_map_io,
	.init_irq = mx6_init_irq,
	.init_machine = mx6_sabresd_board_init,
	.timer = &mx6_sabresd_timer,
	.reserve = mx6q_sabresd_reserve,
MACHINE_END
