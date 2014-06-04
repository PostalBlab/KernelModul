#ifndef RBUF_H_
#define RBUF_H_

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>

#define RBUF_SIZE 2048
#define MAX_LISTENER 2048

//error defines
#define MAX_LISTENER_REACHED -1
#define LISTENER_MALLOC_FAILED -2
#define LISTENER_NOT_FOUND -3

typedef struct {
	char buffer[RBUF_SIZE];
	size_t cur_head;
	size_t cur_tail;
} rbuf_t;

typedef struct {
	size_t cur_pos;
	unsigned long id;
} listener_t;

void exit_rbuf(void);
int init_rbuf(void);
size_t read_next_entry(const unsigned int id, char *user_buffer, const size_t buffer_length);
void next_pos(size_t *cur_pos);
int add_entry(const char *new_entry);
void next_rbuf(void);

listener_t *get_listener(const unsigned long id);
int register_listener(const unsigned long id);
int unregister_listener(const unsigned long id);
void unregister_all_listener(void);
void check_listeners(void);

#endif
