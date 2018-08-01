
#include <math.h>
#include <avr/pgmspace.h>

#include "TVout.h"

void TVout::select_font(const unsigned char * f) 
{
	font = f;
}

/*
 * print an 8x8 char c at x,y
 * x must be a multiple of 8
 */
void TVout::print_char(uint8_t x, uint8_t y, unsigned char c) 
{
	c -= pgm_read_byte(font+2);
	bitmap(x,y,font,(c*pgm_read_byte(font+1))+3,pgm_read_byte(font),pgm_read_byte(font+1));
}

void TVout::inc_txtline() 
{
	if (cursor_y >= (display.vres - pgm_read_byte(font+1)))
		shift(pgm_read_byte(font+1),UP);
	else
		cursor_y += pgm_read_byte(font+1);
}

/* default implementation: may be overridden */
void TVout::write(const char *str)
{
	while (*str)
		write(*str++);
}

/* default implementation: may be overridden */
void TVout::write(const uint8_t *buffer, uint8_t size)
{
	while (size--)
		write(*buffer++);
}

void TVout::write(uint8_t c) 
{
	switch(c) 
	{
		case '\0':			//null
			break;
	
		case '\n':			//line feed
			cursor_x = 0;
			inc_txtline();
			break;
	
		case 8:				//backspace
			cursor_x -= pgm_read_byte(font);
			print_char(cursor_x,cursor_y,' ');
			break;
	
		case 13:			//carriage return !?!?!?!VT!?!??!?!
			cursor_x = 0;
			break;
	
		case 14:			//form feed new page(clear screen)
			//clear_screen();
			break;
	
		default:
			if (cursor_x >= (display.hres*8 - pgm_read_byte(font))) 
			{
				cursor_x = 0;
				inc_txtline();
				print_char(cursor_x,cursor_y,c);
			}
			else
				print_char(cursor_x,cursor_y,c);
	
			cursor_x += pgm_read_byte(font);
	}
}

void TVout::print(const char str[])
{
	write(str);
}

void TVout::print(int n, int base)
{
	if(base == 0)
	{
		write(n);
	}
	else if(base == 10)
	{
		if(n < 0)
		{
			print('-');
			n = -n;
		}
		printNumber(n, 10);
	}
	else
	{
		printNumber(n, base);
	}
}

void TVout::print(double n, int digits)
{
	printFloat(n, digits);
}

void TVout::printPGM(const char str[]) 
{
	char c;
	while ((c = pgm_read_byte(str))) 
	{
		str++;
		write(c);
	}
}

void TVout::printPGM(uint8_t x, uint8_t y, const char str[]) 
{
	char c;
	cursor_x = x;
	cursor_y = y;
	
	while ((c = pgm_read_byte(str))) 
	{
		str++;
		write(c);
	}
}

void TVout::set_cursor(uint8_t x, uint8_t y) 
{
	cursor_x = x;
	cursor_y = y;
}

void TVout::print(uint8_t x, uint8_t y, const char str[]) 
{
	cursor_x = x;
	cursor_y = y;
	write(str);
	
}

void TVout::print(uint8_t x, uint8_t y, int n, int base) 
{
	cursor_x = x;
	cursor_y = y;
	print(n, base);
}

void TVout::print(uint8_t x, uint8_t y, double n, int digits) 
{
	cursor_x = x;
	cursor_y = y;
	print(n,digits);
}

void TVout::printNumber(unsigned long n, uint8_t base)
{
	unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
	unsigned long i = 0;

	if (n == 0) 
	{
		print('0');
		return;
	} 

	while (n > 0) 
	{
		buf[i++] = n % base;
		n /= base;
	}

	for (; i > 0; i--)
		print((char) (buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
}

void TVout::printFloat(double number, uint8_t digits) 
{ 
	// Handle negative numbers
	if (number < 0.0)
	{
		print('-');
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	
	for (uint8_t i=0; i<digits; ++i)
		rounding /= 10.0;
  
	number += rounding;

	// Extract the integer part of the number and print it
	int int_part = (int)number;
	double remainder = number - (double)int_part;
	print(int_part);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0)
		print("."); 

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		int toPrint = int(remainder);
		print(toPrint);
		remainder -= toPrint; 
	}
}