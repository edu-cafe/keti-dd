/***************************************
 * Filename: key_app.c
 * Title: KEY Device Application
 * Desc: Implementation of system call
 ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int fd_key;
    int menu=0;
    int key_stat, ret;
    int i;
    
    fd_key = open("/dev/mykey", O_RDWR);
    if (fd_key<0) {
        perror("/dev/mykey");
        exit(-1);
    }
    else printf("[APP]KEY detected...\n");
    
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
        printf("\n\nKEY Input Waiting...\n");
		key_stat = 0; ret = 0xff;
        ret = read(fd_key, &key_stat, sizeof(key_stat));
        printf("[APP]key_status:%x(ret:%d)\n", key_stat, ret);
    }

    close(fd_key);
    
    return 0;
}
