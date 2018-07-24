#include <linux/module.h>
//#include <linux/kernel.h>
//#include <linux/init.h>

#include <linux/fs.h>

#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/uaccess.h>

#include ........

#define DEVICE_NAME "LED_MY"
int DEVICE_MAJOR = 0;

// Device Address
#define LED     0x0A

#define ADDR_PIN_L_0(x)           EXYNOS4212_GPM0(x+1)
#define ADDR_PIN_L_1(x)           EXYNOS4212_GPM1(x)

#define ADDR_PIN_H_1(x)           EXYNOS4212_GPM1(x+1)

#define DATA_PIN_2(x)             EXYNOS4212_GPM2(x)
#define DATA_PIN_3(x)             EXYNOS4212_GPM3(x)
#define DATA_PIN_4(x)             EXYNOS4212_GPM4(x)
#define DATA_PIN_0(x)             EXYNOS4212_GPM0(x)

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

static ssize_t led_write (struct file *file, const char *buf, size_t count, loff_t *f_pos)
{
	char data[4];
	int ret;
	int menu, led_no;
	
	printk("[DD]led_write()..\n");

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

	return 0;
}

static long led_ioctl(struct file *filp, unsigned int  cmd, unsigned long arg)
{
	int size;
	int err;

	printk(KERN_WARNING "[led_DD]ioctl(cmd:%x)..\n", cmd);

	if( ________ != LED_MAGIC) return -EINVAL;
	if( ________ > LED_MAXNR) return -EINVAL;

	size = _______(cmd);
	if(size) {
		err = 0;
		if( ______(cmd) & _IOC_READ) {
			//err = verify_area(VERIFY_WRITE, (void*)arg, size);
			err = _______(VERIFY_WRITE, (void*)arg, size);
		} else if(_IOC_DIR(cmd) & _IOC_WRITE) {
			//err = verify_area(VERIFY_READ, (void*)arg, size);
			err = access_ok(________, (void*)arg, size);
		}
		if(!err) return -1;
	}

	switch(cmd) {
	case LED_OFF :
		printk("[led_DD] ioctl => LED_OFF..\n");
		led_write(filp, (char*)arg, sizeof(int), NULL);
		break;
	case LED_ON :
		printk("[led_DD] ioctl => LED_ON..\n");
		led_write(filp, (char*)arg, sizeof(int), NULL);
		break;
	case LED_GETSTATE :
		printk("[led_DD] ioctl => LED_GETSTATE..\n");
		break;
	default :
		printk("[led_DD] ioctl => un-known command..\n");
	}
	return 0;
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

	return 0;        

}

static void __exit led_exit(void){
	unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
	gpio_init();
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
