/*
 *  drivers/mtd/devices/mb86hxx.c
 *
 * Copyright (C) 2008 FUJITSU MICROELECTRONICS LIMITED
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * SPI flash interface
 *
 * Copyright (C) 2008 Atmel Corporation
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <malloc.h>
#include <spi.h>
#include <spi_flash.h>

#include "spi_flash_internal.h"

#include <asm/io.h>

/* Flash opcodes. */
#define	OPCODE_WREN             0x06  /* Write enable */
#define	OPCODE_RDSR             0x05  /* Read status register */
#define	OPCODE_WRSR             0x01  /* Write status register 1 byte */
#define	OPCODE_NORM_READ        0x03  /* Read data bytes (low frequency) */
#define	OPCODE_FAST_READ        0x0b  /* Read data bytes (high frequency) */
#define	OPCODE_PP               0x02  /* Page program (up to 256 bytes) */
#define	OPCODE_BE_4K            0x20  /* Erase 4KiB block */
#define	OPCODE_BE_32K           0x52  /* Erase 32KiB block */
#define	OPCODE_CHIP_ERASE	0xc7  /* Erase whole flash chip */
#define	OPCODE_SE               0xd8  /* Sector erase (usually 64KiB) */
#define	OPCODE_RDID             0x9f  /* Read JEDEC ID */
#define OPCODE_WRDI             0x04

#define SFLASH_PORT             0x0
#define SFLASH_COMMAND          0x4
#define SFLASH_SPEED            0x8


int spi_flash_cmd(struct spi_slave *spi, u8 cmd, void *response, size_t len)
{
#if 0
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	if (len == 0)
		flags |= SPI_XFER_END;

	ret = spi_xfer(spi, 8, &cmd, NULL, flags);
	if (ret) {
		debug("SF: Failed to send command %02x: %d\n", cmd, ret);
		return ret;
	}

	if (len) {
		ret = spi_xfer(spi, len * 8, NULL, response, SPI_XFER_END);
		if (ret)
			debug("SF: Failed to read response (%zu bytes): %d\n",
					len, ret);
	}

	return ret;
#else
	int ret = 0;
    u32 val1;

	debug("spi_flash_cmd: cmd=0x%x, len=%d\n", cmd, len);

	switch (cmd)
	{
		case OPCODE_WREN:
			writel(cmd,MB86HXX_SFLASH_BASE+SFLASH_COMMAND);
			readl(MB86HXX_SFLASH_BASE+SFLASH_PORT);
			break;

		case OPCODE_RDSR:
			writel(cmd, MB86HXX_SFLASH_BASE+SFLASH_COMMAND);
			val1=readl(  MB86HXX_SFLASH_BASE+SFLASH_PORT);
			memcpy(response, &val1, len);
			break;
	}

	return ret;
#endif
}

int spi_flash_cmd_read(struct spi_slave *spi, const u8 *cmd,
		size_t cmd_len, void *data, size_t data_len)
{
#if 0
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	if (data_len == 0)
		flags |= SPI_XFER_END;

	ret = spi_xfer(spi, cmd_len * 8, cmd, NULL, flags);
	if (ret) {
		debug("SF: Failed to send read command (%zu bytes): %d\n",
				cmd_len, ret);
	} else if (data_len != 0) {
		ret = spi_xfer(spi, data_len * 8, NULL, data, SPI_XFER_END);
		if (ret)
			debug("SF: Failed to read %zu bytes of data: %d\n",
					data_len, ret);
	}

	return ret;
#else
	u32 index,left,from,data_word;
    u_char *read_buf = data;

	left = data_len%sizeof(s32);
	from = (cmd[1] << 16) | (cmd[2] << 8) | cmd[3];

	debug("spi_flash_cmd_read: cmd=0x%x, from=0x%x\n",
			cmd[0], from);

    writel(OPCODE_NORM_READ,MB86HXX_SFLASH_BASE+SFLASH_COMMAND);
    writel(from,  MB86HXX_SFLASH_BASE+SFLASH_PORT);
    for( index=0; index < data_len/4; index++ )
    {
        data_word = readl(MB86HXX_SFLASH_BASE+SFLASH_PORT);
        memcpy(read_buf,&data_word,sizeof(u32));
        read_buf += sizeof(u32);
    }

    if(left > 0)
    {
        data_word = 0xffffffff;
        data_word = readl(MB86HXX_SFLASH_BASE+SFLASH_PORT);
        memcpy(read_buf,&data_word,left);
    }

	return 0;
#endif
}

