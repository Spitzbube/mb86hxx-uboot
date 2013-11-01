/*
 * (C) Copyright 2008
 * Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/arch/mb86hxx.h>


#define iowrite32(val, addr) (*(volatile unsigned int *)(addr) = (val))
#define ioread32(addr) (*(volatile unsigned int *)(addr))


int board_init(void)
{
	//gpio configuration
	iowrite32(2+32, MB86HXX_GPIO_BASE+0x200+4*21); //UART0_RXDATA = 21 => GPIO 32 (in)
	iowrite32(23, MB86HXX_GPIO_BASE+4*33); //UART0_TXDATA = 23 => GPIO 33 (out)

	return 0;
}

int dram_init(void)
{

	return 0;
}

