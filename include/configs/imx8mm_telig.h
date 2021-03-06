/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2020 Gateworks Corporation
 * Copyright 2020 Council Rock Enterprises
 */

#ifndef __IMX8MM_TELIG_H
#define __IMX8MM_TELIG_H

#include <linux/sizes.h>
#include <asm/arch/imx-regs.h>

#ifndef CONFIG_SPL_BUILD
#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 1) \
	func(MMC, mmc, 2) \
	func(USB, usb, 0) \
	func(USB, usb, 1) \
	func(PXE, pxe, na)

#include <config_distro_bootcmd.h>
#endif

#ifdef CONFIG_SECURE_BOOT
#define CONFIG_CSF_SIZE			SZ_8K
#endif

#define CONFIG_SPL_MAX_SIZE		(148 * 1024)
#define CONFIG_SYS_MONITOR_LEN		SZ_512K
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_USE_SECTOR
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	0x300
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION	1
#define CONFIG_SYS_UBOOT_BASE	\
	(QSPI0_AMBA_BASE + CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR * 512)

#ifdef CONFIG_SPL_BUILD
#define CONFIG_SPL_STACK		0x920000
#define CONFIG_SPL_BSS_START_ADDR	0x910000
#define CONFIG_SPL_BSS_MAX_SIZE		SZ_8K	/* 8 KB */
#define CONFIG_SYS_SPL_MALLOC_START	0x42200000
#define CONFIG_SYS_SPL_MALLOC_SIZE	SZ_512K	/* 512 KB */

/* malloc f used before GD_FLG_FULL_MALLOC_INIT set */
#define CONFIG_MALLOC_F_ADDR		0x930000
/* For RAW image gives a error info not panic */
#define CONFIG_SPL_ABORT_ON_RAW_IMAGE

#endif

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"script=boot.scr\0" \
	"fdt_high=0xffffffff\0" \
	"initrd_high=0xffffffff\0" \
	"fdt_addr_r=0x42000000\0" \
	"kernel_addr_r=0x43000000\0" \
	"ramdisk_addr_r=0x43000000\0" \
	"pxefile_addr_r=0x43000000\0" \
	"scriptaddr=0x43000000\0" \
	"ipaddr=10.10.1.100\0" \
	"serverip=10.10.1.1\0" \
	"console=ttymxc1,115200 earlycon=ec_imx6q,0x30890000,115200\0" \
        BOOTENV

/* Link Definitions */
#define CONFIG_LOADADDR			0x40200000
#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

#define CONFIG_SYS_INIT_RAM_ADDR        0x40000000
#define CONFIG_SYS_INIT_RAM_SIZE        0x200000
#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_ENV_OVERWRITE
#define CONFIG_SYS_MMC_ENV_DEV		2   /* USDHC3 */

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		SZ_32M

#define CONFIG_SYS_SDRAM_BASE           0x40000000
#define PHYS_SDRAM                      0x40000000
#define PHYS_SDRAM_SIZE			0x40000000 /* 1GB DDR */
#define CONFIG_SYS_BOOTM_LEN		SZ_64M

#define CONFIG_SYS_MEMTEST_START	PHYS_SDRAM
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + (PHYS_SDRAM_SIZE >> 1))

#define CONFIG_MXC_UART_BASE		UART2_BASE_ADDR

/*
 * #######################################
 * ### USB                             ###
 * #######################################
 */
#ifdef CONFIG_USB_EHCI_HCD
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_MXC_USB_PORTSC           (PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS            0
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2
#endif

/* Monitor Command Prompt */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_CBSIZE		2048
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)

/* USDHC */
#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_FSL_ESDHC_ADDR	0

#define CONFIG_SYS_MMC_IMG_LOAD_PART	1

#define CONFIG_SYS_I2C_SPEED		100000

#define CONFIG_ETHPRIME                 "eth0"

#define CONFIG_FEC_XCV_TYPE             RGMII
#define CONFIG_FEC_MXC_PHYADDR          0
#define FEC_QUIRK_ENET_MAC

#endif
