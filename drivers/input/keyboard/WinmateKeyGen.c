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



#define KEYGEN_MAJ 308

int KeyGenDeviceEnum=0;
static struct input_dev *pInput_dev=0;


static int KeyGenOpen(struct inode *inode, struct file *file)
{	
	KeyGenDeviceEnum++;
	return 0;
}
static int KeyGenRelease(struct inode *inode, struct file *file)
{
	 KeyGenDeviceEnum--;	
	  return 0;
}
int strtoint(char *inputstr);

unsigned char KeyMap[][3]=
{
		{'0',KEY_0,0},
		{'1',KEY_1,0},			
		{'2',KEY_2,0},
		{'3',KEY_3,0},
		{'4',KEY_4,0},
		{'5',KEY_5,0},
		{'6',KEY_6,0},
		{'7',KEY_7,0},
		{'8',KEY_8,0},
		{'9',KEY_9,0},

		{')',KEY_0,1},
		{'!',KEY_1,1},			
		{'@',KEY_2,1},
		{'#',KEY_3,1},
		{'$',KEY_4,1},
		{'%',KEY_5,1},
		{'^',KEY_6,1},
		{'&',KEY_7,1},
		{'*',KEY_8,1},
		{'(',KEY_9,1},

		{',',KEY_COMMA,0},			
		{'<',KEY_COMMA,1},
		//{';',KEY_SEMICOLON,0},  //standard 101
		{';',KEY_COMMA,1},   //for LIME on screen keyboad
		//{':',KEY_SEMICOLON,1}, //standard 101
		{':',KEY_DOT,1}, //for LIME on screen keyboard
		{'.',KEY_DOT,0},
		{'>',KEY_DOT,1},
		{'/',KEY_SLASH,0},
		{'?',KEY_SLASH,1},
		
		{' ',KEY_SPACE,0},			
		{13,KEY_ENTER,0},
		{9,KEY_TAB,0},
		{'`',KEY_GRAVE,0},
		{'~',KEY_GRAVE,1},
		{'-',KEY_MINUS,0},
		{'_',KEY_MINUS,1},
		{'=',KEY_EQUAL,0},
		{'+',KEY_EQUAL,1},			
		
		{'[',KEY_LEFTBRACE,0},
		{'{',KEY_LEFTBRACE,1},
		{']',KEY_RIGHTBRACE,0},
		{'}',KEY_RIGHTBRACE,1},
		{'\\',KEY_BACKSLASH,0},
		{'|',KEY_BACKSLASH,1},
		{'\'',KEY_APOSTROPHE,0},
		{'\"',KEY_APOSTROPHE,1},
		
		{'a',KEY_A,0},
		{'A',KEY_A,1},
		{'b',KEY_B,0},
		{'B',KEY_B,1},
		{'c',KEY_C,0},
		{'C',KEY_C,1},
		{'d',KEY_D,0},
		{'D',KEY_D,1},
		{'e',KEY_E,0},
		{'E',KEY_E,1},
		{'f',KEY_F,0},
		{'F',KEY_F,1},
		{'g',KEY_G,0},
		{'G',KEY_G,1},
		{'h',KEY_H,0},
		{'H',KEY_H,1},
		{'i',KEY_I,0},
		{'I',KEY_I,1},
		{'j',KEY_J,0},
		{'J',KEY_J,1},
		{'k',KEY_K,0},
		{'K',KEY_K,1},
		{'l',KEY_L,0},
		{'L',KEY_L,1},
		{'m',KEY_M,0},
		{'M',KEY_M,1},
		{'n',KEY_N,0},
		{'N',KEY_N,1},
		{'o',KEY_O,0},
		{'O',KEY_O,1},
		{'p',KEY_P,0},
		{'P',KEY_P,1},
		{'q',KEY_Q,0},
		{'Q',KEY_Q,1},
		{'r',KEY_R,0},
		{'R',KEY_R,1},
		{'s',KEY_S,0},
		{'S',KEY_S,1},
		{'t',KEY_T,0},
		{'T',KEY_T,1},
		{'u',KEY_U,0},
		{'U',KEY_U,1},
		{'v',KEY_V,0},
//		{'V',KEY_V,1},
//perry add
//              {'V',KEY_SEARCH,1},
		{'V',KEY_PASTE,1},
		{'w',KEY_W,0},
		{'W',KEY_W,1},
		{'x',KEY_X,0},
		{'X',KEY_X,1},
		{'y',KEY_Y,0},
		{'Y',KEY_Y,1},
		{'z',KEY_Z,0},
		{'Z',KEY_Z,1},
									
};


