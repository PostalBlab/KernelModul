#include <linux/module.h>
#include <linux/version.h>
#include "printm.h"
#include "kmm.h"

static int init_mod_test(void) {
	//init_printm();
	if(kmm_init() == -1) {
		printk("kmm_init() failed\n");
	} else {
		printk("kmm_init() supi\n");
		print_debug();
	}

	return 0;
}

static void exit_mod_test(void) {
	kmm_exit();
	printk("test module exit\n");
}


module_init(init_mod_test);

module_exit(exit_mod_test);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniel Sparka");
MODULE_DESCRIPTION("Bla");
