/*
 * CM3623 head driver,it would combine to CM3623_light.c and CM3623_prox.c
 *
 * 
 *
 * Copyright (C) 2011 Vincent,Wang Winmate Inc.
 * Licensed under the GPL-2 or later.
 */

#include <linux/input.h>	/* BUS_I2C */
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/types.h>
#include <asm/uaccess.h> 
#include <linux/platform_device.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/kernel.h>


#include <linux/completion.h>
#include <linux/crc-ccitt.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/mutex.h>
#include <linux/nfc/pn544.h>
#include <linux/poll.h>
#include <linux/regulator/consumer.h>
#include <linux/serial_core.h> /* for TCGETS */
#include <linux/slab.h>



#define KEYGEN_MAJ 308
#define MotionSensor_IRQ        IMX_GPIO_NR(1, 11)

static struct input_dev *moInput_dev=0;


static char pName[32];
static char pPhyName[32];
static struct timer_list rpeattimer;

const int irq_num = gpio_to_irq(MotionSensor_IRQ);
void *irq_dev_id = (void*)&irq_num;
static int irqflag = 0;

static irqreturn_t MotionSensor_irq_detect_thread_fn(int irq, void *dev_id)
{
 	irqflag = gpio_get_value(MotionSensor_IRQ);
	printk("Motion Sensor IRQ Test!!! irqflag = %d\n", irqflag);

	
	if (irqflag == 0) {	
		input_event(moInput_dev, EV_MSC, MSC_SCAN, BTN_SELECT);
        	input_report_key(moInput_dev, BTN_SELECT, 1);
        	input_sync(moInput_dev);
	} else if (irqflag == 1) {
	 	input_event(moInput_dev, EV_MSC, MSC_SCAN, BTN_SELECT);
        	input_report_key(moInput_dev, BTN_SELECT, 0);
        	input_sync(moInput_dev);
	}

        return IRQ_HANDLED;
}

static int __devinit MotionSensor_probe(struct platform_device *pdev)
{
	int r = 0;
	
        r = request_threaded_irq(irq_num, NULL, MotionSensor_irq_detect_thread_fn,
                                IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING, "MotionSensor",
                                irq_dev_id);
	int err;

        moInput_dev = input_allocate_device();
        if(!moInput_dev)
       		return -1;

        moInput_dev->name = pName;
        moInput_dev->phys = pPhyName;
        moInput_dev->dev.parent = &pdev->dev;
        moInput_dev->id.bustype = BUS_HOST;
        moInput_dev->id.product = 0;
        moInput_dev->id.vendor = 0x0001;
        moInput_dev->id.version = 0x0101;
        moInput_dev->open = 0;
        moInput_dev->close = 0;

        set_bit(EV_KEY, moInput_dev->evbit);

        input_set_capability(moInput_dev, EV_KEY, BTN_SELECT);
        set_bit(BTN_SELECT, moInput_dev->keybit);
       
        clear_bit(0, moInput_dev->keybit);
        input_set_capability(moInput_dev, EV_MSC, MSC_SCAN);
        err = input_register_device(moInput_dev);

       if(err)
       {
       		printk("MotionSensor Probe error = %d\n", err);
       }


       return 0;	
}

static int __devexit MotionSensor_remove(struct platform_device *pdev)
{
	if(moInput_dev)
	{
		input_unregister_device(moInput_dev);
	 	free_irq(irq_num, irq_dev_id);

	}
	return 0;
}



static struct platform_driver motion_sensor_driver = {
	.probe		= MotionSensor_probe,
	.remove		= __devexit_p(MotionSensor_remove),
	.driver		= {
		.name	= "motion_sensor",
		.owner	= THIS_MODULE,
	}
};

static int __init MotionSensorInit(void)
{
	printk("motion sensor init willy\n");
	return platform_driver_register(&motion_sensor_driver);
	
}
module_init(MotionSensorInit);

static void __exit MotionSensorExit(void)
{
	platform_driver_unregister(&motion_sensor_driver);
}
module_exit(MotionSensorExit);

MODULE_AUTHOR("Willy Wang<willy_wang@winmate.com.tw>");
MODULE_DESCRIPTION("Motion Sensor irq driver");
MODULE_LICENSE("GPL");

