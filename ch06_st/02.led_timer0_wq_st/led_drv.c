#include <linux/module.h>
//#include <linux/kernel.h>
//#include <linux/init.h>

#include <linux/fs.h>

#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/uaccess.h>

#include <linux/interrupt.h>   	//request_irq
#include <mach/irqs.h>			//IRQ_TIMER0_VIC
#include <plat/map-base.h>		//S3C_VA_TIMER
#include <plat/regs-timer.h>	//S3C2410_TCON
#include <linux/io.h>			//__raw_readl

#include <linux/wait.h>
#include <linux/sched.h>		//TASK_INTERRUPTIBLE
#include <linux/delay.h>		//mdelay()

#define DEVICE_NAME "LED_MY"
int DEVICE_MAJOR = 0;

volatile unsigned int flag_timer0Int=0;
volatile unsigned int flag_timeron=0;
________________(wq_timer);

// Device Address
#define LED     0x0A

#define ADDR_PIN_L_0(x)           EXYNOS4212_GPM0(x+1)
#define ADDR_PIN_L_1(x)           EXYNOS4212_GPM1(x)

#define ADDR_PIN_H_1(x)           EXYNOS4212_GPM1(x+1)

#define DATA_PIN_2(x)             EXYNOS4212_GPM2(x)
#define DATA_PIN_3(x)             EXYNOS4212_GPM3(x)
#define DATA_PIN_4(x)             EXYNOS4212_GPM4(x)
#define DATA_PIN_0(x)             EXYNOS4212_GPM0(x)

//#define	SEC_CLK		(16*1000)
#define	SEC_CLK		(24*1000)

int timerPeriod=1, timerClkVal=3*SEC_CLK;

irqreturn_t timer0_int(int irq, void *dev_id);
void TIMER0_ON(void)
{
	unsigned long tcon;

	__raw_writel((0<<8)|(0xFF), S3C2410_TCFG0); 
	__raw_writel(4, S3C2410_TCFG1); 
	__raw_writel((0<<4)|(1<<3)|(0<<2)|(0<<1)|(0), S3C2410_TCON); 
	__raw_writel(0, S3C2410_TCNTB(0)); 
	__raw_writel(0, S3C2410_TCMPB(0)); 


#if 0
	__raw_writel(0, S5P_VIC0REG(VIC_PL192_VECT_ADDR));
	__raw_writel(0, S5P_VIC0REG(VIC_INT_SELECT));
	__raw_writel((unsigned int)timer0_int, S5P_VIC0REG(VIC_VECT_ADDR0+54));
	tcon = __raw_readl(S5P_VIC0REG(VIC_INT_ENABLE));
	tcon |= (1<<21);	//Timer0 Int. Enable
	__raw_writel(tcon, S5P_VIC0REG(VIC_INT_ENABLE));
#endif

	tcon = __raw_readl(S3C64XX_TINT_CSTAT); 
	tcon |= (1<<0);		//Timer0 Int. Enable on Timer Controller
	__raw_writel(tcon, S3C64XX_TINT_CSTAT); 


	__raw_writel(timerClkVal, S3C2410_TCNTB(0)); 
	tcon = __raw_readl(S3C2410_TCON); 
	tcon |= (1<<1)|(0); 
	__raw_writel(tcon, S3C2410_TCON); 
	tcon = __raw_readl(S3C2410_TCON); 
	tcon &= ~(1<<1); 
	__raw_writel(tcon, S3C2410_TCON); 
	tcon = __raw_readl(S3C2410_TCON); 
	tcon |= 1;
	__raw_writel(tcon, S3C2410_TCON); 

}
void TIMER0_OFF(void)
{
	unsigned long tcon;
	tcon = __raw_readl(S3C2410_TCON); 
	tcon &= ~(1<<0);
	__raw_writel(tcon, S3C2410_TCON); 
}

