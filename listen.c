#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
	int fd = open("/dev/mod_test", 	O_RDONLY);
	if(fd == -1) {
		printf("error opening file\n");
		exit(1);
	}
	char buffer[2048];
	while(1) {
		size_t read_bytes = read(fd, buffer, 2048);
		if(read_bytes > 0)
			printf("%s", buffer);
		sleep(1);
	}
}
