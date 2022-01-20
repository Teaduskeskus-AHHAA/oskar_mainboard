#include <compat/deprecated.h>

uint8_t DEBUGINV = 0;
void doSPI(uint8_t data)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		cbi(PORTB, 5);
		if (data & 0x1)
			sbi(PORTB, 3);
		else
			cbi(PORTB, 3);
		// delay(10);
		sbi(PORTB, 5);
		// delay(10);
		data >>= 1;
	}
}

void SendData(uint8_t d3, uint8_t d2, uint8_t d1, uint8_t d0)
{
	DDRB |= 0x38;
	DEBUGINV ^= 0xFF;
	d0 ^= DEBUGINV;
	d1 ^= DEBUGINV;
	d2 ^= DEBUGINV;
	d3 ^= DEBUGINV;
	cbi(PORTB, 4);
	doSPI((d2 & 0xF0) | (d3 >> 4));
	doSPI((d0 & 0x70) | (d1 >> 4) | (d0 & 1) << 7);
	doSPI((d1 & 0xF) << 3 | (d0 & 0xF) >> 1 | (DEBUGINV & 0x80));
	doSPI((d3 << 5 & 0x60) | (d3 << 4 & 0x80) | (d3 << 2 & 0x10) | (d2 & 0xF));
	sbi(PORTB, 4);
}

const uint8_t numbers[10] = { 0x77, 0x41, 0x3B, 0x6B, 0x4D, 0x6E, 0x7E, 0x43, 0x7F, 0x6F };

void WriteNum(int32_t num, uint8_t decimal)
{
	uint8_t i, buf[4];
	uint8_t sgn = 0;
	if (num < 0)
	{
		num = -num;
		sgn = 1;
	}
	for (i = 0; i < 4; i++)
	{
		if (num || i <= decimal)
			buf[i] = numbers[num % 10];
		else if (sgn)
		{
			buf[i] = 8;
			sgn = 0;
		}
		else
			buf[i] = 0;
		num /= 10;
	}
	if (decimal)
		buf[decimal] |= 0x80;
	SendData(buf[3], buf[2], buf[1], buf[0]);
	// SendData(0,0,0,0);
}
