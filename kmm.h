#ifndef KMM_H_
#define KMM_H_

#include <linux/cdev.h>

int kmm_init(void);
void kmm_exit(void);
void print_debug(void);

//device stuff

int device_open(struct inode *inode, struct file *file);
int device_release(struct inode *inode, struct file *file);
ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset);
ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off);

#endif
