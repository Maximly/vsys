/*
 *  GPIO interface for IT87xxx Super I/O chip
 *
 *  Author: dongxiangan <dongxiangan@163.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License 2 as published
 *  by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/reboot.h>
#include <linux/notifier.h>
#include <linux/moduleparam.h>

#include <linux/gpio.h>

#include "sio_gpio.h"

#define OEM_DEBUG_ON   0


#define IO_INDEX                   0x4E
#define P_INDEX                    IO_INDEX
#define P_DATA                     (P_INDEX+1)

static DEFINE_SPINLOCK(sio_lock);

#define GPIO_NAME		"hzx_gpio"
#define SIO_REG_LDSEL               0x07
#define SIO_GPIO_LDN                0x08
#define GPIO_BA_HIGH_BYTE	          0x60
#define GPIO_BA_LOW_BYTE	          0x61
#define GPIO_IOSIZE		              0x02
#define SIO_UNLOCK_KEY1             0x87
#define SIO_UNLOCK_KEY2             0x87
#define SIO_INOUT_REG               0x01
#define SIO_DATA_REG                0x02

#define SIO_LOCK_KEY1               0xAA
#define SIO_CID_REG                 0x20

#define MAX_GPIO_NR                 0x08
#define MAX_GPIO_PIN                MAX_GPIO_NR

#define GPIO_LOWLEVEL               0x00
#define GPIO_HIGHLEVEL              0x01

static	unsigned int chip_id;

/**************************************************************/

static inline int superio_inb(int base, int reg);
static inline int superio_inw(int base, int reg);
static inline void superio_outb(int base, int reg, u8 val);
static inline int superio_enter(int base);
static inline void superio_select(int base, int ld);
static inline void superio_exit(int base);

static int conf_gpio_inout(u8 pin,u8 input);
static int set_gpio_level(u8 pin,u8 level);
static int get_gpio_level(u8 pin,u8 *level);
static void gpio_group_location_conf(u16 base);

u16 base_addr;
u16 base_inout_addr;
u16 base_data_addr;
u8 gpio_group_addr[8];
u8 gpio_location_addr[8];

static inline int superio_inb(int base, int reg)
{
	outb(reg, base);
	return inb(base + 1);
}

static int superio_inw(int base, int reg)
{
	int val;
	val  = superio_inb(base, reg) << 8;
	val |= superio_inb(base, reg + 1);
	return val;
}

static inline void superio_outb(int base, int reg, u8 val)
{
	outb(reg, base);
	outb(val, base + 1);
}

static inline int superio_enter(int base)
{
	if (!request_muxed_region(base, 2, GPIO_NAME)) {
		pr_err("I/O address 0x%02x already in use\n", IO_INDEX);
		return -EBUSY;
	}
//	printk(KERN_INFO"%s request_region 0x%x successful\n",__FUNCTION__,IO_INDEX);
	outb(SIO_UNLOCK_KEY1, base);
	outb(SIO_UNLOCK_KEY2, base);
	return 0;
}

static inline void superio_select(int base, int ld)
{
	outb(SIO_REG_LDSEL, base);
	outb(ld, base + 1);
}

static inline void superio_exit(int base)
{
	outb(SIO_LOCK_KEY1, base);
	release_region(base, 2);
}

ssize_t gpio_write(struct file *file, const char __user *data,
		       size_t len, loff_t *ppos)
{
	return len;
}


ssize_t gpio_read(struct file *file, char __user * buf,
		      size_t len, loff_t * ppos)
{
	return len;
}


static long gpio_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int rc = 0;
	u8 pin_index = 0;
	u8 iospace = 0;
	
	if (_IOC_TYPE(cmd) != TAIO_IOCTL_BASE) return -ENOTTY;
	if (_IOC_NR(cmd) > TAIO_MAX_NR) return -ENOTTY;
		
	switch(cmd){
		case SET_GPIO_LOWLEVEL:
			spin_lock(&sio_lock);
			if (get_user(pin_index, (u8 __user *)arg)){
				spin_unlock(&sio_lock);
				return -EFAULT;
			}
			if(pin_index >= MAX_GPIO_PIN){
				spin_unlock(&sio_lock);
				return -EINVAL;
			}
			set_gpio_level(pin_index, GPIO_LOWLEVEL);
			spin_unlock(&sio_lock);
			return rc;
		case SET_GPIO_HIGHLEVEL:
			spin_lock(&sio_lock);
			if (get_user(pin_index, (u8 __user *)arg)){
				spin_unlock(&sio_lock);
				return -EFAULT;
			}
			if(pin_index >= MAX_GPIO_PIN){
				spin_unlock(&sio_lock);
				return -EINVAL;
			}
			set_gpio_level(pin_index, GPIO_HIGHLEVEL);
			spin_unlock(&sio_lock);
			return rc;
		case GET_GPIO_LEVEL:
			spin_lock(&sio_lock);
			if (get_user(pin_index, (u8 __user *)arg)){
				spin_unlock(&sio_lock);
				return -EFAULT;
			}

			if(pin_index >= MAX_GPIO_PIN){
				spin_unlock(&sio_lock);
				return -EINVAL;
			}
			get_gpio_level(pin_index,&iospace);

			if (put_user(iospace, (u8 __user *)arg)){
				spin_unlock(&sio_lock);
				return -EFAULT;
			}
			spin_unlock(&sio_lock);
			return rc;
	default:
		return -ENOTTY;
	}
}

