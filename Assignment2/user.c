#include "ADC.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>


 int main()
{
 args retval;
 int file_desc,ret_val;
 char  *DEVICE_NAME="/dev/ADC0";
 int msg;
 file_desc=open(DEVICE_NAME,0);
 if(file_desc<0)
 {
  printf("can't open device file : %s \n",DEVICE_NAME);
  exit(-1);
 }
 printf("Enter the channel no:");
 scanf("%d",&msg);
 retval.msg1=msg;
if(ioctl(file_desc,IOCTL_GET_MSG,&retval)<0)
 {
 printf("IOCTL failed");
 return -1;
 }
 printf("ADC reading from channel %d :%d",msg,retval.msg2);

 close(file_desc);
 return 0;
}
