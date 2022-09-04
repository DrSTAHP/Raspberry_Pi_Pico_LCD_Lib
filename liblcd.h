#ifndef LCDLIB_H
#define LCDLIB_H

/*Treat C++ as C*/
/*Opening*/
#ifdef __cplusplus
extern "C" {
#endif

#include "pico/stdlib.h"
typedef struct LCD_t
{
    uint8_t COLUMNS;
    uint8_t ROWS;

    uint8_t DATA_PINS[8];
    uint8_t RS_PIN; 
    uint8_t RW_PIN;
    uint8_t E_PIN;

    bool ONELINE;
    bool BIGFONT;
}LCD_t;

/*Instruction list according to https://www.openhacks.com/uploadsproductos/eone-1602a1.pdf */
#define LCD_CLEAR_DISPLAY 1 << 0
#define LCD_RETURN_HOME 1 << 1
#define LCD_ENTRY_MODE_SET 1 << 2
#define LCD_DISPLAY_ON_OFF 1 << 3 
#define LCD_CURSOR_OR_BLINK_SHIFT 1 << 4
#define LCD_FUNCTION_SET 1 << 5
#define LCD_SET_CGRAM_ADDRESS 1 << 6
#define LCD_SET_DDRAM_ADDRESS 1 << 7
#define LCD_RW 1 << 8
#define LCD_RS 1 << 9
/*----------------------------------------------------------------------------------------*/
/* !!! Note that data_pin array should go D7 - D0 !!! */
void lcd_init(LCD_t* ret, uint8_t columns, uint8_t rows, uint8_t* data_pins, uint8_t rs_pin, uint8_t rw_pin, uint8_t enable_pin);

void lcd_clear(LCD_t* pLCD);

void lcd_print(LCD_t* pLCD, const unsigned char* pText);
/* !!! Note that columns and rows start with 0 !!! */
void lcd_setcursor(LCD_t* pLCD, uint8_t column, uint8_t row);

void lcd_oneline(LCD_t* pLCD, bool bOneLine);

void lcd_bigfont(LCD_t* pLCD, bool bBigFont);

/*Closing*/
#ifdef __cplusplus
}
#endif

#endif 