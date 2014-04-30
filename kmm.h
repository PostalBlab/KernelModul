#ifndef KMM_H_
#define KMM_H_

#include <linux/cdev.h>

int kmm_init(void);
void kmm_exit(void);
void print_debug(void);

#endif
