/*
 * Kernel driver for the TDT4258 controller board
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/fcntl.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/signal.h>
#include <asm/siginfo.h>

#include "efm32gg.h"

static dev_t dev;
static struct cdev cdev;
static struct class *cl;
static volatile struct efm32gg_gpio *GPIO;
static uint8_t status = 0;
static struct fasync_struct *fasync_queue;

static int gamepad_fasync(int fd, struct file *filp, int mode) {
	return fasync_helper(fd, filp, mode, &fasync_queue);
}

static int gamepad_open(struct inode *node, struct file *filp)
{
	return 0;
}

static int gamepad_release(struct inode *node, struct file *filp)
{
	gamepad_fasync(-1, filp, 0);
	return 0;
}

static ssize_t gamepad_read(struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
	if(count <= 0) return 0;
	copy_to_user(buf, &status, 1);
	return 1;
}

static ssize_t gamepad_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp)
{
	return 0;
}

static struct file_operations gamepad_fops = {
	.owner = THIS_MODULE,
	.open = gamepad_open,
	.release = gamepad_release,
	.read = gamepad_read,
	.write = gamepad_write,
	.fasync = gamepad_fasync
};

irqreturn_t gamepad_handler(int irq, void *dev) {
	status = ~ioread8(&GPIO->PC.DIN);
	if(fasync_queue) kill_fasync(&fasync_queue, SIGIO, POLL_IN);
	iowrite32(ioread32(&GPIO->IF), &GPIO->IFC);
	return IRQ_HANDLED;
}

/*
 * gamepad_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init gamepad_init(void)
{
	if(alloc_chrdev_region(&dev, 0, 1, "gamepad") < 0) return -1;
	printk(KERN_INFO "Major: %d Minor: %d\n", MAJOR(dev), MINOR(dev));
	cdev_init(&cdev, &gamepad_fops);
	cdev_add(&cdev, dev, 1);
	cl = class_create(THIS_MODULE, "gamepad");
	device_create(cl, NULL, dev, NULL, "gamepad");

	request_mem_region((unsigned long)GPIO_PHYS, sizeof(*GPIO), "gamepad");
	printk(KERN_INFO "Remapping %08x\n", (unsigned int)GPIO_PHYS);
	GPIO = ioremap_nocache((unsigned long)GPIO_PHYS, sizeof(*GPIO));
	printk(KERN_INFO "ioremap: %08x\n", (unsigned int)GPIO_PHYS);

	iowrite32(0x33333333, &GPIO->PC.MODEL);
	iowrite32(0xff, &GPIO->PC.DOUT);
	iowrite32(0x22222222, &GPIO->EXTIPSELL);
	iowrite32(0xff, &GPIO->EXTIFALL);
	iowrite32(0xff, &GPIO->EXTIRISE);
	iowrite32(0xff, &GPIO->IEN);
	iowrite32(0xff, &GPIO->IFC);

	request_irq(17, gamepad_handler, 0, "gamepad", NULL);
	request_irq(18, gamepad_handler, 0, "gamepad", NULL);

	return 0;
}

/*
 * gamepad_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit gamepad_cleanup(void)
{
	iowrite32(0, &GPIO->IEN);
	free_irq(17, NULL);
	free_irq(18, NULL);
	iounmap(GPIO);
	release_mem_region((unsigned long)GPIO_PHYS, sizeof(*GPIO));
	device_destroy(cl, dev);
	class_destroy(cl);
	cdev_del(&cdev);
	unregister_chrdev_region(dev, 1);
}

module_init(gamepad_init);
module_exit(gamepad_cleanup);

MODULE_DESCRIPTION("TDT4258 gamepad driver");
MODULE_LICENSE("GPL");
