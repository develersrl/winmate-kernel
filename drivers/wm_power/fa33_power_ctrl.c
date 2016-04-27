#include <linux/init.h>        /* For init/exit macros */
#include <linux/module.h>      /* For MODULE_ marcros  */
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>

#include <asm/uaccess.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <mach/system.h>

#include <linux/delay.h>//kevin
#include <linux/gpio.h>//kevin

#include <mach/iomux-mx6q.h>
#include <mach/iomux-mx6dl.h>
#include <mach/ipu-v3.h>
//#include <sys/stat.h>
//#include <sys/cdefs.h>
//#include <linux/sys.h>
#include <linux/syscalls.h>



// device major number.
	#define NFC_ENABLE_MAJ 		312
	#define I2C_ENABLE_MAJ 		313
	#define BARCODE_ENABLE_MAJ 	314
	#define RFID_ENABLE_MAJ 	315

// device major number end

	#define NFC_ENABLE        IMX_GPIO_NR(1, 20)
	#define I2C_SCL_ENABLE	  IMX_GPIO_NR(1, 3)	/* 3 */
	#define I2C_SDA_ENABLE	  IMX_GPIO_NR(1, 6)	/* 6 */
	#define I2C_IRQ_ENABLE	  IMX_GPIO_NR(6, 8)	/* 168 */
	#define I2C_RST_ENABLE	  IMX_GPIO_NR(6, 16)	/* 176 */

static int strtoint(char *inputstr)
{
	int a=0;
	while(*inputstr)
	{
		if( (*inputstr<'0') || (*inputstr>'9'))
			break;
		
		a*=10;
		a+=(*inputstr)-'0';
		inputstr++;
	}
	
	return a;
}

int barcode_enable_DeviceEnum = 0;
static int barcode_enable_open(struct inode *inode, struct file *file) {
    if (barcode_enable_DeviceEnum) return -EBUSY;
    barcode_enable_DeviceEnum++;
    return 0;
}
static int barcode_enable_release(struct inode *inode, struct file *file) {
    barcode_enable_DeviceEnum--;
    return 0;
}
static ssize_t barcode_enable_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
    char bbuf[128];
    int retlen = copy_from_user(bbuf,buff,len);
    bbuf[len] = 0;
    if (strtoint(bbuf)) {
        printk("set Barcode Power Enable.\n");
    } else {
        printk("set Barcode Power Disable.\n");
    }
    return len;
}
struct file_operations barcode_enable_fops = {
    write: barcode_enable_write,
    open: barcode_enable_open,
    release: barcode_enable_release
};

int RFID_Enable_DeviceEnum = 0;
static int RFID_enable_open(struct inode *inode, struct file *file) {
    if (RFID_Enable_DeviceEnum) return -EBUSY;
    RFID_Enable_DeviceEnum++;
    return 0;
}
static int RFID_enable_release(struct inode *inode, struct file *file) {
    RFID_Enable_DeviceEnum--;
    return 0;
}
static ssize_t RFID_enable_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
    char bbuf[128];
    int retlen = copy_from_user(bbuf,buff,len);
    bbuf[len] = 0;
    if (strtoint(bbuf)) {
        printk("set RFID Power Enable.\n");
    } else {
        printk("set RFID Power Disable.\n");
    }
    return len;
}
struct file_operations RFID_enable_fops = {
    write: RFID_enable_write,
    open: RFID_enable_open,
    release: RFID_enable_release
};



int NFC_Enable_DeviceEnum=0;
static int NFC_enable_open(struct inode *inode, struct file *file)
{
    
    if (NFC_Enable_DeviceEnum) return -EBUSY;
    NFC_Enable_DeviceEnum++;
    return 0;
}

static int NFC_enable_release(struct inode *inode, struct file *file)
{
    NFC_Enable_DeviceEnum--;
    return 0;
}


static ssize_t NFC_enable_write(struct file *filp,
                                       const char *buff,
                                       size_t len,
                                       loff_t *off)
{
    char bbuf[128];
    int retlen;
    retlen=copy_from_user(bbuf,buff,len);
    bbuf[len]=0;
//    printk("\n NFC_enable write -- Str=%s\n",bbuf);
    
    if(strtoint(bbuf))
    {
        printk("set NFC Enable.\n");
	gpio_set_value(NFC_ENABLE, 1);			//fa33 reverse this part

	msleep(50);

	gpio_set_value(NFC_ENABLE, 0);

	msleep(50);

	gpio_set_value(NFC_ENABLE, 1);
    }
    else
    {
        printk("set NFC Disable.\n");
	gpio_set_value(NFC_ENABLE, 1);	
    }
	
    return len;
}

struct file_operations NFC_enable_fops = {
write: NFC_enable_write,
open: NFC_enable_open,
release: NFC_enable_release
};



