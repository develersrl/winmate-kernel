//##########################################################################
//#                                                                                                #
//#        Copyright (C) 2014 by GeneralTouch Co., Ltd                                             #
//#                                                                                                #
//#        GeneralTouch Touchscreen kernel driver                                                  #    
//#                                                                                                #
//#        Created by lamson                                                                       #
//#                                                                                                #
//#        Email: android@generaltouch.com                                                         #
//#                                                                                                #
//##########################################################################
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>
#include <linux/input.h>
#include <linux/cdev.h>
#include <asm/uaccess.h> 
#include <linux/fs.h>

#define DRIVER_VERSION "Android single touchscreen v2.1"
#define DRIVER_AUTHOR "Lamson"
#define DRIVER_DESC "GeneralTouch USB touchscreen driver for single point"
#define DRIVER_LICENSE "GPL"
#define TOUCH_DRIVER_NAME "general_touch_single"

#define GENERALTOUCH_DEVICE_MAJOR 47
#define GENERALTOUCH_DEVICE_MINOR 121
#define USB_TRANSFER_LENGTH 7
#define GENERALTOUCH_DEVICE_NAME "general_touch_single"

#define GT_CTLCODE_RECOV_CALIB 	    0xa0
#define GT_CTLCODE_RESET_CALIBX 	0xa1
#define GT_CTLCODE_RESET_CALIBY 	0xa2
#define GT_CTLCODE_SAVE_CALIB_PARA 	0xa3
#define GT_CTLCODE_SAVE_CALIB_PARA_X 	0xa4
#define GT_CTLCODE_SAVE_CALIB_PARA_Y 	0xa5
#define GT_CTLCODE_SET_SAVE_PRE 	0xa6
#define GT_CTLCODE_SET_SAVE_OK 		0xa7
#define GT_CTLCODE_GET_REALPARA         0xa8


#pragma pack(1)
struct calib_param{
	long	A;
	long	B;
	long	C;
	unsigned char reserve[46];
};

#pragma pack()

struct generaltouch_device {
	char name[128];
	char phys[64];
	struct usb_device *usbdev;
	struct input_dev *dev;
	struct urb *irq;
	unsigned char *data;
	dma_addr_t data_dma;
}; 

struct cdev general_touch_single_cdev;
static struct class *generaltouchsingle_class;
static struct usb_driver generaltouchsingle_usb_driver;
static struct file_operations generaltouchsingle_fops;

static struct usb_class_driver general_touch_single_class_driver = {
	.name = "general_touch_single",
	.fops = &generaltouchsingle_fops,
	.minor_base = GENERALTOUCH_DEVICE_MINOR,
};


static int general_touch_single_open(struct inode * inode, struct file * filp)
{
	struct generaltouch_device * general_touch_single;
	struct usb_interface * intf;
	
	printk("lamson: general_touch_single_open!!!");

	intf = usb_find_interface(&generaltouchsingle_usb_driver, GENERALTOUCH_DEVICE_MINOR);
	if(!intf)
	{
		printk("lamson dbg general_touch_single:    %s usb_find_interface ERROR!",__func__);
		return -1;
	}

	general_touch_single = usb_get_intfdata(intf);
	if(!general_touch_single)
	{
		printk("lamson dbg general_touch_single:    %s touch is NULL!",__func__);
		return -1;
	}

	filp->private_data = general_touch_single;

	return 0;
}

static int general_touch_single_release(struct inode * inode, struct file * filp)
{
	printk("lamson:  general_touch_single_release");
	return 0;
}

unsigned char general_touch_send_command(struct usb_device * udev,
			unsigned char * in_data,
			unsigned char * out_data,
			int 		length
		)
{
	unsigned char	buf[64];
	int		cnt = 0;
	int		ret = 0;

	memset(buf,0,sizeof(buf));

	do{
		ret = usb_control_msg(udev, 
					usb_sndctrlpipe(udev, 0), 
					0x01, 
					0x40,
					0, 
					0, 
					(char *)in_data,
					length, 
					1000);
		msleep(10);

		ret = usb_control_msg(udev, 
					usb_rcvctrlpipe(udev, 0), 
					0x02,
					0xc0, 
					0, 
					0, 
					(char *)buf, 
					64, 
					1000);
		msleep(10);

		cnt++;

	}while((buf[1] != 0x01) && (cnt < 5));

	if(cnt >= 5)
	{
		return false;
	}

	memcpy(out_data,buf,sizeof(buf));

	return true;
	
}

static void general_touch_send_params(struct usb_device * udev,	unsigned char	*sedata)
{
	unsigned char	recdata[64];
	int status = -1;
	int length = 20;
	memset(recdata,0,sizeof(recdata));

	status = general_touch_send_command(udev,sedata,recdata,length);
	if(!status)
	{
		printk("lamson dbg general_touch_single:%s  send command mode failed.",__func__);
	}

}



