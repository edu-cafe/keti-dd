/***************************************
 * Filename: led_app.c
 * Title: LED Device Application
 * Desc: Implementation of system call
 ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>

int main(void)
{
    int fd;
    int led_val=0, i, cnt, ret;
	pid_t pid;
    
    switch(pid = fork())
    {
      case -1:    // Fork Error
        perror("LED_app");
        exit(-1);
      case 0:    // Chile Process
        for(i=0; ;i++)
        {
          printf("[C]Child_%d...\n", i);
          sleep(1);
        }
        exit(0);
    }

    // Parent Process
    fd = open("/dev/myled", O_RDWR);
    //fd = open("/dev/myled", O_RDWR|O_NONBLOCK);
    if (fd<0) {
        perror("/dev/myled");
        exit(-1);
    }
    else printf("[APP]LED detected...\n");
    for(i=1, cnt=0; i<=10; i++)
    {
        while(1)
        {
            printf("[P]Waiting LED Write Operation...\n");			
			led_val = (1<<8) | cnt;
			if(( ret = write(fd, &led_val, sizeof(led_val)) ) > 0) break;
            sleep(1);			
        }
		if(++cnt > 7) cnt = 0;
        printf("--->[P]LED Write Ok(%d)!!\n", i);        
    }
    close(fd);

	kill(pid, SIGKILL);
    
    return 0;
}
