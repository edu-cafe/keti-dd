#include <linux/module.h>
//#include <linux/kernel.h>
//#include <linux/init.h>

#include <linux/fs.h>

#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/uaccess.h>

#include <linux/delay.h>		//msleep()

// Device Address
#define LED     0x0A
#define FND     0x0B
#define CLCD    0x0C

#define ADDR_PIN_L_0(x)           EXYNOS4212_GPM0(x+1)
#define ADDR_PIN_L_1(x)           EXYNOS4212_GPM1(x)

#define ADDR_PIN_H_1(x)           EXYNOS4212_GPM1(x+1)

#define DATA_PIN_2(x)             EXYNOS4212_GPM2(x)
#define DATA_PIN_3(x)             EXYNOS4212_GPM3(x)
#define DATA_PIN_4(x)             EXYNOS4212_GPM4(x)
#define DATA_PIN_0(x)             EXYNOS4212_GPM0(x)

#define SEL(x)           ADDR_PIN_H_1(x)
#define     MAX_FND  4

#define DEVICE_NAME 	"FND_MY"
int DEVICE_MAJOR = 0;

//0~F,0~5
unsigned char FND_DATA_TBL[]={
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,
	0x83,0xC6,0xA1,0x86,0x8E,0xC0,0xF9,0xA4,0xB0,0x99,0x92
};

static void gpio_init(void){
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
                gpio_request(DATA_PIN_2(i), "GPM2_3");
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
		gpio_request(ADDR_PIN_L_0(i), "GPM0_1");
	        s3c_gpio_cfgpin(ADDR_PIN_L_0(i), S3C_GPIO_SFN(1));
		gpio_direction_output(ADDR_PIN_L_0(i), 0);
		gpio_set_value(ADDR_PIN_L_0(i), 0);
	}
	//fpga address gpio init - gpm1_0~0
        for(i=0; i<1; i++){
                gpio_request(ADDR_PIN_L_1(i), "GPM1_0");
                s3c_gpio_cfgpin(ADDR_PIN_L_1(i), S3C_GPIO_SFN(1));
                gpio_direction_output(ADDR_PIN_L_1(i), 0);
                gpio_set_value(ADDR_PIN_L_1(i), 0);
        }
	//fpga address gpio init - gpm1_1~6
	for(i=0; i<6; i++){
		gpio_request(ADDR_PIN_H_1(i), "GPM1_1");
		s3c_gpio_cfgpin(ADDR_PIN_H_1(i), S3C_GPIO_SFN(1));
		gpio_direction_output(ADDR_PIN_H_1(i), 0);
		gpio_set_value(ADDR_PIN_H_1(i), 0);
	}

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
void FND_DATA(unsigned char val)
{

	if(val & (1<<0))    gpio_set_value(DATA_PIN_2(0), 1);
    	else         	    gpio_set_value(DATA_PIN_2(0), 0);
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
void FND_SEL(unsigned char val)
{
	int i;
	for(i=0; i<4; i++){
		if(val & (1<<i)){
		    	gpio_set_value(SEL(i), 1);
		}
		else{              
		    	gpio_set_value(SEL(i), 0);
		}
	}
}

void fnd_disp(int num, unsigned char val)
{
	write_addr_l(FND);
	switch(num)
	{
		case 1:
			FND_SEL(~0x01);
			FND_DATA(val);	
			break;
		case 2:
			FND_SEL(~0x02);
			FND_DATA(val);	
			break;
		case 3:
			FND_SEL(~0x04);
			FND_DATA(val);	
			break;
		case 4:
			FND_SEL(~0x08);
			FND_DATA(val);	
			break;
		case 5:
			FND_SEL(~0xF);
			FND_DATA(val);	
			break;
		default :   
			FND_SEL(~0x00);
			break;
	}
}
static void fnd_clear(void)
{
	fnd_disp(1, 0xff);
	fnd_disp(2, 0xff);
	fnd_disp(3, 0xff);
	fnd_disp(4, 0xff);
}

static long fnd_ioctl(struct file *file, unsigned int  cmd, unsigned long arg)
{
	printk("[DD]fnd_ioctl()...\n");
	return 0;
}

static int fnd_open(struct inode *minode, struct file *mfile)
{
	printk("[DD]fnd_open()...\n");		
	return 0;
}
static int fnd_release(struct inode *minode, struct file *mfile)
{
	printk("[DD]fnd_release()...\n");

	fnd_clear();
	return 0;
}

static ssize_t fnd_write (struct file *file, const char __user *buf, size_t count, loff_t *f_pos)
{
	int ret;
	int num;
	
	printk("[DD]fnd_write()...\n");

	ret = copy_from_user(&num, buf, count);
	if(ret < 0) {
		printk("copy_from_user() Error!!\n");
		return -1;
	}
	printk("[DD]fnd(%d, %x) write...\n", num, FND_DATA_TBL[num]);

	...............

	return count;
}

static ssize_t fnd_read (struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
	printk("[DD]fnd_read()...\n");
	return 0;
}

static struct file_operations fnd_fops = {
   .read       =   fnd_read,
   .write      =   fnd_write,
   .open       =   fnd_open,
   .release    =   fnd_release,
   .unlocked_ioctl      =   fnd_ioctl,
};
static int __init myfnd_init(void){
	int result;

	result = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &fnd_fops);
	if (result < 0) {
		printk("unable to register device %s\n", DEVICE_NAME);
		return result;
	} else DEVICE_MAJOR = result;
	printk("H-Smart4412TKU IEB: register device %s(major:%d)\n", DEVICE_NAME, DEVICE_MAJOR);

	gpio_init();
	return 0;        
}

static void __exit myfnd_exit(void){
	unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
	fnd_clear();
	gpio_init();
}
module_init(myfnd_init);
module_exit(myfnd_exit);

MODULE_LICENSE("GPL");
