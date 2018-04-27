                                                  USB to Alpha numeric LCD
     
     SUMMARY:
          
            This project is aimed at sending data from kernel space to the alpha numeric lcd via usb.USB device has atmega 8 IC with 
            firmware loaded into it.The device receives the data via usb port and displays data on lcd.The usb core acts as an 
            intermediate in this data transfer.The command /data entered on the user space is sent to kernel space via ioctl call.The 
            kernel drivers receives the command/data.If command is entered,kernel driver compares the command entered  with predefined 
            commands.Based on command/data received,driver sends a unique number to the user.The user space sends this number in the 
            brequest field of the usb_control_msg.The command or data entered in the user space is sent as data pointer in
            the buffer field of usb_control_msg. The firmware checks the brequest field, and decides the commands to be executed.If 
            brequest=60,then data is obtained from the data pointer and displayed on lcd.
     PREREQUISITES:
            -avrdude
            -libusb
     FIRMWARE BUILD PROCESS:
            make
     LOADING FIRMWARE.HEX
            avrdude -c usbasp -p m8 -U flash:w:main.hex
     KERNEL DRIVER BUILD PROCESS
            -make
            -sudo insmod kernel_ad.ko
            -sudo chmod 777 /dev/usblcd
     USER SPACE BUILD PROCESS
            -gcc user_ad.c -o user `libusb-config--libs`
            -./user
            
             
            
         