irqreturn_t timer0_int(int irq, void *dev_id)
{
	unsigned long tcon;

	printk("[LED_DD]Timer0 Int(irq:%d)!!!!\n", irq);

	tcon = __raw_readl(S3C64XX_TINT_CSTAT); 
	tcon |= (1<<5);		//Timer0 Pending Clear on Timer Controller
	__raw_writel(tcon, S3C64XX_TINT_CSTAT); 
	
	TIMER0_OFF();
	flag_timer0Int=1;
	
#ifdef NO_WQ
#else
	................
#endif
	
	return IRQ_HANDLED;
}

#if 1
 void gpio_init(void){
	int i;

	//fpga data gpio init - gpm2_0~2
        for(i=0; i<3; i++){
                gpio_request(DATA_PIN_2(i), "GPM2_0");
                s3c_gpio_cfgpin(DATA_PIN_2(i), S3C_GPIO_SFN(1));
                gpio_direction_output(DATA_PIN_2(i), 1);
        }
        //fpga data gpio init - gpm0_0
	for(i=0; i<1; i++){
                gpio_request(DATA_PIN_0(i), "GPM0_0");
                s3c_gpio_cfgpin(DATA_PIN_0(i), S3C_GPIO_SFN(1));
                gpio_direction_output(DATA_PIN_0(i), 1);
        }
        //fpga data gpio init - gpm2_3~4
        for(i=3; i<5; i++){
                gpio_request(DATA_PIN_2(i), "GPM2_0");
                s3c_gpio_cfgpin(DATA_PIN_2(i), S3C_GPIO_SFN(1));
                gpio_direction_output(DATA_PIN_2(i), 1);
        }
        //fpga data gpio init - gpm3_0~7
        for(i=0; i<2; i++){
                gpio_request(DATA_PIN_3(i), "GPM3_0");
                s3c_gpio_cfgpin(DATA_PIN_3(i), S3C_GPIO_SFN(1));
                gpio_direction_output(DATA_PIN_3(i), 1);
        }
        //fpga address gpio init - gpm0_1~7 <= (0~6+1) //add 1
        for(i=0; i<7; i++){
                gpio_request(ADDR_PIN_L_0(i), "GPM0_0");
                s3c_gpio_cfgpin(ADDR_PIN_L_0(i), S3C_GPIO_SFN(1));
                gpio_direction_output(ADDR_PIN_L_0(i), 0);
                gpio_set_value(ADDR_PIN_L_0(i), 0);
        }
        //fpga address gpio init - gpm1_0~0
        for(i=0; i<1; i++){
                gpio_request(ADDR_PIN_L_1(i), "GPM0_0");
                s3c_gpio_cfgpin(ADDR_PIN_L_1(i), S3C_GPIO_SFN(1));
                gpio_direction_output(ADDR_PIN_L_1(i), 0);
                gpio_set_value(ADDR_PIN_L_1(i), 0);
        }
        //fpga address gpio init - gpm1_1~6
        for(i=0; i<6; i++){
                gpio_request(ADDR_PIN_H_1(i), "GPM1_0");
                s3c_gpio_cfgpin(ADDR_PIN_H_1(i), S3C_GPIO_SFN(1));
                gpio_direction_output(ADDR_PIN_H_1(i), 0);
                gpio_set_value(ADDR_PIN_H_1(i), 0);
        }
}
#endif

void LED_DATA(unsigned char val)
{
 	if(val & (1<<0))    gpio_set_value(DATA_PIN_2(0), 1);
        else                gpio_set_value(DATA_PIN_2(0), 0);
        if(val & (1<<1))    gpio_set_value(DATA_PIN_2(1), 1);
        else                gpio_set_value(DATA_PIN_2(1), 0);
        if(val & (1<<2))    gpio_set_value(DATA_PIN_2(2), 1);
        else                gpio_set_value(DATA_PIN_2(2), 0);

        if(val & (1<<3))    gpio_set_value(DATA_PIN_0(0), 1);
        else                gpio_set_value(DATA_PIN_0(0), 0);

        if(val & (1<<4))    gpio_set_value(DATA_PIN_2(3), 1);
        else                gpio_set_value(DATA_PIN_2(3), 0);
        if(val & (1<<5))    gpio_set_value(DATA_PIN_2(4), 1);
        else                gpio_set_value(DATA_PIN_2(4), 0);

        if(val & (1<<6))    gpio_set_value(DATA_PIN_3(0), 1);
        else                gpio_set_value(DATA_PIN_3(0), 0);
        if(val & (1<<7))    gpio_set_value(DATA_PIN_3(1), 1);
        else                gpio_set_value(DATA_PIN_3(1), 0);

}
static void write_addr_l(unsigned char val){
	int i;

        for(i=0; i<7; i++){
                if(val & (1<<i)){
                        gpio_set_value(ADDR_PIN_L_0(i), 1);
                }
                else{
                        gpio_set_value(ADDR_PIN_L_0(i), 0);
                }
        }
        if(val & (1 << 7))
                gpio_set_value(ADDR_PIN_L_1(0), 1);
        else
                gpio_set_value(ADDR_PIN_L_1(0), 0);

}

