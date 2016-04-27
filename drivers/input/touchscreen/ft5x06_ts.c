/*
 * Touch Screen driver for Renesas MIGO-R Platform
 *
 * Copyright (c) 2008 Magnus Damm
 * Copyright (c) 2007 Ujjwal Pande <ujjwal@kenati.com>,
 *  Kenati Technologies Pvt Ltd. 
 *
 * This file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU  General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/input.h>
#include "ft5x06_ts.h"
#include <asm/div64.h>
#include <asm/io.h>
#include <linux/earlysuspend.h>
#include <linux/interrupt.h>
#include <linux/earlysuspend.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/timer.h>


static struct i2c_client *this_client;



#define CONFIG_FT5X0X_MULTITOUCH 1
#define FTS_NULL                0x0
#define FTS_TRUE                0x01
#define FTS_FALSE              0x0

struct ts_event {
	u16	x1;
	u16	y1;
	u16	x2;
	u16	y2;
	u16	x3;
	u16	y3;
	u16	x4;
	u16	y4;
	u16	x5;
	u16	y5;
	u16	pressure;
    u8  touch_point;
};

struct ft5x0x_ts_data {
	struct input_dev	*input_dev;
	struct ts_event		event;
	struct work_struct 	pen_event_work;
	struct workqueue_struct *ts_workqueue;
	struct early_suspend	early_suspend;
};
/***********************************************************************************************
Name	:	ft5x0x_i2c_rxdata 
Input	:	*rxdata  *length
Output	:	ret
function	:	
***********************************************************************************************/
static int ft5x0x_i2c_rxdata(char *rxdata, int length)
{
	int ret;

	struct i2c_msg msgs[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= 1,
			.buf	= rxdata,
		},
		{
			.addr	= this_client->addr,
			.flags	= I2C_M_RD,
			.len	= length,
			.buf	= rxdata,
		},
	};

    //msleep(1);
	ret = i2c_transfer(this_client->adapter, msgs, 2);
	if (ret < 0)
		pr_err("msg %s i2c read error: %d\n", __func__, ret);
	
	return ret;
}

/***********************************************************************************************
Name	:	 ft5x0x_i2c_txdata
Input	:	 *txdata, int length                     
Output	:	ret
function	:	
***********************************************************************************************/
static int ft5x0x_i2c_txdata(char *txdata, int length)
{
	int ret;

	struct i2c_msg msg[] = {
		{
			.addr	= this_client->addr,
			.flags	= 0,
			.len	= length,
			.buf	= txdata,
		},
	};
   	//msleep(1);
	ret = i2c_transfer(this_client->adapter, msg, 1);
	if (ret < 0)
		pr_err("%s i2c write error: %d\n", __func__, ret);

	return ret;
}
/***********************************************************************************************
Name	:	 ft5x0x_read_fw_ver
Input	:	 void     
Output	:	 firmware version 	

function	:	 read TP firmware version
************************************************************************************************/
static unsigned char ft5x0x_read_fw_ver(void)
{

	struct i2c_msg msgs[2];

       unsigned char ver;
       unsigned char txdata[2]={0};
       unsigned char rxdata[2]={0};
	int ret=-1;

        txdata[0]=0xfc;
        txdata[1]=0x7b;
        ret = ft5x0x_i2c_txdata(txdata,2); 

        msleep(2);

     if (ret < 0)
		pr_err("FW %s Write COMMAND Error: %d\n", __func__, ret);

	msgs[0].flags=I2C_M_RD;
	msgs[0].addr=this_client->addr;
	msgs[0].len=2;
	msgs[0].buf= rxdata;	
    //msleep(1);
	ret = i2c_transfer(this_client->adapter, msgs, 1);
	if (ret < 0)
		pr_err("FW %s read error: %d\n", __func__, ret);

	 ver = rxdata[0];
	
	return(ver);
}

/************************************************************************************************
[function]: 
    callback: read data from ctpm by i2c interface,implemented by special user;
[parameters]:
    bt_ctpm_addr[in]    :the address of the ctpm;
    pbt_buf[out]        :data buffer;
    dw_lenth[in]        :the length of the data buffer;
[return]:
    FTS_TRUE     :success;
    FTS_FALSE    :fail;
************************************************************************************************/
unsigned char i2c_read_interface(unsigned char bt_ctpm_addr, unsigned char* pbt_buf, unsigned int dw_lenth)
{
    int ret;
    
    ret=i2c_master_recv(this_client, pbt_buf, dw_lenth);

    if(ret<=0)
    {
        printk("[TSP]i2c_read_interface error\n");
        return FTS_FALSE;
    }
  
    return FTS_TRUE;
}