int spi_flash_cmd_write(struct spi_slave *spi, const u8 *cmd, size_t cmd_len,
		const void *data, size_t data_len)
{
#if 0
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	if (data_len == 0)
		flags |= SPI_XFER_END;

	ret = spi_xfer(spi, cmd_len * 8, cmd, NULL, flags);
	if (ret) {
		debug("SF: Failed to send read command (%zu bytes): %d\n",
				cmd_len, ret);
	} else if (data_len != 0) {
		ret = spi_xfer(spi, data_len * 8, data, NULL, SPI_XFER_END);
		if (ret)
			debug("SF: Failed to read %zu bytes of data: %d\n",
					data_len, ret);
	}

	return ret;
#else
	int ret = 0;
	u32 to = (cmd[1] << 16) | (cmd[2] << 8) | cmd[3];
    u32 i,addr,left,data_word;
    u_char *wptr = NULL;

    wptr =(u_char*) data;
    addr = to;

    /* what page do we start with? */
    left=data_len%sizeof(u32);
    data_word=0xffffffff;

	debug("spi_flash_cmd_write: cmd=0x%x, addr=0x%x\n", cmd[0], addr);

	switch (cmd[0])
	{
		case OPCODE_SE:
		    writel(OPCODE_SE,MB86HXX_SFLASH_BASE+SFLASH_COMMAND);
		    writel(addr,MB86HXX_SFLASH_BASE+SFLASH_PORT);/*address should be written*/
		    break;

		case OPCODE_PP:
		    if((addr&0xff)!=0)
		    {
		        writel(OPCODE_PP,MB86HXX_SFLASH_BASE+SFLASH_COMMAND);
		        writel((u32)addr,MB86HXX_SFLASH_BASE+SFLASH_PORT);
		    }

		    for(i=0;i<data_len/4;i++)
		    {
		        if((addr&0xff) ==0)
		        {
		            writel(OPCODE_PP,MB86HXX_SFLASH_BASE+SFLASH_COMMAND);
		            writel((u32)(addr),MB86HXX_SFLASH_BASE+SFLASH_PORT);
		        }
		        memcpy(&data_word,wptr,sizeof(u32));
		        writel(data_word,MB86HXX_SFLASH_BASE+SFLASH_PORT);

		        addr+=sizeof(u32);
		        wptr+=sizeof(u32);
		    }
		    if(left>0)
		    {
		        if((addr&0xff) ==0)
		        {
		            writel(OPCODE_PP,MB86HXX_SFLASH_BASE+SFLASH_COMMAND);
		            writel((u32)(addr),MB86HXX_SFLASH_BASE+SFLASH_PORT);
		         }
		        data_word=0xffffffff;
		         memcpy(&data_word,wptr,left);
			 writel(data_word,MB86HXX_SFLASH_BASE+SFLASH_PORT);
		    }

			break;

		default:
			ret = -1;
			break;
	}

	return ret;
#endif
}


int spi_flash_read_common(struct spi_flash *flash, const u8 *cmd,
		size_t cmd_len, void *data, size_t data_len)
{
	struct spi_slave *spi = flash->spi;
	int ret;

	spi_claim_bus(spi);
	ret = spi_flash_cmd_read(spi, cmd, cmd_len, data, data_len);
	spi_release_bus(spi);

	return ret;
}

struct spi_flash *spi_flash_probe(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode)
{
	struct spi_slave *spi;
	struct spi_flash *flash;
	int ret;
	u8 idcode[5];
    u8    code = OPCODE_RDID;
    u32    jedec;

	spi = spi_setup_slave(bus, cs, max_hz, spi_mode);
	if (!spi) {
		debug("SF: Failed to set up slave\n");
		return NULL;
	}

	ret = spi_claim_bus(spi);
	if (ret) {
		debug("SF: Failed to claim SPI bus: %d\n", ret);
		goto err_claim_bus;
	}

    /*24.75mhz*/
    writel(0, MB86HXX_SFLASH_BASE+SFLASH_SPEED);
	/* Read the ID codes */
#if 0
	ret = spi_flash_cmd(spi, CMD_READ_ID, &idcode, sizeof(idcode));
	if (ret)
		goto err_read_id;

	debug("SF: Got idcode %02x %02x %02x %02x %02x\n", idcode[0],
			idcode[1], idcode[2], idcode[3], idcode[4]);
#else
    writel(code,MB86HXX_SFLASH_BASE+SFLASH_COMMAND);
    jedec = readl(MB86HXX_SFLASH_BASE+SFLASH_PORT);
	idcode[0] = (jedec >> 16) & 0xff;
	idcode[1] = (jedec >> 8) & 0xff;
	idcode[2] = jedec & 0xff;

	debug("SF: Got idcode %02x %02x %02x\n", idcode[0],
			idcode[1], idcode[2]);
#endif

	switch (idcode[0]) {
#ifdef CONFIG_SPI_FLASH_SPANSION
	case 0x01:
		flash = spi_flash_probe_spansion(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_ATMEL
	case 0x1F:
		flash = spi_flash_probe_atmel(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_MACRONIX
	case 0xc2:
		flash = spi_flash_probe_macronix(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_WINBOND
	case 0xef:
		flash = spi_flash_probe_winbond(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_STMICRO
	case 0x20:
		flash = spi_flash_probe_stmicro(spi, idcode);
		break;
#endif
#ifdef CONFIG_SPI_FLASH_SST
	case 0xBF:
		flash = spi_flash_probe_sst(spi, idcode);
		break;
#endif
	default:
		debug("SF: Unsupported manufacturer %02X\n", idcode[0]);
		flash = NULL;
		break;
	}

	if (!flash)
		goto err_manufacturer_probe;

	spi_release_bus(spi);

	return flash;

err_manufacturer_probe:
err_read_id:
	spi_release_bus(spi);
err_claim_bus:
	spi_free_slave(spi);
	return NULL;
}

void spi_flash_free(struct spi_flash *flash)
{
	spi_free_slave(flash->spi);
	free(flash);
}
