#ifndef ADC_H
#define ADC_H

#include <linux/ioctl.h>
#define MAGIC_NUM 125
typedef struct
{
   int val1;

}args;

#define IOCTL_SET_MSG _IOR(MAGIC_NUM,0,char *)
#define IOCTL_GET_MSG _IOW(MAGIC_NUM,1,int)


#endif