static long general_touch_single_ioctl(struct file * filp, unsigned int ctl_code, unsigned long ctl_param)
{
	unsigned char bufstx[64],bufsty[64],bufx[64],bufy[64],bufsv[64],bufok[64],bufrcv[64];
	int ret = -1;
	struct generaltouch_device *general_touch_single;
	
	memset(bufstx,0,sizeof(bufstx));
	memset(bufsty,0,sizeof(bufsty));
	memset(bufx,0,sizeof(bufx));
	memset(bufy,0,sizeof(bufy));
	memset(bufsv,0,sizeof(bufsv));
	memset(bufok,0,sizeof(bufok));
	memset(bufok,0,sizeof(bufrcv));


	printk("lamson :   general_touch_single_ioctl!!!");

	if(NULL == filp->private_data)
	{
		printk("GENERALTOUCH:    %s lamosn's private_data is NULL!",__func__);
		return ret;
	}

	general_touch_single = filp->private_data;
	//lamson 
	switch(ctl_code)
	{
	
		case GT_CTLCODE_RESET_CALIBX:			
			ret = copy_from_user(&bufstx, (struct calib_param *)ctl_param, sizeof(struct calib_param));
			if(ret != 0)
			{
				printk("lamson dbg general_touch_single: %s <GT_CTLCODE_SET_CALIB_PARA_X>copy_from_user failed!",__func__);
			}
			else
			{
				general_touch_send_params(general_touch_single->usbdev,bufstx);	
			}
			break;
		case GT_CTLCODE_RESET_CALIBY:			
			ret = copy_from_user(&bufsty, (struct calib_param *)ctl_param, sizeof(struct calib_param));
			if(ret != 0)
			{
				printk("lamson dbg general_touch_single: %s <GT_CTLCODE_SET_CALIB_PARA_Y>copy_from_user failed!",__func__);
			}
			else
			{
				general_touch_send_params(general_touch_single->usbdev,bufsty);	
			}
			break;
		case GT_CTLCODE_SAVE_CALIB_PARA_X:			
			ret = copy_from_user(&bufx, (struct calib_param *)ctl_param, sizeof(struct calib_param));
			if(ret != 0)
			{
				printk("lamson dbg general_touch_single:  %s <GT_CTLCODE_SAVE_CALIB_PARA_X>copy_from_user failed!",__func__);
			}
			else
			{
				general_touch_send_params(general_touch_single->usbdev,bufx);	
			}
			break;
		case GT_CTLCODE_SAVE_CALIB_PARA_Y:			
			ret = copy_from_user(&bufy, (struct calib_param *)ctl_param, sizeof(struct calib_param));
			if(ret != 0)
			{
				printk("lamson dbg general_touch_single: %s <GT_CTLCODE_SAVE_CALIB_PARA_Y>copy_from_user failed!",__func__);
			}
			else
			{
				general_touch_send_params(general_touch_single->usbdev,bufy);	
			}
			break;
		case GT_CTLCODE_SET_SAVE_PRE:			
			ret = copy_from_user(&bufsv, (struct calib_param *)ctl_param, sizeof(struct calib_param));
			if(ret != 0)
			{
				printk("lamson dbg general_touch_single: %s <GT_CTLCODE_SET_PRE>copy_from_user failed!",__func__);
			}
			else
			{
				general_touch_send_params(general_touch_single->usbdev,bufsv);	
			}
			break;
		case GT_CTLCODE_SET_SAVE_OK:			
			ret = copy_from_user(&bufok, (struct calib_param *)ctl_param, sizeof(struct calib_param));
			if(ret != 0)
			{
				printk("lamson dbg general_touch_single: %s <GT_CTLCODE_SET_SAVE_OK>copy_from_user failed!",__func__);
			}
			else
			{
				general_touch_send_params(general_touch_single->usbdev,bufok);	
			}
			break;
		case GT_CTLCODE_RECOV_CALIB:			
			ret = copy_from_user(&bufrcv, (struct calib_param *)ctl_param, sizeof(struct calib_param));
			if(ret != 0)
			{
				printk("lamson dbg general_touch_single: %s <GT_CTLCODE_SET_CALIB_PARA_X>copy_from_user failed!",__func__);
			}
			else
			{
				general_touch_send_params(general_touch_single->usbdev,bufrcv);	
			}
			break;
		default:
			break;
	}
	return 0;
}

