#include "rbuf.h"

//zugriff auf beide noch nicht threadsafe!
listener_t *listeners[MAX_LISTENER];
size_t listener_count;
rbuf_t *rbuf = NULL;

int init_rbuf(void) {
	//init rbuf
	int i = 0;
	rbuf_t *rbuf = kmalloc(sizeof(rbuf), __GFP_NORETRY);
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

char *read_next_entry(size_t cur_pos) {
	char *start = NULL;
	char *end = NULL;

	//ungültige positionen filtern
	if(cur_pos > rbuf->cur_tail) {
		return NULL;
	}
	start = rbuf->buffer + cur_pos;
	while(rbuf->buffer[cur_pos] != '\n' && rbuf->buffer[cur_pos] != '\0') {
		next_pos(&cur_pos);
	}
	end = rbuf->buffer + cur_pos;
	//XXX copy string
	return NULL;
}

void next_pos(size_t *cur_pos) {
	if(*cur_pos < RBUF_SIZE -1) {
		++(*cur_pos);
	} else {
		*cur_pos = 0;
	}
}

int register_listener(const unsigned long id) {
	listener_t *listener = NULL;
	//schauen ob wir nicht schon zu viele listener haben
	if(listener_count == MAX_LISTENER -1) {
		return MAX_LISTENER_REACHED;
	}

	//speicher für einen neuen listener holen
	listener = kmalloc(sizeof(listener), __GFP_NORETRY);
	if(!listener) {
		return LISTENER_MALLOC_FAILED;
	}

	listener->cur_pos = rbuf->cur_head;
	listener->id = id;

	listeners[listener_count] = listener;
	return 0;
}

int unregister_listener(const unsigned long id) {
	unsigned int i = 0;
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

