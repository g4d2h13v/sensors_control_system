#include "auth.h"
#include "lcd_menu.h"
#include "mdr_delay.h"
#include "font.h"
#include "eeprom_passw.h"

void read_password_from_mem(void)
{
	uint32_t tmp[PASSWORD_LENGTH] = {0};
	read_password_from_flash(tmp, PASSWORD_LENGTH);
	
	if (is_password_combination(tmp))
	{
		for (uint8_t i = 0; i < PASSWORD_LENGTH; ++i)
		{
			correct_password[i] = (passw_t)tmp[i];
		}
	}
	/*
	for (uint8_t i = 0; i < PASSWORD_LENGTH; ++i)
	{
		if (is_password_symbol(tmp[i]))
		{
			correct_password[i] = (passw_t)tmp[i];
		}
		else
		{
			correct_password[i] = PASSW_DEFAULT;
		}
	}
	*/
}

auth_status_t try_login(const passw_t * user_password)
{
	if (user_password == 0)
		return AUTH_FAIL;
	
	uint8_t i;
	for (i = 0; i < PASSWORD_LENGTH; ++i)
	{
		if (user_password[i] != correct_password[i])
		{
			return AUTH_FAIL;
		}
	}
	return AUTH_SUCCESS;
}

auth_status_t authenticate(void)
{
	passw_t user_passw[PASSWORD_LENGTH] = {PASSW_DEFAULT};
	uint8_t i = 0;
	
	while (i < PASSWORD_LENGTH)
	{
		mdr_delay_ms(170);
		if (PORT_ReadInputDataBit(MDR_PORTB, PORT_Pin_5) == Bit_RESET)
		{
			user_passw[i++] = PASSW_UP;
			LcdPutChar(sym_arrow_up, UP_ARROW_X, UP_ARROW_Y, NOT_SMOOTH, INVERTED);
			mdr_delay_ms(170);
			LcdPutChar(sym_arrow_up, UP_ARROW_X, UP_ARROW_Y, NOT_SMOOTH, NOT_INVERTED);
		}
		else if (PORT_ReadInputDataBit(MDR_PORTB, PORT_Pin_6) == Bit_RESET)
		{
			user_passw[i++] = PASSW_RIGHT;
			LcdPutChar(sym_arrow_right, RIGHT_ARROW_X, RIGHT_ARROW_Y, NOT_SMOOTH, INVERTED);
			mdr_delay_ms(170);
			LcdPutChar(sym_arrow_right, RIGHT_ARROW_X, RIGHT_ARROW_Y, NOT_SMOOTH, NOT_INVERTED);
		}
		else if (PORT_ReadInputDataBit(MDR_PORTE, PORT_Pin_1) == Bit_RESET)
		{
			user_passw[i++] = PASSW_DOWN;
			LcdPutChar(sym_arrow_down, DOWN_ARROW_X, DOWN_ARROW_Y, NOT_SMOOTH, INVERTED);
			mdr_delay_ms(170);
			LcdPutChar(sym_arrow_down, DOWN_ARROW_X, DOWN_ARROW_Y, NOT_SMOOTH, NOT_INVERTED);
		}
		else if (PORT_ReadInputDataBit(MDR_PORTE, PORT_Pin_3) == Bit_RESET)
		{
			user_passw[i++] = PASSW_LEFT;
			LcdPutChar(sym_arrow_left, LEFT_ARROW_X, LEFT_ARROW_Y, NOT_SMOOTH, INVERTED);
			mdr_delay_ms(170);
			LcdPutChar(sym_arrow_left, LEFT_ARROW_X, LEFT_ARROW_Y, NOT_SMOOTH, NOT_INVERTED);
		}
	}
	return try_login(user_passw);
}

void reset_user_password(passw_t * user_password)
{
	uint8_t i;
	for (i = 0; i < PASSWORD_LENGTH; ++i)
	{
		user_password[i] = PASSW_DEFAULT;
	}
}

passw_state_t is_password_defined(void)
{
	return (correct_password[0] == PASSW_DEFAULT)? PASSWORD_UNDEFINED : PASSWORD_DEFINED;
}

static uint8_t is_password_combination(uint32_t * combination)
{
	for (uint8_t i = 0; i < PASSWORD_LENGTH; ++i)
	{
		if ((combination[i] > (PASSW_DIRECTIONS_COUNT - 1))
			||((passw_t)combination[i] == PASSW_DEFAULT)) // ?
		{
			return 0;
		}
	}
	return 1;
}

void input_new_password(void)
{
	passw_t user_passw[PASSWORD_LENGTH] = {PASSW_DEFAULT};
	uint8_t i = 0;
	
	while (i < PASSWORD_LENGTH)
	{
		mdr_delay_ms(170);
		if (PORT_ReadInputDataBit(MDR_PORTB, PORT_Pin_5) == Bit_RESET)
		{
			user_passw[i++] = PASSW_UP;
			LcdPutChar(sym_arrow_up, UP_ARROW_X, UP_ARROW_Y, NOT_SMOOTH, INVERTED);
			mdr_delay_ms(170);
			LcdPutChar(sym_arrow_up, UP_ARROW_X, UP_ARROW_Y, NOT_SMOOTH, NOT_INVERTED);
		}
		else if (PORT_ReadInputDataBit(MDR_PORTB, PORT_Pin_6) == Bit_RESET)
		{
			user_passw[i++] = PASSW_RIGHT;
			LcdPutChar(sym_arrow_right, RIGHT_ARROW_X, RIGHT_ARROW_Y, NOT_SMOOTH, INVERTED);
			mdr_delay_ms(170);
			LcdPutChar(sym_arrow_right, RIGHT_ARROW_X, RIGHT_ARROW_Y, NOT_SMOOTH, NOT_INVERTED);
		}
		else if (PORT_ReadInputDataBit(MDR_PORTE, PORT_Pin_1) == Bit_RESET)
		{
			user_passw[i++] = PASSW_DOWN;
			LcdPutChar(sym_arrow_down, DOWN_ARROW_X, DOWN_ARROW_Y, NOT_SMOOTH, INVERTED);
			mdr_delay_ms(170);
			LcdPutChar(sym_arrow_down, DOWN_ARROW_X, DOWN_ARROW_Y, NOT_SMOOTH, NOT_INVERTED);
		}
		else if (PORT_ReadInputDataBit(MDR_PORTE, PORT_Pin_3) == Bit_RESET)
		{
			user_passw[i++] = PASSW_LEFT;
			LcdPutChar(sym_arrow_left, LEFT_ARROW_X, LEFT_ARROW_Y, NOT_SMOOTH, INVERTED);
			mdr_delay_ms(170);
			LcdPutChar(sym_arrow_left, LEFT_ARROW_X, LEFT_ARROW_Y, NOT_SMOOTH, NOT_INVERTED);
		}
	}
	change_password(user_passw);
}

void change_password(const passw_t * user_password)
{
	uint32_t tmp[PASSWORD_LENGTH] = {0};
	for (uint8_t i = 0; i < PASSWORD_LENGTH; ++i)
	{
		tmp[i] = (uint32_t)user_password[i];
	}
	write_password_to_flash(tmp, PASSWORD_LENGTH);
}

static uint8_t is_password_symbol(uint32_t symbol)
{
	return (symbol < (PASSW_DIRECTIONS_COUNT - 1))? 1 : 0;
}