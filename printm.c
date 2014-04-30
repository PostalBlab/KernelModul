#include "printm.h"
#include <stdarg.h>
#include <linux/version.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/module.h>

static list_element_t *head;
static list_element_t *tail;
DEFINE_SPINLOCK(head_lock);
DEFINE_SPINLOCK(tail_lock);

void init_printm(void) {
	head = NULL;
	tail = NULL;
}

int printm(char *format, ...) {
	va_list parameter;

	char *buffer = NULL;

	size_t size = 0;

	va_start(parameter, format);

	//kmalloc buffer and write to buffer
	buffer = kmalloc(sizeof(char) * 1024, __GFP_NORETRY);
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
	list_element_t *new_element = kmalloc(sizeof(list_element_t), __GFP_NORETRY);
	if(!new_element) {
		//kmalloc went wrong
		return;
	}

	//init new element
	new_element->message = message;
	new_element->next = NULL;
	
	if(!head) {
		spin_lock(&head_lock);
		head = new_element;	
		spin_unlock(&head_lock);

		spin_lock(&tail_lock);
		tail = new_element;
		spin_unlock(&tail_lock);
	} else {
		spin_lock(&tail_lock);
		tail->next = new_element;
		tail = new_element;
		spin_unlock(&tail_lock);
	}
}

char *dequeue(void) {
	//locking...
	char *tmp_message = NULL;
	list_element_t *tmp_element;
	if(!head)
		return NULL;

	spin_lock(&head_lock);
	tmp_message = head->message;
	tmp_element = head;
	if(head == tail) {
		tail = NULL;
		head = NULL;
	} else {
		head = head->next;
	}
	spin_unlock(&head_lock);

	kfree(tmp_element);
	return tmp_message;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniel Sparka");
MODULE_DESCRIPTION("Bla alles super hui");
