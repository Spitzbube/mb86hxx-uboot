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
#include <asm/io.h>
#include <asm/arch/mb86hxx.h>


flash_info_t	flash_info[CONFIG_SYS_MAX_FLASH_BANKS]; /* info for FLASH chips */


int board_init(void)
{
	//gpio configuration
	writel(2+32, MB86HXX_GPIO_BASE+0x200+4*21); //UART0_RXDATA = 21 => GPIO 32 (in)
	writel(23, MB86HXX_GPIO_BASE+4*33); //UART0_TXDATA = 23 => GPIO 33 (out)

	//flash configuration
    writel(2+43,  MB86HXX_GPIO_BASE+0x200+4*2); //SFLASH_DATA_in = 2 => GPIO 43 (in)
    writel(49, MB86HXX_GPIO_BASE+4*44); //SFLASH_HOLD = 49 => GPIO 44 (out)
    writel(50, MB86HXX_GPIO_BASE+4*45); //SFLASH_CS = 50 => GPIO 45 (out)
    writel(51, MB86HXX_GPIO_BASE+4*46); //SFLASH_CLK = 51 => GPIO 46 (out)
    writel(52, MB86HXX_GPIO_BASE+4*47); //SFLASH_DATA_OUT = 52 => GPIO 47 (out)

	return 0;
}

int dram_init(void)
{

	return 0;
}

unsigned long flash_init(void)
{
	return 0x400000; //4MB
}

int write_buff (flash_info_t *info, uchar *src, ulong addr, ulong cnt)
{
	printf ("write_buff\n");
}



