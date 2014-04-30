#include "kmm.h"
#include <linux/types.h>
#include <linux/fs.h>

struct cdev *vcdev = NULL;
dev_t vdev;

int kmm_init(void) {
	vcdev = cdev_alloc();
	if(!vcdev) {
		return -1;
	}
	
	alloc_chrdev_region(&(vcdev->dev), 123, 20, "mod_test");
	return 1;
}

void kmm_exit(void) {
	if(vcdev)
		cdev_del(vcdev);
}

void print_debug(void) {
	printk("dev->%i\n", MKDEV(vcdev->dev));
}
