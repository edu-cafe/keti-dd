#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int fd;
    int menu=0, led_func=0, led_no=0;
    int led_stat, ret;
    
    fd = open("/dev/myled", O_RDWR);  
    if (fd<0) {
        perror("/dev/myled");
        exit(-1);
    }
    else
        printf("[APP]LED detected...\n");
    
    while(1)
    {
        printf("\n\n===== LED Test Program ====\n");
        printf(" 1. LED On\n");
        printf(" 2. LED Off\n");
        printf(" 3. LED Status\n");
        printf(" 0. Program Quit\n");
        do {
            printf(" Select Menu => ");
            scanf("%d", &menu);
        } while(menu<0 || menu>3);

        if(menu == 0) break;

        do {
	    printf("\nInput LED Number(0~7,8:all) => ");
            scanf("%d", &led_no);
        } while(menu<0 || menu>7);

		led_func = menu;
		menu = menu << 8;
		menu |= led_no;

		switch(led_func) {
		case 1:
		case 2:
			printf("write call(menu:%x)\n", menu);
			write(fd, &menu, sizeof(menu));
			break;
		case 3:
			printf("read call(menu:%x)\n", menu);
			led_stat = 0;
			ret=read(fd, &led_stat, sizeof(led_stat));
			printf("read result(led_stat:%x,ret:%d)\n", led_stat, ret);
			break;
		}
    }
    close(fd);    
    return 0;
}
