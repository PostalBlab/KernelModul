#include "kmm.h"
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/device.h>

struct cdev *vcdev = NULL;
dev_t vdev;
struct class *myclass = NULL;
struct file_operations fops = {
	read: device_read,
	write: device_write,
	open: device_open,
	release: device_release
};
struct device *mydevice = NULL;

int kmm_init(void) {

	vcdev = cdev_alloc();

	if(!vcdev) {
		return -EIO;
	}

	cdev_init(vcdev, &fops);


	cdev_add(vcdev, MAJOR(vcdev->dev), 0);

	// returns alway 0...
	kobject_set_name(&(vcdev->kobj), "kmm_test");
	
	alloc_chrdev_region(&(vcdev->dev), 0, 1, "mod_test");
	myclass = class_create(vcdev->owner, "kmm_test");
	if(!myclass)
		goto fail_class;

	mydevice = device_create(myclass, NULL, vcdev->dev, NULL, "mod_test");
	if(!mydevice)
		goto fail_device;


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
	if(mydevice && myclass) {
		device_destroy(myclass, vcdev->dev);
	}

	if(vcdev)
		cdev_del(vcdev);
		unregister_chrdev_region(MAJOR(vcdev->dev), 0);

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
	printk("device_open\n");
	return -1;
}

int device_release(struct inode *inode, struct file *file) {
	printk("device_release\n");
	return -1;
}

ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
	printk("device_read\n");
	return -1;
}

ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
	printk("device_write\n");
	return -1;
}
