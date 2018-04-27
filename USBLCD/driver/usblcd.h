#ifndef USBLCD_H
#define USBLCD_H
#include <linux/ioctl.h>

#define USB_ENDPOINT_IN 0X80
#define USB_ENDPOINT_OUT 0X00
#define LCD_CLEAR_DISP           1  /*clear display*/
#define LCD_CURSOR_HOME          2   /*cursor home and unshift display*/
#define LCD_ENTRY_DEC            4   /* display shift off, dec cursor move dir */
#define LCD_ENTRY_DEC_SHIFT      5   /* display shift on,  dec cursor move dir */
#define LCD_ENTRY_INC_           6   /* display shift off, inc cursor move dir */
#define LCD_ENTRY_INC_SHIFT      7   /* display shift on,  inc cursor move dir */

/* display on/off, cursor on/off, blinking char at cursor position */
#define LCD_DISP_OFF             8   /* display off                            */
#define LCD_DISP_ON              12   /* display on, cursor off                 */
#define LCD_DISP_ON_BLINK        13   /* display on, cursor off, blink char     */
#define LCD_DISP_ON_CURSOR       14   /* display on, cursor on                  */
#define LCD_DISP_ON_CURSOR_BLINK 15   /* display on, cursor on, blink char      */

/* move cursor/shift display */
#define LCD_MOVE_CURSOR_LEFT     16   /* move cursor left  (decrement)          */
#define LCD_MOVE_CURSOR_RIGHT    20   /* move cursor right (increment)          */
#define LCD_MOVE_DISP_LEFT       24   /* shift display left                     */
#define LCD_MOVE_DISP_RIGHT      28   /* shift display right                    */

/* function set: set interface data length and number of display lines */
#define LCD_FUNCTION_4BIT_1LINE  32   /* 4-bit interface, single line, 5x7 dots */
#define LCD_FUNCTION_4BIT_2LINES 40   /* 4-bit interface, dual line,   5x7 dots */
#define LCD_FUNCTION_8BIT_1LINE  48   /* 8-bit interface, single line, 5x7 dots */
#define LCD_FUNCTION_8BIT_2LINES 56   /* 8-bit interface, dual line,   5x7 dots */


#define LCD_MODE_DEFAULT     ((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC) )
#endif