/************************************************************************************************
[function]: 
    callback: write data to ctpm by i2c interface,implemented by special user;
[parameters]:
    bt_ctpm_addr[in]    :the address of the ctpm;
    pbt_buf[in]        :data buffer;
    dw_lenth[in]        :the length of the data buffer;
[return]:
    FTS_TRUE     :success;
    FTS_FALSE    :fail;
    
    
************************************************************************************************/
unsigned char i2c_write_interface(unsigned char bt_ctpm_addr, unsigned char* pbt_buf, unsigned int dw_lenth)
{
    int ret;
    ret=i2c_master_send(this_client, pbt_buf, dw_lenth);
    if(ret<=0)
    {
        printk("[TSP]i2c_write_interface error line = %d, ret = %d\n", __LINE__, ret);
        return FTS_FALSE;
    }

    return FTS_TRUE;
}

/************************************************************************************************
[function]: 
    send a command to ctpm.
[parameters]:
    btcmd[in]        :command code;
    btPara1[in]    :parameter 1;    
    btPara2[in]    :parameter 2;    
    btPara3[in]    :parameter 3;    
    num[in]        :the valid input parameter numbers, if only command code needed and no parameters followed,then the num is 1;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
************************************************************************************************
unsigned char cmd_write(unsigned char btcmd,unsigned char btPara1,unsigned char btPara2,unsigned char btPara3,unsigned char num)
{
    unsigned char write_cmd[4] = {0};

    write_cmd[0] = btcmd;
    write_cmd[1] = btPara1;
    write_cmd[2] = btPara2;
    write_cmd[3] = btPara3;
    return i2c_write_interface(I2C_CTPM_ADDRESS, write_cmd, num);
}
*/
/************************************************************************************************
[function]: 
    write data to ctpm , the destination address is 0.
[parameters]:
    pbt_buf[in]    :point to data buffer;
    bt_len[in]        :the data numbers;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
************************************************************************************************
unsigned char byte_write(unsigned char* pbt_buf, unsigned int dw_len)
{
    
    return i2c_write_interface(I2C_CTPM_ADDRESS, pbt_buf, dw_len);
}
*/
/************************************************************************************************
[function]: 
    read out data from ctpm,the destination address is 0.
[parameters]:
    pbt_buf[out]    :point to data buffer;
    bt_len[in]        :the data numbers;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
/************************************************************************************************
unsigned char byte_read(unsigned char* pbt_buf, unsigned char bt_len)
{
    return i2c_read_interface(I2C_CTPM_ADDRESS, pbt_buf, bt_len);
}
*/
/***********************************************************************************************
Name	:	
Input	:	
Output	:	
function	:	
***********************************************************************************************/
static void ft5x0x_ts_release(void)
{
	struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);

	input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, 0);
	input_report_key(data->input_dev, BTN_TOUCH, 0);    
  input_mt_sync(data->input_dev);         
	input_sync(data->input_dev);
}
/***********************************************************************************************
Name	:	
Input	:	
Output	:	
function	:	
***********************************************************************************************/
static int ft5x0x_read_data(void)
{
	struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;

	u8 buf[26] = {0};
       unsigned long x_tmp[5]={0},y_tmp[5]={0};
	
	int ret = -1;	

       buf[0]=0xf9;
	     ret = ft5x0x_i2c_rxdata(buf, 26);

    if (ret < 0) {
		printk("%s read_data i2c_rxdata failed: %d\n", __func__, ret);
		return ret;	
                           }
	memset(event, 0, sizeof(struct ts_event));

	event->touch_point = buf[3] & 0x07;// 000 0111

    if (event->touch_point == 0) {
        ft5x0x_ts_release();
        return 1; 
    }


    switch (event->touch_point) {
		case 5:

                       x_tmp[4] = (((buf[21] & 0x0f) << 8 | buf[22]))*SCREEN_MAX_X;
                       do_div(x_tmp[4],MAX_X_CTP); 
                       event->x5= (s16)x_tmp[4];

                       y_tmp[4] = (((buf[23] & 0x0f) << 8 | buf[24]))*SCREEN_MAX_Y;
                       do_div(y_tmp[4],MAX_Y_CTP);
                       event->y5 =(s16)y_tmp[4];	
                

		case 4:
			 x_tmp[3] = (((buf[17] & 0x0f) << 8 | buf[18]))*SCREEN_MAX_X;
                       do_div(x_tmp[3],MAX_X_CTP); 
                       event->x4 = (s16)x_tmp[3];

                       y_tmp[3] = (((buf[19] & 0x0f) << 8 | buf[20]))*SCREEN_MAX_Y;
                       do_div(y_tmp[3],MAX_Y_CTP);
                       event->y4 =(s16)y_tmp[3];	   

		case 3:
                       x_tmp[2] = (((buf[13] & 0x0f) << 8 | buf[14]))*SCREEN_MAX_X;
                       do_div(x_tmp[2],MAX_X_CTP); 
                       event->x3 = (s16)x_tmp[2];

                       y_tmp[2] = (((buf[15] & 0x0f) << 8 | buf[16]))*SCREEN_MAX_Y;
                       do_div(y_tmp[2],MAX_Y_CTP);
                       event->y3 =(s16)y_tmp[2];	   

		case 2:
                       x_tmp[1] = (((buf[9] & 0x0f) << 8 | buf[10]))*SCREEN_MAX_X;
                       do_div(x_tmp[1],MAX_X_CTP); 
                       event->x2 = (s16)x_tmp[1];

                       y_tmp[1] = (((buf[11] & 0x0f) << 8 | buf[12]))*SCREEN_MAX_Y;
                       do_div(y_tmp[1],MAX_Y_CTP);
                       event->y2 =(s16)y_tmp[1];	   
			
		case 1:
			 x_tmp[0] = (((buf[5] & 0x0f) << 8 | buf[6]))*SCREEN_MAX_X;
                       do_div(x_tmp[0],MAX_X_CTP); 
                       event->x1 = (s16)x_tmp[0];

                       y_tmp[0] = (((buf[7] & 0x0f) << 8 | buf[8]))*SCREEN_MAX_Y;
                       do_div(y_tmp[0],MAX_Y_CTP);
                       event->y1 =(s16)y_tmp[0];	   
            break;
		default:
		    return -1;
	}
    event->pressure = 200;

	dev_dbg(&this_client->dev, "%s: 1:%d %d 2:%d %d \n", __func__,
		event->x1, event->y1, event->x2, event->y2);
	//printk("%d (%d, %d), (%d, %d)\n", event->touch_point, event->x1, event->y1, event->x2, event->y2);

    return 0;
}
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static void ft5x0x_report_value(void)
{
	struct ft5x0x_ts_data *data = i2c_get_clientdata(this_client);
	struct ts_event *event = &data->event;	

	switch(event->touch_point) {
		case 5:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x5);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y5);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			    input_report_key(data->input_dev, BTN_TOUCH, 1); 
			input_mt_sync(data->input_dev);
