#include <linux/module.h>
#include <linux/version.h>
#include "mod_test.h"
#include "../printm.h"

static int hertz_multiplier = 0;
module_param_named(hz, hertz_multiplier, int, 0755);
DECLARE_DELAYED_WORK(workq, freq_message);
struct workqueue_struct *wq;

int (*sym)(char *format, ...) = NULL;
char termflag = 0;

static int init_mod_test(void) {
	sym = symbol_get(printm);
	if(!sym) {
		printk("oh noes symbol does not exists\n");
		return -1;
	} else {
		wq = create_workqueue("freq_call");
		INIT_DELAYED_WORK(&workq, freq_message);
		queue_delayed_work(wq, &workq, hertz_multiplier * HZ);	
	}
	return 0;
}

static void exit_mod_test(void) {
	termflag = 1;
	cancel_delayed_work(&workq);
	flush_delayed_work(&workq);
	destroy_workqueue(wq);

	if(sym) {
		symbol_put(printm);
	}
}

void freq_message(struct work_struct *bla) {
	static unsigned int i = 0;
	if(termflag == 0) {
		if(sym)
			sym("test %i\n", i++);
		queue_delayed_work(wq, &workq, hertz_multiplier * HZ);
	}
	return;
}

module_init(init_mod_test);

module_exit(exit_mod_test);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniel Sparka");
MODULE_DESCRIPTION("Bla");
