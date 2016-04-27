#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/dma-mapping.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/ipu.h>
#include <linux/fb.h>
#include <linux/delay.h>
#include <linux/uio_driver.h>
#include <linux/iram_alloc.h>
#include <linux/fsl_devices.h>
#include <linux/uaccess.h>
#include <mach/common.h>
#include <mach/hardware.h>
#include <mach/gpio.h>

MODULE_LICENSE("GPL");

#define DEV_BUFSIZE 1024

static int dev_open(struct inode*, struct file*);
static int dev_release(struct inode*, struct file*);
static ssize_t dev_read(struct file*, char __user*, size_t, loff_t*);
static ssize_t dev_write(struct file*, const char __user *, size_t, loff_t*);
static void __exit exit_modules(void);

static struct file_operations dev_fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
};

#define AMODE0		IMX_GPIO_NR(5, 5)	/*128+5 = 133 */
#define AMODE1		IMX_GPIO_NR(5, 6)	/* 134 */
#define BMODE0		IMX_GPIO_NR(5, 8)	/* 136 */	
#define BMODE1		IMX_GPIO_NR(5, 9)	/* 137 */
#define CMODE0		IMX_GPIO_NR(5, 15)	/* 143 */
#define CMODE1		IMX_GPIO_NR(5, 16)	/* 144 */
#define COM_PORT_AMOUNT	3
#define FIRST_MINOR		11
static int iModeSwitchGpio[3][2] = {{AMODE0, AMODE1},{BMODE0, BMODE1},{CMODE0, CMODE1}};

static struct miscdevice com_switch_miscdev[] = {
	{
		.minor      = FIRST_MINOR,
		.name       = "com0_switch_dev",
		.fops       = &dev_fops,
	},
	{
		.minor      = FIRST_MINOR + 1,
		.name       = "com1_switch_dev",
		.fops       = &dev_fops,
	},
	{
		.minor      = FIRST_MINOR + 2,
		.name       = "com2_switch_dev",
		.fops       = &dev_fops,
	}
};

static int
dev_open(struct inode *inode, struct file *filp)
{
	
	/*printk(KERN_DEBUG "%s():\n", __FUNCTION__);*/

    return 0;
}

static int
dev_release(struct inode *inode, struct file *filp)
{
	/*int i;
	
	for(i=0;i<COM_PORT_AMOUNT;i++){
		gpio_free(iModeSwitchGpio[i][0]);
		gpio_free(iModeSwitchGpio[i][1]);
	}*/
	/*printk(KERN_DEBUG "%s():\n", __FUNCTION__);*/
	
    return 0;
}

#define MODE_LENGTH sizeof("RS-232")
static ssize_t
dev_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
	int mode=0, name=0, iRetVal;
	struct inode* inode = filp->f_dentry->d_inode;
	int minor = iminor(inode);
	int iComPort = minor - FIRST_MINOR;
	unsigned char cpPortName[MODE_LENGTH];

	if(*pos > 0)
		return 0;
	
	mode = (gpio_get_value(iModeSwitchGpio[iComPort][0])<<1) + gpio_get_value(iModeSwitchGpio[iComPort][1]);
	switch(mode){
		case 2://485
			name = 485;
			break;
		case 1://232
			name = 232;
			break;
		case 0://422
			name = 422;
			break;
		default:
			name = 0;
			break;
	}
	memset(cpPortName, 0, MODE_LENGTH);
	sprintf(cpPortName, "RS-%d", name);
	iRetVal = copy_to_user(buf, cpPortName, strlen(cpPortName)+1);
	*pos = strlen(cpPortName)+1;
	if(iRetVal)
		printk(KERN_DEBUG "%s():COM port %d mode is RS-%d, copy_to_user return %d, send %s, offset is %d\n", __FUNCTION__, iComPort, name, iRetVal, cpPortName);
    
    return strlen(cpPortName)+1;
}

static ssize_t
dev_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{
	int mode = 0;
	struct inode* inode = filp->f_dentry->d_inode;
	int minor = iminor(inode);
	int iComPort = minor - FIRST_MINOR;
	
	if(strstr(buf, "232")){
		mode=232;
		gpio_set_value(iModeSwitchGpio[iComPort][0], 0);
		gpio_set_value(iModeSwitchGpio[iComPort][1], 1);
	}
	else if(strstr(buf, "422")){
		mode=422;
		gpio_set_value(iModeSwitchGpio[iComPort][0], 0);
		gpio_set_value(iModeSwitchGpio[iComPort][1], 0);
	}
	else if(strstr(buf, "485")){
		mode=485;
		gpio_set_value(iModeSwitchGpio[iComPort][0], 1);
		gpio_set_value(iModeSwitchGpio[iComPort][1], 0);
	}
    printk(KERN_DEBUG "%s():switch COM port to RS-%d\n", __FUNCTION__, mode);
	
    return count;
}

static int __init init_modules(void)
{
	int ret = 0, i, j;
	char cpComPort[16];

	for(i=0;i<COM_PORT_AMOUNT;i++){
		sprintf(cpComPort, "COM-%d-SW-0", i);
		ret = gpio_request(iModeSwitchGpio[i][0], cpComPort);
		gpio_direction_output(iModeSwitchGpio[i][0], 1);
		gpio_free(iModeSwitchGpio[i][0]);
		sprintf(cpComPort, "COM-%d-SW-1", i);
		ret = gpio_request(iModeSwitchGpio[i][1], cpComPort);
		gpio_direction_output(iModeSwitchGpio[i][1], 1);
		gpio_free(iModeSwitchGpio[i][1]);
		gpio_set_value(iModeSwitchGpio[i][0], 0);/*set to RS232*/
		gpio_set_value(iModeSwitchGpio[i][1], 1);/*set to RS232*/
	}
	
	for(i=0;i<COM_PORT_AMOUNT;i++){
		ret = misc_register(&(com_switch_miscdev[i]));
		if (ret != 0) {
			printk(KERN_DEBUG "cannot register miscdev on minor=%d (err=%d)\n",com_switch_miscdev[i].minor , ret);
			for(j=i-1;j>-1;j--){
				misc_deregister(&(com_switch_miscdev[j]));
			}
			break;
		}
	}

	return 0;
}

static void __exit exit_modules(void)
{
	int i;
	for(i=0;i<COM_PORT_AMOUNT;i++){
		misc_deregister(&(com_switch_miscdev[i]));
	}
}

module_init(init_modules);
module_exit(exit_modules);
