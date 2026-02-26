/*Hide characters*/
HD44780_NoDisplay();
HD44780_Cursor();
HD44780_SetCursor(0,0);
HD44780_PrintStr("HELLO STM32!!!");
HD44780_PrintSpecialChar(0);

/*Show characters*/
HD44780_Display();

/*Move position*/
HD44780_SetCursor(0,1);
HD44780_PrintStr("BYE STM32!!!");
HD44780_PrintSpecialChar(1);

/*Blink cursor*/
HD44780_Blink();

/*Scroll Right*/
for(int i=0; i<3; i++)
{
    HD44780_ScrollDisplayRight();
    HAL_Delay(1000);
}

/*Scroll Left*/
for(int i=0; i<3; i++)
{
    HD44780_ScrollDisplayLeft();
    HAL_Delay(1000);
}