static ssize_t KeyGenWrite(struct file *filp,
	   const char *buff,
		 size_t len,
		 loff_t *off)
	{
		char bbuf[512];
		int retlen;
		char key;
		int i=0,j=0;
		int cap=0;
		retlen=copy_from_user(bbuf,buff,len);
		bbuf[len]=0;
		
		printk("str=%s\n",bbuf);
				

		
		for(i=0;i<len;i++)
		{
			cap=0;
			//printk("[%d]=0x%X\n",i,bbuf[i]);
			for(j=0;j<sizeof(KeyMap)/3;j++)
			{
				if(bbuf[i]==KeyMap[j][0])
				{
					cap=KeyMap[j][2];
					key=KeyMap[j][1];
					break;
				}
			}
			
			if(j>=sizeof(KeyMap)/3)				
				continue;

			if(i%5==0) 
				msleep(10);

			if(pInput_dev)
			{
				//if(cap)
				//{
					//input_event(pInput_dev, EV_MSC, MSC_SCAN, KEY_MENU);
					//input_report_key(pInput_dev,KEY_MENU, 1);

				//}
				
				input_event(pInput_dev, EV_MSC, MSC_SCAN, key);
				input_report_key(pInput_dev,key, 1);

				input_sync(pInput_dev);

				input_event(pInput_dev, EV_MSC, MSC_SCAN, key);
				input_report_key(pInput_dev,key, 0);


				//if(cap)
				//{
					//input_event(pInput_dev, EV_MSC, MSC_SCAN, KEY_MENU);
					//input_report_key(pInput_dev,KEY_MENU, 0);

				//}

				input_sync(pInput_dev);

			}

		}
		input_sync(pInput_dev);
	   return len;
	}
	
 struct file_operations KeyGen_fops = {
       write: KeyGenWrite,
       open: KeyGenOpen,
       release: KeyGenRelease
    };



static char pName[32];
static char pPhyName[32];
static struct timer_list rpeattimer;

void TimerRefresh(unsigned long a)
{

	  rpeattimer.expires = jiffies + HZ*5;
	 add_timer(&rpeattimer);
	  
	input_event(pInput_dev, EV_KEY, KEY_1, 1);
	input_sync(pInput_dev);

	input_event(pInput_dev, EV_KEY, KEY_1, 0);
	input_sync(pInput_dev);
}
int KeyArray[]=
{
KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_0,
KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,KEY_M,
KEY_N,KEY_O,KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,
KEY_LEFTSHIFT,KEY_RIGHTSHIFT,KEY_COMMA,KEY_DOT,KEY_SLASH,KEY_SPACE,KEY_ENTER,KEY_TAB,
KEY_GRAVE,KEY_MINUS,KEY_EQUAL,KEY_LEFTBRACE,KEY_RIGHTBRACE,KEY_BACKSLASH,KEY_APOSTROPHE,
KEY_MENU,KEY_BACK,KEY_SEARCH,KEY_PASTE
};

static int __devinit WinamteGenKey_probe(struct platform_device *pdev)
{

		int err;		
		int i;
		pInput_dev = input_allocate_device();
		if(!pInput_dev)
			return -1;
		
		sprintf(pName,"WinmateVirtualKeyGen");
		sprintf(pPhyName,"WinmateKeyGen/input0");
		pInput_dev->name = pName;		
		pInput_dev->phys = pPhyName;
		pInput_dev->dev.parent = &pdev->dev;
		pInput_dev->id.bustype = BUS_HOST;
		pInput_dev->id.product = 0;
		pInput_dev->id.vendor = 0x0001;	
		pInput_dev->id.version = 0x0101;
		pInput_dev->open = 0;
		pInput_dev->close = 0;
		
		set_bit(EV_KEY, pInput_dev->evbit);
		for(i=0;i<sizeof(KeyArray)/sizeof(int);i++)
		{
			input_set_capability(pInput_dev, EV_KEY, KeyArray[i]);
			set_bit(KeyArray[i], pInput_dev->keybit);
		}
		clear_bit(0, pInput_dev->keybit);
		
		input_set_capability(pInput_dev, EV_MSC, MSC_SCAN);
		
		
		err = input_register_device(pInput_dev);					
		if(err) 
		{
				printk("Vincent -------KeyGen input subsys register error\n");
		}		
		err=register_chrdev(KEYGEN_MAJ, "keygen",&KeyGen_fops);	
		if(err) 
		{
				printk("Vincent -------KeyGen chrdev register error\n");
		}		
		
		//init_timer(&rpeattimer);  
		//rpeattimer.function = TimerRefresh;
		//rpeattimer.data = ((unsigned long) 0);
		//rpeattimer.expires = jiffies+ HZ*5;
		//add_timer(&rpeattimer); 
	
	
	return 0;	
}

static int __devexit WinmateKeyGen_remove(struct platform_device *pdev)
{
	if(pInput_dev)
	{
		input_unregister_device(pInput_dev);
		pInput_dev=0;
	}
	return 0;
}



static struct platform_driver winmate_keygen_driver = {
	.probe		= WinamteGenKey_probe,
	.remove		= __devexit_p(WinmateKeyGen_remove),
	.driver		= {
		.name	= "WinmateKeyGen",
		.owner	= THIS_MODULE,
	}
};

static int __init WinmateKeyGenInit(void)
{
	return platform_driver_register(&winmate_keygen_driver);
	
}
module_init(WinmateKeyGenInit);

static void __exit WinmateKeyGenExit(void)
{
	platform_driver_unregister(&winmate_keygen_driver);
}
module_exit(WinmateKeyGenExit);

MODULE_AUTHOR("Vincent Wang <vincent_wang@winmate.com.tw>");
MODULE_DESCRIPTION("Virtual Key driver");
MODULE_LICENSE("GPL");