//			printk("===x2 = %d,y2 = %d ====\n",event->x2,event->y2);
		case 4:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x4);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y4);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			    input_report_key(data->input_dev, BTN_TOUCH, 1); 
			input_mt_sync(data->input_dev);
//			printk("===x2 = %d,y2 = %d ====\n",event->x2,event->y2);
		case 3:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x3);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y3);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			    input_report_key(data->input_dev, BTN_TOUCH, 1); 
			input_mt_sync(data->input_dev);
//			printk("===x2 = %d,y2 = %d ====\n",event->x2,event->y2);
		case 2:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x2);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y2);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			    input_report_key(data->input_dev, BTN_TOUCH, 1); 
			input_mt_sync(data->input_dev);
//			printk("===x2 = %d,y2 = %d ====\n",event->x2,event->y2);
		case 1:
			input_report_abs(data->input_dev, ABS_MT_TOUCH_MAJOR, event->pressure);
			input_report_abs(data->input_dev, ABS_MT_POSITION_X, event->x1);
			input_report_abs(data->input_dev, ABS_MT_POSITION_Y, event->y1);
			input_report_abs(data->input_dev, ABS_MT_WIDTH_MAJOR, 1);
			input_report_key(data->input_dev, BTN_TOUCH, 1); 
			input_mt_sync(data->input_dev);
			//printk("===x1 = %d,y1 = %d ====\n",event->x1,event->y1);

		default:
//			printk("==touch_point default =\n");
			break;
	}
	input_sync(data->input_dev);

	dev_dbg(&this_client->dev, "%s: 1:%d %d 2:%d %d \n", __func__,
		event->x1, event->y1, event->x2, event->y2);
}	/*end ft5x0x_report_value*/