static int conf_gpio_inout(u8 pin,u8 input)
{
	u8 regvalue;
	u8 *pgroup = gpio_group_addr;
	u8 *plocation = gpio_location_addr;
	if (!request_muxed_region(base_addr, 1, GPIO_NAME)) {
		pr_err("conf_gpio_inout request I/O address 0x%02x already in use\n", IO_INDEX);
		return -EBUSY;
	}
 
	if(input == SET_GPIO_INPUT){
		outb(*(pgroup+pin), base_addr);
		regvalue = inb(base_inout_addr);
		regvalue &= ~(0x01 << (*(plocation+pin)));
		regvalue |= (0x01 << (*(plocation+pin)));
		outb(regvalue, base_inout_addr);
	}else{
		outb(*(pgroup+pin), base_addr);
		regvalue = inb(base_inout_addr);
		regvalue &= ~(0x01 << (*(plocation+pin)));
		outb(regvalue, base_inout_addr);		
	}
	release_region(base_addr, 1);
	return 0;
}

static int set_gpio_level(u8 pin,u8 level)
{
	u8 regvalue;
	u8 *pgroup = gpio_group_addr;
	u8 *plocation = gpio_location_addr;
	if (!request_muxed_region(base_addr, 1, GPIO_NAME)) {
		pr_err("set_gpio_level request I/O address 0x%02x already in use\n", IO_INDEX);
		return -EBUSY;
	}

	if(level == GPIO_HIGHLEVEL){
		outb(*(pgroup+pin), base_addr);
		regvalue = inb(base_data_addr);
		regvalue &= ~(0x01 << (*(plocation+pin)));
		regvalue |= (0x01 << (*(plocation+pin)));
		outb(regvalue, base_data_addr);
	}else{
		outb(*(pgroup+pin), base_addr);
		regvalue = inb(base_data_addr);
		regvalue &= ~(0x01 << (*(plocation+pin)));
		outb(regvalue, base_data_addr);
	}
	release_region(base_addr, 1);
	return 0;
}

static int get_gpio_level(u8 pin,u8 *level)
{
	u8 regvalue;
	u8 *pgroup = gpio_group_addr;
	u8 *plocation = gpio_location_addr;
	if (!request_muxed_region(base_addr, 1, GPIO_NAME)) {
		pr_err("get_gpio_level request I/O address 0x%02x already in use\n", IO_INDEX);
		return -EBUSY;
	}

	outb(*(pgroup+pin), base_addr);
	regvalue = inb(base_data_addr);
	if(regvalue & (0x01 << *(plocation+pin))){
		*level = 1;
	}else{
		*level = 0;
	}
	
	release_region(base_addr, 1);
	return 0;
}

static void gpio_group_location_conf(u16 base)
{
	u8 ngroup,nlocat,chkloop;
	u8 *pgroup = gpio_group_addr;
	u8 *plocation = gpio_location_addr;
	u8 *ppin = gpio_pin;
	for(chkloop = 0; chkloop < MAX_GPIO_NR; chkloop++){
		ngroup = *(ppin+chkloop) /10;
		nlocat = *(ppin+chkloop) %10;
		*(pgroup+chkloop) = ngroup;
		*(plocation+chkloop) = nlocat;
	}
	base_inout_addr = base+SIO_INOUT_REG;
	base_data_addr = base+SIO_DATA_REG;
}

static int gpio_open(struct inode *inode, struct file *file)
{
	return nonseekable_open(inode, file);
}

static const struct file_operations gpio_fileops = {
	.owner					= THIS_MODULE,
	.open						= gpio_open,
	.unlocked_ioctl = gpio_ioctl,
	.write					= gpio_write,
	.read						= gpio_read,
	.llseek 				= no_llseek,
};

static struct miscdevice gpio_miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = GPIO_NAME,
	.fops = &gpio_fileops,
};

static int __init gpio_init(void)
{
	int rc;
	rc = superio_enter(IO_INDEX);
	if(rc){
		printk(KERN_INFO"%s enter pnp mode fail",__FUNCTION__);
		superio_exit(IO_INDEX);		
		return rc;
	}
	chip_id = superio_inw(IO_INDEX,SIO_CID_REG);
	if (chip_id != SIO_CID) {
		superio_exit(IO_INDEX);
		printk(KERN_INFO"%s get chip_id is 0x%x \n",__FUNCTION__,chip_id);
		return -ENODEV;
	}
	superio_select(IO_INDEX,SIO_GPIO_LDN);
	base_addr = superio_inw(IO_INDEX,GPIO_BA_HIGH_BYTE);
	superio_exit(IO_INDEX);
	printk(KERN_INFO"%s get base_addr is 0x%x \n",__FUNCTION__,base_addr);
	if (!request_region(base_addr+SIO_INOUT_REG, GPIO_IOSIZE, GPIO_NAME)){
		printk(KERN_INFO"%s request_region base_addr+SIO_INOUT_REG is failed\n",__FUNCTION__);
		return -EBUSY;
	}
	printk(KERN_INFO"%s request_region 0x%x successful\n",__FUNCTION__,(base_addr+SIO_INOUT_REG));
	rc = misc_register(&gpio_miscdev);
	if (rc) {
		pr_err("cannot register miscdev on minor=%d (err=%d)\n",
		       MISC_DYNAMIC_MINOR, rc);
		release_region(base_addr+SIO_INOUT_REG, GPIO_IOSIZE);
	}
	gpio_group_location_conf(base_addr);

	printk(KERN_INFO"%s GPIO Loaded driver successful\n",__FUNCTION__);
	
	return 0;
}

static void __exit gpio_exit(void)
{
	misc_deregister(&gpio_miscdev);
	release_region(base_addr+SIO_INOUT_REG, GPIO_IOSIZE);
}
module_init(gpio_init);
module_exit(gpio_exit);

MODULE_AUTHOR("dongxiangan <dongxiangan@163.com>");
MODULE_DESCRIPTION("GPIO interface for Super IO chip IT8728F V1.1");
MODULE_LICENSE("GPL");
