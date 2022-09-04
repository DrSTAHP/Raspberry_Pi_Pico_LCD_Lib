/*Treat C++ as C*/
/*Opening*/
#ifdef __cplusplus
extern "C" {
#endif

#include "liblcd.h"
#include <string.h>

static uint16_t LCD_GetBits(LCD_t* pLCD)
{
    if(pLCD == NULL)
        return 0;
    uint16_t iReturn = 0;

    if(gpio_get((uint)pLCD->RS_PIN))
        iReturn |= LCD_RS;
    if(gpio_get((uint)pLCD->RW_PIN))
        iReturn |= LCD_RW;
    

    uint8_t k = 0;
    for(uint8_t i = sizeof(pLCD->DATA_PINS) - 1; i >= 0; i--)
    {
        if(k > sizeof(pLCD->DATA_PINS) - 1)
            break;

        if(gpio_get((uint)pLCD->DATA_PINS[k]))
        {
            iReturn |= 1 << i;
            k++;
        }
    }
    return iReturn;
}

static void LCD_SetBits(LCD_t* pLCD, uint16_t bits)
{
    if(pLCD == NULL)
        return;
    
    uint8_t k = 0;
    for(uint8_t i = (sizeof(uint16_t) * 8) - 7; i >= 0; i--)
    {
        if(i < 8)
        {
            if(k > sizeof(pLCD->DATA_PINS) - 1)
                break;

            gpio_put((uint)pLCD->DATA_PINS[k], (bool)((bits >> i) & 1));
            k++;
        }
        else if(i == 8)
            gpio_put((uint)pLCD->RW_PIN, (bool)((bits >> i) & 1));
        else
            gpio_put((uint)pLCD->RS_PIN, (bool)((bits >> i) & 1));
    }

    /* Clock signal */
    gpio_put((uint)pLCD->E_PIN, true);
    sleep_us(1);
    gpio_put((uint)pLCD->E_PIN, false);

    /* Make sure we don't hit the busy period. */
    sleep_us(50);
}

void lcd_init(LCD_t* ret, uint8_t columns, uint8_t rows, uint8_t* data_pins, uint8_t rs_pin, uint8_t rw_pin, uint8_t enable_pin)
{
    if(ret == NULL)
        return;
    memcpy(ret->DATA_PINS, data_pins, sizeof(ret->DATA_PINS));

    ret->ROWS = rows;
    ret->COLUMNS = columns;
    ret->RS_PIN = rs_pin;
    ret->RW_PIN = rw_pin;
    ret->E_PIN = enable_pin;
    
    uint16_t newBits;
    
    /* Set the entry mode */
    newBits = 0;
    newBits |= LCD_ENTRY_MODE_SET | 1 << 1; //Move cursor to the right
    LCD_SetBits(ret, newBits);

    /* Turn on the display */
    newBits = 0;
    newBits |= LCD_DISPLAY_ON_OFF | 1 << 2; //Turn it on 
    LCD_SetBits(ret, newBits);
    
    /* Set the functionality */
    newBits = 0;
    newBits |= LCD_FUNCTION_SET | 1 << 4 | 1 << 3; //5x8 dot display ; 8 bit mode ; 2-line mode
    LCD_SetBits(ret, newBits);

    ret->ONELINE = false;
    ret->BIGFONT = false;
}

void lcd_clear(LCD_t* pLCD)
{
    if(pLCD == NULL)
        return;

    uint16_t LCD_newBits = 0;
    LCD_newBits |= LCD_CLEAR_DISPLAY;
    
    LCD_SetBits(pLCD, LCD_newBits);
}

void lcd_print(LCD_t* pLCD, const unsigned char* pText)
{
    if(pLCD == NULL || pText == NULL)
        return;

    uint16_t newBits;
    int iTextLen = strlen(pText);
    for(int i = 0; i < iTextLen; i++)
    {
        newBits = 0;
        newBits |= LCD_RS | (unsigned char)pText[i];
        LCD_SetBits(pLCD, newBits);

        sleep_ms(2); //Wait 2ms for the next instruction.
    }
}

void lcd_setcursor(LCD_t* pLCD, uint8_t column, uint8_t row)
{
    if(pLCD == NULL || column >= pLCD->COLUMNS || row >= pLCD->ROWS)
        return;

    uint16_t bits = 0;
    bits |= LCD_SET_DDRAM_ADDRESS;

    if(!row)
        bits |= 0x0 + column;
    else
        bits |= 0x40 + column;

    LCD_SetBits(pLCD, bits);
}

void lcd_oneline(LCD_t* pLCD, bool bOneLine)
{
    if(pLCD == NULL)
        return;
    uint16_t bits = 0;
    bits |= LCD_FUNCTION_SET | 1 << 4;

    if(!bOneLine)
        bits |= 1 << 3;
    if(pLCD->BIGFONT)
        bits |= 1 << 2;

    LCD_SetBits(pLCD, bits);
}

void lcd_bigfont(LCD_t* pLCD, bool bBigFont)
{
    if(pLCD == NULL)
        return;
    uint16_t bits = 0;
    bits |= LCD_FUNCTION_SET | 1 << 4;

    if(bBigFont)
        bits |= 1 << 2;
    if(!pLCD->ONELINE)
        bits |= 1 << 3;

    LCD_SetBits(pLCD, bits);
}


/*Closing*/
#ifdef __cplusplus
}
#endif