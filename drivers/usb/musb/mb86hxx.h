/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 * Tom Rix <Tom.Rix@windriver.com>
 *
 * This file is based on the file drivers/usb/musb/davinci.h
 *
 * This is the unique part of its copyright:
 *
 * --------------------------------------------------------------------
 *
 * Copyright (c) 2008 Texas Instruments
 * Author: Thomas Abraham t-abraham@ti.com, Texas Instruments
 *
 * --------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef MUSB_MB86HXX_H_
#define MUSB_MB86HXX_H_

/* Timeout for USB module */
#define MB86HXX_USB_TIMEOUT 0x3FFFFFF

#if 1
#define MB86HXX_USB0_BASE		MB86HXX_USB_BASE
#else
//MB86H61
#define	MB86HXX_USB0_BASE		MB86HXX_USBPHY0_BASE
#endif

#define MB86HXX_USB_MODE		(MB86HXX_DMA_BASE + (0x14))
#if 0
//MB86H61
#define MB86HXX_USB0_MODE       (MB86HXX_DMA_BASE + (0x3c))
#define MB86HXX_USB1_MODE       (MB86HXX_DMA_BASE + (0x4c))
#endif
#define MB86HXX_USB_1BYTE_ACCESS	(0)
#define MB86HXX_USB_2BYTE_ACCESS	(1)
#define MB86HXX_USB_4BYTE_ACCESS	(2)

#endif /* MUSB_MB86HXX_H_ */
