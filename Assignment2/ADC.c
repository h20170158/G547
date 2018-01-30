#include<linux/init.h>
#include<linux/module.h>
#include<linux/device.h>
#include<linux/version.h>
#include<linux/types.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/unistd.h>
#include<linux/random.h>
#include<asm/uaccess.h>
#include<linux/ioctl.h>
#include<asm/ioctls.h>
#include "ADC.h"
#define DEVICE_NAME "ADC0"
#define SUCCESS 0
#define BUF_LEN 80
#define MAJOR_NUM 100
static int majorNumber;
static int num;
static int numberOpens=0;
static dev_t devno;
int genrandom(void);


 int dev_open(struct inode *inodep, struct file *filep) 
{
  numberOpens++;
  printk(KERN_INFO "ADC0 Device has been opened %d time \n",numberOpens);
  try_module_get(THIS_MODULE);
  return 0;
}
 int dev_release(struct inode *inode,struct file *file)
{ 
  #ifdef DEBUG
  printk(KERN_INFO "device released");
  #endif
  numberOpens--;
  module_put(THIS_MODULE);
  return SUCCESS;
}
ssize_t dev_read(struct file *file,char __user *buffer,size_t length,loff_t *offset)
{
  args val;
  raw_copy_from_user(&val,buffer,sizeof(buffer));
 switch(val.msg1)
{
case 1:
printk(KERN_INFO "The ADC Channel is channel1  and reading ");
break;
case 2:
printk(KERN_INFO "The ADC Channel is channel2  and reading ");
break;
case 3:
printk(KERN_INFO "The ADC Channel is channel3  and reading ");
break;
case 4:
printk(KERN_INFO "The ADC Channel is channel4  and reading ");
break;
case 5:
printk(KERN_INFO "The ADC Channel is channel5  and reading ");
break;
case 6:
printk(KERN_INFO "The ADC Channel is channel6  and reading ");
break;
case 7:
printk(KERN_INFO "The ADC Channel is channel7 and reading ");
break;
case 8:
printk(KERN_INFO "The ADC Channel is channel8 and reading ");
break;
default:
printk(KERN_INFO "this channel no doesnt exist\n");
return -1;
}
}


 ssize_t dev_write(struct file *file,const char __user *buffer,size_t length,loff_t *offset)
{
  args val;
  int i=genrandom();
  val.msg2=i;
  printk(KERN_INFO "Reading of ADC is :%d\n ",i);
  if(raw_copy_to_user(buffer,&val,sizeof(args)))
  return -1;
  return i;
}
int genrandom(void)
{
  int random;
  get_random_bytes(&random,2);
  random=random%1023;
  return random;
}
long dev_ioctl(struct file *file,unsigned int ioctl_num,unsigned long ioctl_param)
{
  args retval;
 
switch(ioctl_num)
{
case IOCTL_SET_MSG:
	dev_write(file,(char *)ioctl_param,0,0);
	break;
case IOCTL_GET_MSG:
         dev_read(file,(char *)ioctl_param,0,0);
	break;
}
return SUCCESS;
}
static struct file_operations fops =
{  .owner=THIS_MODULE,
   .open=dev_open,
   .read= dev_read,
   .write= dev_write,
   .release = dev_release,
   .unlocked_ioctl=dev_ioctl
   
};

int  ADC0_init (void) 
{
 printk(KERN_INFO "Initializing ADC\n");
 majorNumber = register_chrdev(MAJOR_NUM, DEVICE_NAME , &fops);
 devno=MKDEV(MAJOR_NUM,0);

if (majorNumber<0)
   {
     printk(KERN_ALERT "ADC failed to register a major number\n");
     return majorNumber;
    }
 printk(KERN_INFO "ADC registered correctly with major number %d\n",MAJOR_NUM);
 printk(KERN_INFO "device allocated \n");
 return 0;
}

 void ADC0_exit(void)
{
 
 unregister_chrdev(MAJOR_NUM , DEVICE_NAME );
 printk(KERN_INFO "ADC bye\n");
}

module_init(ADC0_init);
module_exit(ADC0_exit);