int I2C_Enable_DeviceEnum=0;
static int I2C_enable_open(struct inode *inode, struct file *file)
{
    
    if (I2C_Enable_DeviceEnum) return -EBUSY;
    I2C_Enable_DeviceEnum++;
    return 0;
}

static int I2C_enable_release(struct inode *inode, struct file *file)
{
    I2C_Enable_DeviceEnum--;
    return 0;
}


static ssize_t I2C_enable_write(struct file *filp,
                                       const char *buff,
                                       size_t len,
                                       loff_t *off)
{
    char bbuf[128];
    int retlen;
    retlen=copy_from_user(bbuf,buff,len);
    bbuf[len]=0;
    printk("\n I2C_enable write -- Str=%s\n",bbuf);
    
    static iomux_v3_cfg_t mx6q_I2C_pads[] = {MX6Q_PAD_GPIO_3__GPIO_1_3, MX6Q_PAD_GPIO_6__GPIO_1_6, MX6Q_PAD_NANDF_ALE__GPIO_6_8, MX6Q_PAD_NANDF_CS3__GPIO_6_16};
    static iomux_v3_cfg_t mx6dl_I2C_pads[] = {MX6DL_PAD_GPIO_3__GPIO_1_3, MX6DL_PAD_GPIO_6__GPIO_1_6, MX6DL_PAD_NANDF_ALE__GPIO_6_8, MX6DL_PAD_NANDF_CS3__GPIO_6_16};
    if(strtoint(bbuf))
    {
        printk("set I2C Enable.\n");
	if (cpu_is_mx6q())
		mxc_iomux_v3_setup_multiple_pads(mx6q_I2C_pads, ARRAY_SIZE(mx6q_I2C_pads));
	else if (cpu_is_mx6dl())
		mxc_iomux_v3_setup_multiple_pads(mx6dl_I2C_pads, ARRAY_SIZE(mx6dl_I2C_pads));

	gpio_request(I2C_SCL_ENABLE, "I2CSCL");    	//willy add
        gpio_direction_input(I2C_SCL_ENABLE);
        gpio_export(I2C_SCL_ENABLE,0);
        gpio_request(I2C_SDA_ENABLE, "I2CSDA");		//willy add    
        gpio_direction_output(I2C_SDA_ENABLE, 0);
        gpio_export(I2C_SDA_ENABLE,0);
        gpio_request(I2C_IRQ_ENABLE, "I2CIRQ");    	//willy add
        gpio_direction_input(I2C_IRQ_ENABLE);
        gpio_export(I2C_IRQ_ENABLE,0);
        gpio_request(I2C_RST_ENABLE, "I2CRST");    	//willy add
        gpio_direction_output(I2C_RST_ENABLE, 0);
        gpio_export(I2C_RST_ENABLE,0);

//	sys_chmod("/sys/class/gpio/gpio3/value", 420);
//        sys_chmod("/sys/class/gpio/gpio6/value", 420);
//        sys_chmod("/sys/class/gpio/gpio168/value", 420);
//        sys_chmod("/sys/class/gpio/gpio176/value", 420);

    }
    else
    {
    }
	
    return len;
}


struct file_operations I2C_enable_fops = {
write: I2C_enable_write,
open: I2C_enable_open,
release: I2C_enable_release
};





static int __init fa33_power_ctrl_init(void)
{
    int result;

        result=register_chrdev(NFC_ENABLE_MAJ, "nfc_enable",&NFC_enable_fops);
	printk("NFC_enable char register result = %d. \r\n", result);
	result=register_chrdev(I2C_ENABLE_MAJ, "i2c_enable",&I2C_enable_fops);
	printk("I2C_enable char register result = %d. \r\n", result);
        result=register_chrdev(RFID_ENABLE_MAJ, "RFID_enable",&RFID_enable_fops);
        printk("RFID_enable char register result = %d. \r\n", result);
        result=register_chrdev(BARCODE_ENABLE_MAJ, "barcode_enable",&barcode_enable_fops);
        printk("barcode_enable char register result = %d. \r\n", result);
	
    return 0;
}

static void __exit fa33_power_ctrl_exit (void)
{
    	unregister_chrdev(NFC_ENABLE_MAJ, "nfc_enable");			//kevin add
	unregister_chrdev(I2C_ENABLE_MAJ, "i2c_enable");                        //willy add
	unregister_chrdev(RFID_ENABLE_MAJ, "RFID_enable");                      //perry add
	unregister_chrdev(BARCODE_ENABLE_MAJ, "barcode_enable");                //perry add
}

module_init(fa33_power_ctrl_init);
module_exit(fa33_power_ctrl_exit);

MODULE_AUTHOR("WM_Kevin");
MODULE_DESCRIPTION("Winmate I/O Char Driver");
MODULE_LICENSE("GPL");



		