/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static void ft5x0x_ts_pen_irq_work(struct work_struct *work)
{
	int ret = -1;
//	printk("==work 1=\n");
	ret = ft5x0x_read_data();	
	if (ret == 0) {	
		ft5x0x_report_value();
	}
//	else printk("data package read error\n");
//	printk("==work 2=\n");
//    	msleep(1);
  enable_irq(this_client->irq);
//	enable_irq(IRQ_EINT(8));
}
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static irqreturn_t ft5x0x_ts_interrupt(int irq, void *dev_id)
{
	struct ft5x0x_ts_data *ft5x0x_ts = dev_id;
	//disable_irq(IRQ_EINT(8));
	disable_irq_nosync(this_client->irq);	//	
	
	if (!work_pending(&ft5x0x_ts->pen_event_work)) {
		queue_work(ft5x0x_ts->ts_workqueue, &ft5x0x_ts->pen_event_work);
	}
/*
  disable_irq_nosync(irq);
	schedule_delayed_work(&ft5x0x_ts->work, HZ / 100);*/
	
	return IRQ_HANDLED;
}

/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static int ft5x0x_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct ft5x0x_ts_data *ft5x0x_ts;
	struct input_dev *input_dev;
	int err = 0;
	unsigned char uc_reg_value; 
	
	printk("==ft5x0x_ts_probe=\n");
	
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		err = -ENODEV;
		goto exit_check_functionality_failed;
	}

	printk("==kzalloc=\n");
	ft5x0x_ts = kzalloc(sizeof(*ft5x0x_ts), GFP_KERNEL);
	if (!ft5x0x_ts)	{
		err = -ENOMEM;
		goto exit_alloc_data_failed;
	}


	this_client = client;
	i2c_set_clientdata(client, ft5x0x_ts);


	INIT_WORK(&ft5x0x_ts->pen_event_work, ft5x0x_ts_pen_irq_work);

	ft5x0x_ts->ts_workqueue = create_singlethread_workqueue(dev_name(&client->dev));
	if (!ft5x0x_ts->ts_workqueue) {
		err = -ESRCH;
		goto exit_create_singlethread;
	}

	//err = request_irq(IRQ_EINT(8), ft5x0x_ts_interrupt, IRQF_TRIGGER_FALLING, "ft5x0x_ts", ft5x0x_ts);
	err = request_irq(client->irq, ft5x0x_ts_interrupt, IRQF_TRIGGER_FALLING, "ft5x06_ts", ft5x0x_ts);
	if (err < 0) {
		dev_err(&client->dev, "ft5x0x_probe: request irq failed\n");
		goto exit_irq_request_failed;
	}

	//disable_irq(IRQ_EINT(8));
	disable_irq(this_client->irq);


	input_dev = input_allocate_device();
	if (!input_dev) {
		err = -ENOMEM;
		dev_err(&client->dev, "failed to allocate input device\n");
		goto exit_input_dev_alloc_failed;
	}
	
	ft5x0x_ts->input_dev = input_dev;

#ifdef CONFIG_FT5X0X_MULTITOUCH
  set_bit(EV_ABS, input_dev->evbit);
	set_bit(ABS_MT_TOUCH_MAJOR, input_dev->absbit);
	set_bit(ABS_MT_POSITION_X, input_dev->absbit);
	set_bit(ABS_MT_POSITION_Y, input_dev->absbit);
	set_bit(ABS_MT_WIDTH_MAJOR, input_dev->absbit);

	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_X, 0, SCREEN_MAX_X-1, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_POSITION_Y, 0, SCREEN_MAX_Y-1, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_TOUCH_MAJOR, 0, PRESS_MAX, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_MT_WIDTH_MAJOR, 0, 200, 0, 0);
#else
	set_bit(ABS_X, input_dev->absbit);
	set_bit(ABS_Y, input_dev->absbit);
	set_bit(ABS_PRESSURE, input_dev->absbit);
	set_bit(BTN_TOUCH, input_dev->keybit);

	input_set_abs_params(input_dev, ABS_X, 0, SCREEN_MAX_X, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, SCREEN_MAX_Y, 0, 0);
	input_set_abs_params(input_dev,
			     ABS_PRESSURE, 0, PRESS_MAX, 0 , 0);
