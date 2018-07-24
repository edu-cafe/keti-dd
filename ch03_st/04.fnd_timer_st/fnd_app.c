#include	<stdio.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include	<string.h>

#define fnd_dev	"/dev/myfnd"

int main(int argc, char *argv[])
{
	int fnd_num;
	int fnd_fd = 0;
	if(argc<2) {
		printf("Usage : %s num(0~15)\n", argv[0], argv[1]);
		exit(1);
	}
	fnd_num = atoi(argv[1]);
	fnd_fd = open(fnd_dev, O_RDWR);
	if(fnd_fd <0){
		printf("Can't Open Device\n");
		exit(1);
	}
	printf("[APP]fnd(%d) write request!!\n", fnd_num);
	write(fnd_fd, &fnd_num, sizeof(fnd_num));	
	printf("Press Enter-Key to quit...");
	getchar();
	close(fnd_fd);
	return 0;
}


