
#include<linux/types.h>
#include<asm/io.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/interrupt.h>
#include<linux/delay.h>
#include<linux/kernel.h>
#include<linux/version.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/irq.h>
#include "pit.h"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Priyanka");
MODULE_DESCRIPTION("Programmable TIMer");
#define MAJOR_NO 100
#define MINOR_NO 3
static dev_t devno;
static struct class *class_cr;
static struct cdev c_dev;
#define dev_name "Timer0"
#define dev_class "PITCLASS"
#define dev "PIT class"
#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_CMDREG   0x43
int ticks=0;
int count,divisor;
int irq=-1;
int numberOpens=0;
int readtime(void);
int dev_open(struct inode *inodep, struct file *filep) 
{
  numberOpens++;
  printk(KERN_INFO "Timer0 Device has been opened %d time \n",numberOpens);
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
  return 0;
}
int readtime(void)
{
outb(0x00,PIT_CMDREG );
count =inb(PIT_CHANNEL0);
return count;
}
ssize_t dev_read(struct file *file,char __user *buffer,size_t length,loff_t *offset)
{args value;
int val;
printk(KERN_INFO "device read\n");
val=readtime();
printk(KERN_INFO "count is :%d\n",count);
value.msg=val;
if(raw_copy_to_user(buffer,&value,sizeof(args)))
  return -1;
return 0;

}
ssize_t dev_write(struct file *file,const char __user *buffer,size_t length,loff_t *offset)
{
printk(KERN_INFO "device write\n");
return 0;
}

int timer_set(int hz)
{
 divisor=1193180/hz;
outb_p(0x60,PIT_CMDREG);
outb_p(divisor,PIT_CHANNEL0);
outb((divisor>>8)&0xFF,PIT_CHANNEL0);
return 0;
}



static struct file_operations fops =
{  .owner=THIS_MODULE,
   .open=dev_open,
   .read= dev_read,
   .write= dev_write,
   .release = dev_release,
   
   
};
int  PIT_init (void) 
{
int setret,setretval,i;
 printk(KERN_INFO "MODULE CREATED\n");
devno=MKDEV(MAJOR_NO,MINOR_NO);
if((class_cr=class_create(THIS_MODULE,dev_class))==NULL)
{class_destroy(class_cr);
unregister_chrdev_region(devno,1);
return -1;
}
if(device_create(class_cr,NULL,devno,NULL,"Timer0")==NULL)
{class_destroy(class_cr);
unregister_chrdev_region(devno,1);
return -1;
}
cdev_init(&c_dev,&fops);
c_dev.ops=&fops;
c_dev.owner=THIS_MODULE;
if(cdev_add(&c_dev,devno,1)==-1)
{
device_destroy(class_cr,devno);
class_destroy(class_cr);
unregister_chrdev_region(devno,1);
return -1;
}
setretval=timer_set(1000);

for( i=0;i<divisor;i++);    //for a certain time delay do nothing
printk("kernel is sleeping \n");//Then print this statement
return 0;

}
void PIT_exit(void)
{ cdev_del(&c_dev);
device_destroy(class_cr,devno);
class_destroy(class_cr);
unregister_chrdev_region(devno,1);
  printk(KERN_INFO "MODULE EXITED\n");
}
module_init(PIT_init);
module_exit(PIT_exit);
