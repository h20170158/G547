#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include<string.h>
#include "lcd.h"
#include "usbdrv.h"
#include "oddebug.h"
#include <util/delay.h>
static uchar buffer[64];
static uchar currentpos,bytesrem;
USB_PUBLIC usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;
	static uchar    replyBuf[2];
	usbMsgPtr = replyBuf;
	/*Get the brequest field sent from the driver and process the request based on its values*/
	if(rq->bRequest==1){  
        lcd_command( LCD_CLEAR_DISP);/*Clear lcd display*/
        return 2;
    }
        else if( rq->bRequest== 2){  
        lcd_command( LCD_CURSOR_HOME);/*Move cursor to home position*/
        return 2;
    }
        else if( rq->bRequest==4){  
        lcd_command(LCD_ENTRY_DEC);
        return 2;
    }
	else if( rq->bRequest==5){  
        lcd_command(LCD_ENTRY_DEC_SHIFT);
        return 2;
    }
	else if(rq->bRequest==6){  
        lcd_command(LCD_ENTRY_INC_);
        return 2;
    }
	else if(rq->bRequest== 7){  
        lcd_command(LCD_ENTRY_INC_SHIFT);
        return 2;
    }
	else if(rq->bRequest==8){  
        lcd_command( LCD_DISP_OFF);/*lcd display is turned off*/
        return 2;
    }
	else if(rq->bRequest==12){ 
        lcd_command( LCD_DISP_ON);/*lcd display is turned on*/
        return 2;
    }
	else if(rq->bRequest== 13){  
        lcd_command( LCD_DISP_ON_BLINK);/*lcd displays with blink*/
        return 2;
    }
	else if(rq->bRequest==14){  
        lcd_command( LCD_DISP_ON_CURSOR);/*lcd displays the cursor*/
        return 2;
    }
	else if(rq->bRequest== 15 ){ 
        lcd_command( LCD_DISP_ON_CURSOR_BLINK);/*lcd displays cursor with blink*/
        return 2;
    }
	else if(rq->bRequest== 16){  
        lcd_command( LCD_MOVE_CURSOR_LEFT);/*Cursor is left shifted by one position*/
        return 2;
    }
	else if(rq->bRequest== 20){  
        lcd_command( LCD_MOVE_CURSOR_RIGHT);/*Cursor is right shifted by one position*/
        return 2;
    }
	else if(rq->bRequest== 24){ 
        lcd_command( LCD_MOVE_DISP_LEFT );/*lcd display is shifted left*/
        return 2;
    }
	else if(rq->bRequest== 28){  
        lcd_command( LCD_MOVE_DISP_RIGHT );/*lcd display is shifted right*/
        return 2;
    }
	else if(rq->bRequest== 32){  
        lcd_command( LCD_FUNCTION_4BIT_1LINE );/*lcd displays 4 bits in a single line*/
        return 2;
    }
	else if(rq->bRequest== 40){  
        lcd_command( LCD_FUNCTION_4BIT_2LINES );/*lcd displays 4 bits in 2 lines*/
        return 2;
    }
	else if(rq->bRequest== 48){ 
        lcd_command( LCD_FUNCTION_8BIT_1LINE );/*lcd displays 8 bits in 1 line*/
        return 2;
    }
	else if(rq->bRequest==56){  
        lcd_command( LCD_FUNCTION_8BIT_2LINES );/*lcd displays 8 bits in 2 lines*/
        return 2;
    }
	
  else  if (rq->bRequest==60){ 
	currentpos=0;
	bytesrem=rq->wLength.word;
	if(bytesrem>sizeof(buffer))
	{
	bytesrem=sizeof(buffer);
	}
	return USB_NO_MSG;/*Enters in to usbFunctionWrite function*/
	}
	return 0;
}
USB_PUBLIC uchar usbFunctionWrite(uchar *data,uchar len)
{
	uchar i;
	bytesrem -=len;
	for(i=0;i<len;i++)
	{buffer[currentpos++]=data[i];/*Check if the data sent from the driver is complete*/
	}
	lcd_puts((char *) buffer);/*Puts the data on to the lcd*/
	return bytesrem==0;
}
int main(void)
{
    uchar   i;
    uchar controller=0;
    wdt_enable(WDTO_1S);/*Enable watch dog timer*/
    odDebugInit();
    DDRB = ~(_BV(6)|_BV(7));/*PB6 and PB7 are not output pins.Hence it is disabled*/
    DDRC=~(_BV(6));/*PC6 is used for switch .Hence it is disabled as an output pin*/
    PORTC=0xBF;
	  DDRD = ~USBMASK;    /* set all pins as outputs except USB */
    PORTD = 0;
	  lcd_init(LCD_DISP_ON);/*Initialise lcd and turn on the display*/
	  lcd_puts("USBLCD");/*Displays "USBLCD" for 500 milliseconds*/
    _delay_ms(500);
	  lcd_command(LCD_CLEAR_DISP);/*Display is cleared after 500 milli seconds*/
	  usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    i = 0;
    while(--i){         
        wdt_reset();
        _delay_ms(2);
    }
    
    usbDeviceConnect();/*Connect to usb device*/
	  usbInit();/*Initialise usb device*/
    sei();
    for(;;){    /* main event loop */
        wdt_reset();
        usbPoll(); }
    return 0;
}