#endif

	set_bit(EV_SYN, input_dev->evbit);
   set_bit(BTN_TOUCH, input_dev->keybit);
	 set_bit(EV_KEY, input_dev->evbit);

	input_dev->name		= FT5X0X_NAME;		//dev_name(&client->dev)
	err = input_register_device(input_dev);
	if (err) {
		dev_err(&client->dev,
		"ft5x0x_ts_probe: failed to register input device: %s\n",
		dev_name(&client->dev));
		goto exit_input_register_device_failed;
	}



    msleep(50);

    uc_reg_value = ft5x0x_read_fw_ver();
    printk("[FST] Firmware version = 0x%x\n", uc_reg_value);

    //enable_irq(IRQ_EINT(8));
    enable_irq(this_client->irq);

	printk("==probe over =\n");
    return 0;

exit_input_register_device_failed:
	input_free_device(input_dev);
exit_input_dev_alloc_failed:
free_irq(client->irq, ft5x0x_ts);
	//free_irq(IRQ_EINT(8), ft5x0x_ts);
exit_irq_request_failed:
	cancel_work_sync(&ft5x0x_ts->pen_event_work);
	destroy_workqueue(ft5x0x_ts->ts_workqueue);
exit_create_singlethread:
	printk("==singlethread error =\n");
	i2c_set_clientdata(client, NULL);
	kfree(ft5x0x_ts);
exit_alloc_data_failed:
exit_check_functionality_failed:
	return err;
}
/***********************************************************************************************
Name	:	 

Input	:	
                     

Output	:	

function	:	

***********************************************************************************************/
static int ft5x0x_ts_remove(struct i2c_client *client)
{

	struct ft5x0x_ts_data *ft5x0x_ts = i2c_get_clientdata(client);
	//free_irq(IRQ_EINT(8), ft5x0x_ts);
	free_irq(client->irq, ft5x0x_ts);
	input_unregister_device(ft5x0x_ts->input_dev);
	kfree(ft5x0x_ts);
	cancel_work_sync(&ft5x0x_ts->pen_event_work);
	destroy_workqueue(ft5x0x_ts->ts_workqueue);
	i2c_set_clientdata(client, NULL);

	return 0;
}
/*******************************************************************************
Name	:	
Input	:	
Output	:	
function	:	
********************************************************************************/
static void ft5x0x_ts_suspend(struct early_suspend *handler)
{
//	 struct ft5x0x_ts_data *ft5x0x_ts = i2c_get_clientdata(client);
	 printk("==ft5x0x_ts_suspend=\n");
/*	if (device_may_wakeup(&this_client->dev))
		enable_irq_wake(ft5x0x_ts->irq);

	return 0;*/
}
/*******************************************************************************
Name	:	
Input	:	
Output	:	
function	:	
********************************************************************************/
static void ft5x0x_ts_resume(struct early_suspend *handler)
{
    printk("==ft5x0x_ts_resume=\n");
	/* struct ft5x0x_ts_data *ft5x0x_ts = i2c_get_clientdata(client);
	 
	if (device_may_wakeup(&this_client->dev))
		disable_irq_wake(ft5x0x_ts->irq);

	return 0;*/
}
/***********************************************************************************************
Name	:	 
Input	:	 
Output	:
function	:	
***********************************************************************************************/
static const struct i2c_device_id ft5x0x_ts_id[] = {
	{ FT5X0X_NAME, 0 },{ }
};
MODULE_DEVICE_TABLE(i2c, ft5x0x_ts_id);
/***********************************************************************************************
Name	:	 
Input	:	 
Output	:
function	:	
***********************************************************************************************/
static struct i2c_driver ft5x0x_ts_driver = {
	.driver	= {
		.name	= FT5X0X_NAME,	
	},
	.probe		= ft5x0x_ts_probe,
	.remove		= ft5x0x_ts_remove,	
	.id_table	= ft5x0x_ts_id,
	

};

/***********************************************************************************************
Name	:	 
Input	:	 
Output	:
function	:	
***********************************************************************************************/
static int __init ft5x0x_ts_init(void)
{
	return i2c_add_driver(&ft5x0x_ts_driver);
}

/***********************************************************************************************
Name	:	 
Input	:	
Output	:	
function	:	
***********************************************************************************************/
static void __exit ft5x0x_ts_exit(void)
{
	i2c_del_driver(&ft5x0x_ts_driver);
}

MODULE_AUTHOR("<jimmy_wu@edt.com.tw>");
MODULE_DESCRIPTION("EDT SYSTEM FocalTech ft5x06  driver");
MODULE_LICENSE("GPL");


module_init(ft5x0x_ts_init);
module_exit(ft5x0x_ts_exit);