static void general_touch_processdata(struct generaltouch_device *general_touch_single,
                                 unsigned char *pkt, int len)
{
	int x = 0, y = 0, touch = 0;

	x = (pkt[2] << 8) | pkt[1];
	y = (pkt[4] << 8) | pkt[3];
        touch = pkt[0] & 0x01;

	input_report_key(general_touch_single->dev, BTN_TOUCH, touch);
	input_report_abs(general_touch_single->dev, ABS_X, x);
	input_report_abs(general_touch_single->dev, ABS_Y, y);
	input_sync(general_touch_single->dev);
}

static void general_touch_single_irq(struct urb *urb)
{
	struct generaltouch_device *general_touch_single = urb->context;
	int status;

	switch (urb->status) {
		case 0:            /* success */
			break;
		case -ECONNRESET:  /* unlink */
		case -ENOENT:
		case -ESHUTDOWN:
			return;    /* -EPIPE:  should clear the halt */
		default:	   /* error */
			printk("lamson dbg general_touch_single: %s - nonzero urb status received: %d\n",
		    __func__, urb->status);
			goto resubmit;
	}
	
	general_touch_processdata(general_touch_single, general_touch_single->data, USB_TRANSFER_LENGTH);

	resubmit:
	status = usb_submit_urb (urb, GFP_ATOMIC);
	if (status)
		printk ("lamson dbg general_touch_single: can't resubmit intr, %s-%s/input0, status %d\n",
				general_touch_single->usbdev->bus->bus_name,
				general_touch_single->usbdev->devpath, status);
}

static struct file_operations generaltouchsingle_fops = {
	.owner = THIS_MODULE,
	.open = general_touch_single_open,
	.unlocked_ioctl = general_touch_single_ioctl,
	.release = general_touch_single_release,
};

static int general_touch_single_open_device(struct input_dev *dev)
{
	struct generaltouch_device *general_touch_single = input_get_drvdata(dev);

	general_touch_single->irq->dev = general_touch_single->usbdev;
	if (usb_submit_urb(general_touch_single->irq, GFP_KERNEL))
		return -EIO;

	return 0;
}

static void general_touch_single_close_device(struct input_dev *dev)
{
	struct generaltouch_device *general_touch_single = input_get_drvdata(dev);

	usb_kill_urb(general_touch_single->irq);
}

static bool general_touch_single_mkdev(void)
{
	int ret;

	dev_t dev_nb = MKDEV(GENERALTOUCH_DEVICE_MAJOR,GENERALTOUCH_DEVICE_MINOR);

	ret = register_chrdev_region(dev_nb,1,GENERALTOUCH_DEVICE_NAME);
	if(ret < 0)
	{
		printk("lamson dbg general_touch_single:  %s  failed to  register chrdev.",__func__);
		return false;
	}

	cdev_init(&general_touch_single_cdev,&generaltouchsingle_fops);
	general_touch_single_cdev.owner = THIS_MODULE;
	general_touch_single_cdev.ops = &generaltouchsingle_fops;
	ret = cdev_add(&general_touch_single_cdev,dev_nb,1);

	if(ret)
	{
		printk("lamson dbg general_touch_single: %s   add cdev error=%d",__func__,ret);
		return false;
	}

	generaltouchsingle_class = class_create(THIS_MODULE, GENERALTOUCH_DEVICE_NAME);
	if(IS_ERR(generaltouchsingle_class))
	{
		printk("lamson dbg general_touch_single: %s   class create failed.",__func__);
		return false;
	}
	
	device_create(generaltouchsingle_class,NULL,MKDEV(GENERALTOUCH_DEVICE_MAJOR,GENERALTOUCH_DEVICE_MINOR),NULL,GENERALTOUCH_DEVICE_NAME);

	printk("lamson dbg general_touch_single:    general_touch_single_mkdev success!");
	return true;
}

