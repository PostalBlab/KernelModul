#include "rbuf.h"

//zugriff auf beide noch nicht threadsafe!
listener_t *listeners[MAX_LISTENER];
size_t listener_count;
rbuf_t *rbuf = NULL;

void exit_rbuf(void) {
	unregister_all_listener();

	if(rbuf)
		kfree(rbuf);
}

int init_rbuf(void) {
	//init rbuf
	int i = 0;
	rbuf = kmalloc(sizeof(rbuf_t), __GFP_NORETRY);
	//memset ist im kernelspace nicht verfügbar bzw. sollte nicht benutzt werden
	for(i = 0; i < RBUF_SIZE; i++) {
		rbuf->buffer[i] = '\0';
	}
	rbuf->cur_head = 0;
	rbuf->cur_tail = 0;

	//init listeners
	for(i = 0; i < MAX_LISTENER; i++) {
		listeners[i] = NULL;
	}
	listener_count = 0;

	return 0;
}

//gibt zurück wie viele byte geschrieben wurden
size_t read_next_entry(const unsigned int id, char *user_buffer, const size_t buffer_length) {
	size_t written = 0;
	
	listener_t *cur_listener = get_listener(id);
	if(!cur_listener) {
		return 0;
	}

	while(written < buffer_length - 1) {
		user_buffer[written++] = rbuf->buffer[cur_listener->cur_pos];
		next_pos(&(cur_listener->cur_pos));
	}
	user_buffer[written++] = '\0';

	return written;
}

void next_pos(size_t *cur_pos) {
	if(*cur_pos < RBUF_SIZE -1) {
		++(*cur_pos);
	} else {
		*cur_pos = 0;
	}
}

listener_t *get_listener(const unsigned long id) {
	unsigned int i = 0;
	while(listeners[i]) {
		if(listeners[i]->id == id) {
			return listeners[i];
		}
	}

	return NULL;
}

int register_listener(const unsigned long id) {
	listener_t *listener = NULL;
	printk("register listener %lu\n", id);
	//schauen ob wir nicht schon zu viele listener haben
	if(listener_count == MAX_LISTENER -1) {
		return MAX_LISTENER_REACHED;
	}
	++listener_count;

	//speicher für einen neuen listener holen
	listener = kmalloc(sizeof(listener_t), __GFP_NORETRY);
	if(!listener) {
		return LISTENER_MALLOC_FAILED;
	}

	listener->cur_pos = rbuf->cur_head;
	listener->id = id;

	listeners[listener_count - 1] = listener;
	return 0;
}

int unregister_listener(const unsigned long id) {
	unsigned int i = 0;
	printk("unregister listener %lu\n", id);
	for(i = 0; i < MAX_LISTENER_REACHED; i++) {
		if(!listeners[i]) {
			return LISTENER_NOT_FOUND;
		}
		if(listeners[i]->id == id) {
			listener_t *tmp = listeners[i];
			listeners[i] = NULL;
			--listener_count;
			if(listener_count)
				listeners[i] = listeners[listener_count - 1];
			kfree(tmp);
			return 0;
		}
	}
	return LISTENER_NOT_FOUND;
}

void unregister_all_listener(void) {
	unsigned int i = 0;
	for(i = 0; i < MAX_LISTENER; i++) {
		if(listeners[i]) {
			kfree(listeners[i]);
			listeners[i] = NULL;
		}
	}
}