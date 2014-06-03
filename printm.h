#ifndef PRINTM_H_
#define PRINTM_H_

void init_printm(void);
int printm(char *format, ...);
void enqueue(char *message);
char *dequeue(void);

typedef struct list_element {
	char *message;
	struct list_element* next;
} list_element_t;

#endif
