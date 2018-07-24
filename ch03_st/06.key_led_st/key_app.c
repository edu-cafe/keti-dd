/***************************************
 * Filename: key_app.c
 * Title: KEY Device Application
 * Desc: Implementation of system call
 ***************************************/
 //key press -> led on(during 3sec) & off 
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int fd_key, fd_led;
    int menu=0;
    int key_stat = 0, led_val=0, ret;
    int i;
    
    fd_key = open("/dev/mykey", O_RDWR);
    if (fd_key<0) {
        perror("/dev/mykey");
        exit(-1);
    }
    else printf("[APP]KEY detected...\n");
	
	fd_led = open("/dev/myled", O_RDWR);
    if (fd_led<0) {
        perror("/dev/myled");
        exit(-1);
    }
    else printf("[APP]LED detected...\n");
    
    while(1)
    {
        printf("\n\n===== KEY Test Program ====\n");
        printf(" 1. KEY Check\n");
        printf(" 0. Program Quit\n");
        do {
            printf(" Select Menu => ");
            scanf("%d", &menu);
        } while(menu<0 || menu>1);

        if(menu == 0) break;

		for(i=1, key_stat=0, led_val=0; i<=2; i++) {
			printf("\n\nKEY Checking...\n");
			ret=read(fd_key, &key_stat, sizeof(key_stat));
			printf("[APP]read result(key_stat:%x,ret:%d)\n", key_stat, ret);
			if(key_stat) {
				led_val = (1 << 8) | (key_stat%8);
				printf("[APP]led_write(led_val:%x)\n", led_val);
				write(fd_led, &led_val, sizeof(led_val)); 
				sleep(3);
				led_val = (2 << 8) | 8;
				printf("[APP]led_write(led_val:%x)\n", led_val);
				write(fd_led, &led_val, sizeof(led_val)); 
			} else printf("[APP]Key is not Pressed!!\n");
			//sleep(1);
		}
    }

    close(fd_key);
	close(fd_led);
    
    return 0;
}

