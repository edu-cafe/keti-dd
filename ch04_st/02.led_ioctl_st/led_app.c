#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include ........
#define	DEVICE_FILENAME	"/dev/myled"

int main(void)
{
	int fd;
	int tx_data, rx_data;
	int ret;
	unsigned int flag;
	
	fd = open(DEVICE_FILENAME, O_RDWR);
	printf("After_open:Press Enter to continue...\n");
	getchar();
	if(fd < 0) {
		perror("/dev/myled file open error!!");
		exit(-1);
	} else printf("/dev/myled file open ok!!\n");

	tx_data = (1 << 8) | 2;		//LED_2 On
	printf("app => write request(tx_data:%x)!!\n", tx_data);
	ret = write(fd, &tx_data, sizeof(int));
	printf("After write:Press Enter to continue...\n");
	getchar();
	printf("app => write done(ret:%d)!!\n", ret);

	printf("app => ioctl request(cmd:LED_ON)!!\n");
	tx_data = (1 << 8) | 4;		//LED_4 On
	ret = ioctl(fd, LED_ON, &tx_data);
	printf("After ioctl:Press Enter to continue...\n");
	getchar();
	printf("app => ioctl done(ret:%d)!!\n", ret);
	
	printf("app => ioctl request(cmd:LED_GETSTATE)!!\n");
	rx_data = 0;
	ret = ioctl(fd, LED_GETSTATE, &rx_data);
	printf("After ioctl:Press Enter to continue...\n");
	getchar();
	printf("app => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);	

	printf("app => ioctl request(cmd:LED_OFF)!!\n");
	tx_data = (2 << 8) | 2;		//LED_2 Off
	ret = ioctl(fd, LED_OFF, &tx_data);
	printf("After ioctl:Press Enter to continue...\n");
	getchar();
	printf("app => ioctl done(ret:%d)!!\n", ret);

	close(fd);
	return 0;
}

