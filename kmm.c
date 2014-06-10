#include "kmm.h"
#include "printm.h"
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/device.h>
#include "rbuf.h"

#define CLASS_NAME "kmm_class"
#define DEVICE_NAME "mod_test"
#define DRIVER_NAME "kernel_monitor"

struct cdev *vcdev = NULL;
dev_t vdev;
struct class *myclass = NULL;

struct file_operations fops = {
	.read=device_read,
	.write=device_write,
	.open=device_open,
	.release=device_release
};

struct device *mydevice = NULL;

int kmm_init(void) {
	int result = 0;

	if(alloc_chrdev_region(&vdev, 0, 1, DRIVER_NAME)) {
		printk("alloc_chrdev_region failed\n");
		return -EIO;
	}

	printk("KMM: Dynamic device number assignment: DN=%d MAJOR=%d MINOR=%d\n", vdev, MAJOR(vdev),MINOR(vdev));

	vcdev = cdev_alloc();

	if(!vcdev) {
		return -EIO;
	}

	//init stuff
	if(init_rbuf() < 0) {
		printk("init_rbuf failed\n");
		return -EIO;
	}
	init_printm();
	cdev_init(vcdev, &fops);
	kobject_set_name(&vcdev->kobj, DRIVER_NAME);
	vcdev->owner = THIS_MODULE;


	result = cdev_add(vcdev, vdev, 1);
	if(result < 0) {
		printk("adding device failed\n");
		goto fail_device;
	}

	
	myclass = class_create(vcdev->owner, CLASS_NAME);
	if(!myclass)
		goto fail_class;

	mydevice = device_create(myclass, NULL, vdev, NULL, DEVICE_NAME);
	if(!mydevice)
		goto fail_device;


	printm("hello world1");
	printm("hello world2");
	printm("hello world3");
	return 1;

fail_device:
	if(myclass) {
		class_unregister(myclass);
		class_destroy(myclass);
		myclass = NULL;
	}
fail_class:
	if(vcdev) {
		cdev_del(vcdev);
		vcdev = NULL;
	}
	return -EIO;
}

void kmm_exit(void) {
	exit_rbuf();
	if(mydevice && myclass) {
		device_destroy(myclass, vcdev->dev);
	}

	if(vcdev)
		cdev_del(vcdev);
		unregister_chrdev_region(MAJOR(vcdev->dev), 1);

	if(myclass) {
		class_unregister(myclass);
		class_destroy(myclass);
		myclass = NULL;
	}
}

void print_debug(void) {
	printk("dev->%i\n", MAJOR(vcdev->dev));
}

int device_open(struct inode *inode, struct file *file) {
	register_listener((unsigned long) file);
	printk("device_open %lu\n", (unsigned long) file);
	
	return 0;
}

int device_release(struct inode *inode, struct file *file) {
	unregister_listener((unsigned long) file);
	printk("device_release\n");
	return 0;
}

ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
	size_t written = 0;
	printk("read offset %i\n", *offset);
	check_listeners();	
	written = read_next_entry((unsigned long) filp, buffer, length);
	return written;
}

ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
	char tmp_buf[1000];
	int i = 0;
	for(i = 0; i < len; i++) {
		printk("adding %c from %i\n", buff[i], i);
		tmp_buf[i] = buff[i];
	}
	if(len) {
		tmp_buf[len] = '\0';
	}
	return printm(tmp_buf);
}