static int general_touch_single_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	struct usb_device *dev = interface_to_usbdev(intf);
	struct usb_host_interface *interface;
	struct usb_endpoint_descriptor *endpoint;
	struct generaltouch_device *general_touch_single;
	struct input_dev *input_dev;
	int pipe;
	int error = -ENOMEM;
	
	interface = intf->cur_altsetting;

	if (interface->desc.bNumEndpoints != 1)
		return -ENODEV;
	
	endpoint = &interface->endpoint[0].desc;
	if (!(endpoint->bEndpointAddress & USB_DIR_IN))
		return -ENODEV;
	if ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) != USB_ENDPOINT_XFER_INT)
		return -ENODEV;
	
	pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);
	
	general_touch_single= kzalloc(sizeof(struct generaltouch_device), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!general_touch_single || !input_dev)
		goto fail_dev;

	general_touch_single->data = usb_alloc_coherent(dev, USB_TRANSFER_LENGTH, GFP_ATOMIC, &general_touch_single->data_dma);
	if (!general_touch_single->data)
		goto fail_dev;
	
	general_touch_single->irq = usb_alloc_urb(0, GFP_KERNEL); 
	if (!general_touch_single->irq)
		goto fail_data;
	
	general_touch_single->usbdev = dev;
	general_touch_single->dev = input_dev;

	if (dev->manufacturer)
		strlcpy(general_touch_single->name, dev->manufacturer, sizeof(general_touch_single->name));

	if (dev->product) {
		if (dev->manufacturer)
			strlcat(general_touch_single->name, " ", sizeof(general_touch_single->name));
		strlcat(general_touch_single->name, dev->product, sizeof(general_touch_single->name));
	}

	if (!strlen(general_touch_single->name))
		snprintf(general_touch_single->name, sizeof(general_touch_single->name),
				"lamson:--generaltouch single-touch  %04x:%04x",
				le16_to_cpu(dev->descriptor.idVendor),
				le16_to_cpu(dev->descriptor.idProduct));
	usb_make_path(dev, general_touch_single->phys, sizeof(general_touch_single->phys));
	strlcat(general_touch_single->phys, "/input0", sizeof(general_touch_single->phys));

	input_dev->name = general_touch_single->name;
	input_dev->phys = general_touch_single->phys;
	usb_to_input_id(dev, &input_dev->id);
	input_dev->dev.parent = &intf->dev;

	input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
	input_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);

	input_set_abs_params(input_dev, ABS_X, 0, 32767, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, 32767, 0, 0);

	input_set_drvdata(input_dev, general_touch_single);

	input_dev->open = general_touch_single_open_device;
	input_dev->close = general_touch_single_close_device;
	
	usb_fill_int_urb(general_touch_single->irq, dev, pipe, general_touch_single->data, USB_TRANSFER_LENGTH,
			general_touch_single_irq, general_touch_single, endpoint->bInterval);
	general_touch_single->irq->transfer_dma = general_touch_single->data_dma;
	general_touch_single->irq->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

	error = input_register_device(general_touch_single->dev);

	usb_set_intfdata(intf, general_touch_single);

	error = usb_register_dev(intf, &general_touch_single_class_driver);
	if(error)
	{
		printk("lamson dbg general_touch_single:  %s failed to get a minor for generaltouch device.",__func__);
		usb_set_intfdata(intf,NULL);
		goto fail_reg;
	}

	msleep(1000);	
	general_touch_single_mkdev();
	printk("lamson :general_touch_single probe success!!!\n");

	return 0;

fail_reg:
	usb_free_urb(general_touch_single->irq);
fail_data:
	usb_free_coherent(dev, USB_TRANSFER_LENGTH, general_touch_single->data, general_touch_single->data_dma);
	
fail_dev:
	input_free_device(input_dev);
	kfree(general_touch_single);
	return -ENOMEM;
}

static void general_touch_single_disconnect(struct usb_interface *intf)
{
	struct generaltouch_device *general_touch_single = usb_get_intfdata (intf);


	dev_t dev_nb = MKDEV(GENERALTOUCH_DEVICE_MAJOR,GENERALTOUCH_DEVICE_MINOR);

	cdev_del(&general_touch_single_cdev);
	unregister_chrdev_region(dev_nb,1);

	device_destroy(generaltouchsingle_class,dev_nb);
	class_destroy(generaltouchsingle_class);

	usb_deregister_dev(intf, &general_touch_single_class_driver);

	usb_set_intfdata(intf, NULL);
	if(general_touch_single) 
	{
		usb_kill_urb(general_touch_single->irq);
		input_unregister_device(general_touch_single->dev);
		usb_free_urb(general_touch_single->irq);
		usb_free_coherent(interface_to_usbdev(intf), USB_TRANSFER_LENGTH, general_touch_single->data, general_touch_single->data_dma);
		kfree(general_touch_single);
	}

}

static struct usb_device_id general_touch_single_id_table [] = {
	{ USB_DEVICE(0x0dfc, 0x0001) },
	{ }   
};

MODULE_DEVICE_TABLE (usb, general_touch_single_id_table);

static struct usb_driver generaltouchsingle_usb_driver = {
	.name        = "general_touch_single",
	.probe        = general_touch_single_probe,
	.disconnect    = general_touch_single_disconnect,
	.id_table    = general_touch_single_id_table,
};

static int __init general_touch_single_init(void)
{
	int ret = usb_register(&generaltouchsingle_usb_driver);
	if (ret == 0)
		printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_VERSION ":" DRIVER_DESC);
	return ret;
}

static void __exit general_touch_single_exit(void)
{
	usb_deregister(&generaltouchsingle_usb_driver);
}

module_init(general_touch_single_init);
module_exit(general_touch_single_exit);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE(DRIVER_LICENSE);
MODULE_VERSION(DRIVER_VERSION);
