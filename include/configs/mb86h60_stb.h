
#ifndef MB86H60_STB_H_
#define MB86H60_STB_H_

#define CONFIG_MACH_FUJITSU_TVSTBSOC_MB86HXX_H60
#include <asm/arch/mb86hxx.h>

#define CONFIG_SYS_SDRAM_BASE		0x20000000
#define CONFIG_SYS_LOAD_ADDR            0x22e08000 //0x20000000
#define CONFIG_SYS_UBOOT_BASE		TEXT_BASE	//0x23F00000

/* Memory Info */
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE	/* memtest works on	      */
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x3800000) /* 56MB in DRAM */

#define CONFIG_NR_DRAM_BANKS		1

#define CONFIG_SYS_HZ                   1000

/* Serial Driver Info */
#define CONFIG_PL011_SERIAL
#define CONFIG_PL011_CLOCK		MB86HXX_APB_HZ //81000000
#define CONFIG_PL01x_PORTS		{(void*) MB86HXX_UART0_BASE} //0xc2000000}
#define CONFIG_CONS_INDEX		0
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/* Flash and environment info */
#define CONFIG_MB86HXX_SFLASH
#define CONFIG_MB86HXX_SPI
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS

#define CONFIG_SYS_MAX_FLASH_SECT       64
#define CONFIG_SYS_MAX_FLASH_BANKS      1
#define CONFIG_ENV_OFFSET		0x00000
#define CONFIG_ENV_SECT_SIZE	0x10000

//#define CONFIG_SYS_NO_FLASH
//#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SIZE				8192

#define CONFIG_SYS_PROMPT		"MB86H60 > "
#define CONFIG_SYS_CBSIZE		256		/* Console I/O Buffer Size    */
#define CONFIG_SYS_PBSIZE		384		/* Print Buffer Size          */

#define CONFIG_SYS_MAXARGS		16

#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 128*1024)
#define CONFIG_SYS_GBL_DATA_SIZE	128

//#include <config_cmd_default.h>
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop mtest */
#define CONFIG_CMD_SF
#define CONFIG_CMD_SAVEENV

#endif /* MB86H60_STB_H_ */