static long led_ioctl(struct file *file, unsigned int  cmd, unsigned long arg)
{
	printk("[DD]led_ioctl()..\n");
	return 0;
}

static int led_open(struct inode *minode, struct file *mfile)
{
	printk("[DD]led_open()..\n");
	return 0;
}

static int led_release(struct inode *minode, struct file *mfile)
{
	printk("[DD]led_release()..\n");
	gpio_init();
	return 0;
}

static ssize_t led_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	char data[4];
	int ret;
	int menu, led_no;
	
	printk("[DD]led_write()..\n");
	
	if(!flag_timeron) {
		printk("[DD]TIMER0_ON()..\n");
		flag_timeron = 1;
		TIMER0_ON();
	}
#ifdef NO_WQ
	while(!flag_timer0Int) {
		printk(KERN_WARNING "[LED_DD]Waiting Timer0 Int..\n");
		mdelay(1000);
	}
#else
	while(!flag_timer0Int) {
		if( ......... ) return -EAGAIN;
			
		printk(KERN_WARNING "[LED_DD]Waiting Timer0 Int..\n");
		//interruptible_sleep_on(&wq_timer);	//old version
		ret = ..............
		printk(KERN_WARNING "[LED_DD]After Timer0 Int(ret:%d)..\n", ret);
	}
#endif
	printk(KERN_WARNING "[LED_DD]After Timer0 Int..\n");

	write_addr_l(LED);

	memset(data, 0, 4);
	ret=copy_from_user(data, buf, count);
	if(ret < 0) {
		printk("copy_from_user() Error!!\n");
		return -1;
	}

	menu = data[1];
	if(data[0] == 8) led_no = 0xFF;
	else led_no = 1 << data[0];
	
	//printk("led_no(%x, %x)\n", data[0], led_no);	
	
	switch(menu) {
		case 1 : 	//LED ON
			LED_DATA(~led_no);
			break;
		case 2 :	//LED OFF
			LED_DATA(led_no);
			break;
	}

	_______ = 0;
	flag_timeron = 0;
	
	return count;
}

static struct file_operations led_fops = {
   .write      =   led_write,
   .open       =   led_open,
   .release    =   led_release,
   .unlocked_ioctl      =   led_ioctl,
};

static int __init led_init(void){
	int result;
	
	result = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &led_fops);
	if (result < 0) {
		printk("unable to register device %s\n", DEVICE_NAME);
		return result;
	} else DEVICE_MAJOR = result;
	printk("H-Smart4412TKU IEB: register device %s(major:%d)\n", DEVICE_NAME, DEVICE_MAJOR);

	gpio_init();	
	
	result = request_irq(IRQ_TIMER0_VIC, timer0_int, 0, "MyTimer", NULL);
	if(result != 0) {
	   printk(KERN_WARNING "request_irq(%d) Error(%d)!!\n", IRQ_TIMER0_VIC, result);
	}
	printk(KERN_WARNING "request_irq(%d) OK!!\n", IRQ_TIMER0_VIC);
	
	return 0;        

}

static void __exit led_exit(void){
	free_irq(IRQ_TIMER0_VIC, NULL);
	unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
	gpio_init();
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
