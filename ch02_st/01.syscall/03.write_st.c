#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct _INFO {
	char name[20];
	int age;
	float height;
} INFO;

int main(int argc, char *argv[])
{
	INFO data[5] = {"kim",20,170.1F,
					"lee",25,180.3F,
					"park",21,176.2F,
					"choi",28,167.9F,
					"kang",25,172.5F};
	int fd;
	int i;

	if(...............)
	{
		perror("open");
		exit(1);
	}
	for(i=0; i<5; i++)
	{
		if(.............) {
			perror("write");
			......
			exit(2);
		}
	}
	.......
	return 0;
}
