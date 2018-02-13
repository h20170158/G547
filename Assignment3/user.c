#include "pit.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


 int main()
{
 args retval;
 int file_desc,ret_val;
 char  *DEVICE_NAME="/dev/Timer0";
 int msg;
 file_desc=open(DEVICE_NAME,0);
 if(file_desc<0)
 {
  printf("can't open device file : %s \n",DEVICE_NAME);
  exit(-1);
 }
 
 ret_val=read(file_desc,&retval,1);
printf("The current time reading is :%d",retval.msg);

return 0;
}
