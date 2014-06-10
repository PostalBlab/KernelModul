#include "printm.h"
#include <stdarg.h>
#ifdef __KERNEL__
#include <linux/version.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/module.h>
#else
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#endif

static list_element_t *head;
static list_element_t *tail;
size_t queue_size;
#ifdef __KERNEL__
DEFINE_SPINLOCK(head_lock);
DEFINE_SPINLOCK(tail_lock);
#endif

void init_printm(void) {
	head = NULL;
	tail = NULL;
	queue_size = 0;
}

int printm(char *format, ...) {
	va_list parameter;

	char *buffer = NULL;

	size_t size = 0;

	va_start(parameter, format);

	//kmalloc buffer and write to buffer
	#ifdef __KERNEL__
	buffer = kmalloc(sizeof(char) * 1024, __GFP_NORETRY);
	#else 
	buffer = malloc(sizeof(char) * 1024);
	#endif
	
	if(!buffer) {
		return -1;
	}

	size = vsnprintf(buffer, 1023, format, parameter);

	va_end(parameter);

	enqueue(buffer);

	return size;
}

void enqueue(char *message) {
	//lock...
	#ifdef __KERNEL__
	list_element_t *new_element = kmalloc(sizeof(list_element_t), __GFP_NORETRY);
	#else
	list_element_t *new_element = malloc(sizeof(list_element_t));
	#endif
	if(!new_element) {
		//kmalloc went wrong
		return;
	}

	//init new element
	new_element->message = message;
	new_element->next = NULL;
	
	//TODO check for max size and dequeue stuff
	if(!head) {
		#ifdef __KERNEL__
		spin_lock(&head_lock);
		#endif
		head = new_element;	
		#ifdef __KERNEL__
		spin_unlock(&head_lock);
		#endif

		#ifdef __KERNEL__
		spin_lock(&tail_lock);
		#endif
		tail = new_element;
		#ifdef __KERNEL__
		spin_unlock(&tail_lock);
		#endif
	} else {
		#ifdef __KERNEL__
		spin_lock(&tail_lock);
		#endif
		tail->next = new_element;
		tail = new_element;
		#ifdef __KERNEL__
		spin_unlock(&tail_lock);
		#endif
	}
}

char *dequeue(void) {
	//locking...
	char *tmp_message = NULL;
	list_element_t *tmp_element;
	if(!head)
		return NULL;

	#ifdef __KERNEL__
	spin_lock(&head_lock);
	#endif
	tmp_message = head->message;
	tmp_element = head;
	if(head == tail) {
		tail = NULL;
		head = NULL;
	} else {
		head = head->next;
	}
	#ifdef __KERNEL__
	spin_unlock(&head_lock);
	#endif

	#ifdef __KERNEL__
	kfree(tmp_element);
	#else
	free(tmp_element);
	#endif
	if(tmp_message)
		printk("dequeuingasf %s\n", tmp_message);
	return tmp_message;
}

#ifdef __KERNEL__
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniel Sparka");
MODULE_DESCRIPTION("Bla alles super hui");
#endif
