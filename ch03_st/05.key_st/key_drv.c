#include <linux/module.h>
//#include <linux/kernel.h>
//#include <linux/init.h>

#include <linux/fs.h>

#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "KEY_MY"
int DEVICE_MAJOR = 0;

// Device Address
#define KEY	0x0F

#define ADDR_PIN_L_0(x)           EXYNOS4212_GPM0(x+1)
#define ADDR_PIN_L_1(x)           EXYNOS4212_GPM1(x)

#define ADDR_PIN_H_1(x)           EXYNOS4212_GPM1(x+1)

#define DATA_PIN_2(x)             EXYNOS4212_GPM2(x)
#define DATA_PIN_3(x)             EXYNOS4212_GPM3(x)
#define DATA_PIN_4(x)             EXYNOS4212_GPM4(x)
#define DATA_PIN_0(x)             EXYNOS4212_GPM0(x)

unsigned int sw_data = 0x00;

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

static int KEY_DATA(void)
{
	int i, sw_data=0;
	
	for(i=0; i<3; i++){
                sw_data |= ((gpio_get_value(DATA_PIN_2(i)))<<i);
        }
                sw_data |= ((______________(DATA_PIN_0(0)))<<3);
                sw_data |= ((gpio_get_value(DATA_PIN_2(3)))<<4);
                sw_data |= ((gpio_get_value(DATA_PIN_2(4)))<<5);
                sw_data |= ((gpio_get_value(DATA_PIN_3(0)))<<6);
                sw_data |= ((gpio_get_value(DATA_PIN_3(1)))<<7);
	
	return sw_data;
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

static long key_ioctl(struct file *file, unsigned int  cmd, unsigned long arg)
{
	printk("[DD]key_ioctl()...\n");
	return 0;
}

static int key_open(struct inode *minode, struct file *mfile)
{
	printk("[DD]key_open()...\n");
	return 0;
}

static ssize_t key_read (struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
	int ret, sw_data=0;
	
	printk("[DD]key_read()...\n");

	.............
	sw_data = .........
	printk("[DD]sw_data:%x\n", sw_data);

	ret = copy_to_user(buf, &sw_data, sizeof(sw_data));
	if(ret < 0) {
		printk("copy_to_user() Error!!\n");
		return -1;
	}

	return count;

}

static int key_release(struct inode *minode, struct file *mfile)
{
	printk("[DD]key_release()...\n");
	return 0;
}

static ssize_t key_write (struct file *file, const char *buf, size_t count, loff_t *f_pos)
{
	printk("[DD]key_write()...\n");
	return 0;
}

static struct file_operations key_fops = {
   .write      =   key_write,
   .read	   =   key_read,
   .open       =   key_open,
   .release    =   key_release,
   .unlocked_ioctl      =   key_ioctl,
};

static int __init key_init(void){
	int result;

	result = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &key_fops);
	if (result < 0) {
		printk("unable to register device %s\n", DEVICE_NAME);
		return result;
	} else DEVICE_MAJOR = result;
	printk("H-Smart4412TKU IEB: register device %s(major:%d)\n", DEVICE_NAME, DEVICE_MAJOR);

	gpio_init();

	return 0;        

}

static void __exit key_exit(void){
	unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
	gpio_init();
}

module_init(key_init);
module_exit(key_exit);

MODULE_LICENSE("GPL");
