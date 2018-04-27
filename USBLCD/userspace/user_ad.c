#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include <sys/ioctl.h>
#include"usblcd.h"
#include"ad.h"
#include <string.h>
#include <usb.h>
#define USBDEV_SHARED_VENDOR    0x16C0  
#define USBDEV_SHARED_PRODUCT   0x05DC 
usbGetStringAscii(usb_dev_handle *dev, int index, int langid, char *buf, int buflen)
{
	char    buffer[256];
	int     rval, i;
	/*Get the descriptor from the usb device*/
    if((rval = usb_control_msg(dev, USB_ENDPOINT_IN, USB_REQ_GET_DESCRIPTOR, (USB_DT_STRING << 8) + index, langid, buffer, sizeof(buffer), 1000)) < 0)
        return rval;
    if(buffer[1] != USB_DT_STRING)
        return 0;
    if((unsigned char)buffer[0] < rval)
        rval = (unsigned char)buffer[0];
    rval /= 2;
    
   for(i=1;i<rval;i++){
        if(i > buflen) 
            break;
        buf[i-1] = buffer[2 * i];
        if(buffer[2 * i + 1] != 0) 
            buf[i-1] = '?';
    }
    buf[i-1] = 0;
    return i-1;
}
#define USB_ERROR_NOTFOUND  1
#define USB_ERROR_ACCESS    2
#define USB_ERROR_IO        3
static int usbOpenDevice(usb_dev_handle **device, int vendor, char *vendorName, int product, char *productName)
{
/*Pointer to usb_bus structure*/
struct usb_bus      *bus;
/*Pointer to usb_device structure*/
struct usb_device   *dev;
usb_dev_handle      *handle = NULL;
int                 errorCode = USB_ERROR_NOTFOUND;
static int          didUsbInit = 0;

    if(!didUsbInit){
        didUsbInit = 1;
        usb_init();
    }
    usb_find_busses();
    usb_find_devices();
    for(bus=usb_get_busses(); bus; bus=bus->next){
        for(dev=bus->devices; dev; dev=dev->next){
            if(dev->descriptor.idVendor == vendor && dev->descriptor.idProduct == product){
                char    string[256];
                int     len;
                handle = usb_open(dev); 
                if(!handle){
                    errorCode = USB_ERROR_ACCESS;
		     fprintf(stderr, "Warning: cannot open USB device: %s\n", usb_strerror());
                    continue;
                }
                if(vendorName == NULL && productName == NULL){ 
                    break;
                }
                
                len = usbGetStringAscii(handle, dev->descriptor.iManufacturer,
0x0409, string, sizeof(string));
                if(len < 0){
                    errorCode = USB_ERROR_IO;
                    fprintf(stderr, "Warning: cannot query manufacturer for device: %s\n", usb_strerror());
                }else{
                    errorCode = USB_ERROR_NOTFOUND;
                    fprintf(stderr, "seen device from vendor ->%s<-\n", string);
                    if(strcmp(string, vendorName) == 0){
                        len = usbGetStringAscii(handle, dev->descriptor.iProduct, 0x0409, string, sizeof(string));
                        if(len < 0){
                            errorCode = USB_ERROR_IO;
fprintf(stderr, "Warning: cannot query product for device: %s\n", usb_strerror());
                        }else{
                            errorCode = USB_ERROR_NOTFOUND;
                            fprintf(stderr, "seen product ->%s<-\n", string); 
                            if(strcmp(string, productName) == 0)
                                break;
                        }
                    }
                }
                usb_close(handle);
                handle = NULL;
            }
  }
        if(handle)
            break;
    }
    if(handle != NULL){
        errorCode = 0;
 *device = handle;
    }
    return errorCode;
}



/*ioctl_set_msg sends the data from the userspace to the driver*/    
int ioctl_set_msg(int file,char msg[50])
{

	int rel;
	rel=ioctl(file,IOCTL_SET_MSG,msg);
	if(rel<0)
   	{
      		printf("IOCTL failed in kernel space \n");
      		exit(-1);
   	}
return 0;
   
}

int main()
{
	args q;
	int file;
	char msg[50];
	int rel;
	usb_dev_handle      *handle = NULL;
	unsigned char       buffer[8];
	int                 nBytes;
	int number;
	char *device="/dev/usblcd";
	/*Initialise usb device*/
	usb_init();
	/*Opens the usb device with the vendor name "www.obdev.at" and product name "usblcd"*/
    	if(usbOpenDevice(&handle, USBDEV_SHARED_VENDOR, "www.obdev.at", USBDEV_SHARED_PRODUCT, "usblcd") != 0){
        fprintf(stderr, "Could not find USB device \"USBasp\" with vid=0x%x pid=0x%x\n", USBDEV_SHARED_VENDOR, USBDEV_SHARED_PRODUCT);
        exit(1);
  	}
   	/*Opens the device*/
   	file=open(device,0);
   	if(file<0)
   	{
      	printf("File did not open from user space\n");
      	exit(-1);
   	}
   	printf("Enter the command or data\n");
   	scanf("%[^\n]%*c",msg);
	/*Send the command/data to the kernel space using ioctl_set_msg*/
	ioctl_set_msg(file,msg);
	/*Get the number to be passed as brequest value ,from the driver using 		read function*/
	rel=read(file,&q,1);
	number=q.val1;
	/*usb_control_msg sends the brequest value obtained from the driver 	along with the data to be passed*/
	nBytes = usb_control_msg(handle, USB_TYPE_VENDOR |   USB_RECIP_DEVICE | 	USB_ENDPOINT_OUT,number,0,0, (char *)msg, strlen(msg),0);
	/*Print the number of bytes transferred*/
	printf("The bytes txfrd is %d\n",nBytes);
	/*Print the brequest value that is transferred*/
	printf("The val is %d\n",number);
 return 0;
}
