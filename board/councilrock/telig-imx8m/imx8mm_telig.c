// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2020 Gateworks Corporation
 * Copyright 2020 Council Rock Enterprises
 */

#include <common.h>
#include <miiphy.h>
#include <netdev.h>

#include <asm/arch/clock.h>
#include <asm/arch/sys_proto.h>
#include <asm/io.h>

#include "gsc.h"

DECLARE_GLOBAL_DATA_PTR;

int dram_init(void)
{
	/* TODO adjust per EEPROM */
	gd->ram_size = PHYS_SDRAM_SIZE;

	return 0;
}

#if IS_ENABLED(CONFIG_FEC_MXC)
static int setup_fec(void)
{
	struct iomuxc_gpr_base_regs *gpr =
		(struct iomuxc_gpr_base_regs *)IOMUXC_GPR_BASE_ADDR;

	/* Use 125M anatop REF_CLK1 for ENET1, not from external */
	clrsetbits_le32(&gpr->gpr[1], 0x2000, 0);

	return 0;
}

int board_phy_config(struct phy_device *phydev)
{
	unsigned short val;

        /* TI DP83867 */
	if (phydev->phy_id == 0x2000a231) {
		puts("DP83867 ");
		/* LED configuration */
		val = 0;
		val |= 0x5 << 4; /* LED1(Amber;Speed)   : 1000BT link */
		val |= 0xb << 8; /* LED2(Green;Link/Act): blink for TX/RX act */
		phy_write(phydev, MDIO_DEVAD_NONE, 24, val);
	}

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	return 0;
}
#endif

#if !defined(DM_USB)
#include <imx_sip.h>
#include <usb.h>
#include <asm/gpio.h>
#include <asm/arch/imx8mm_pins.h>
#include <asm/mach-imx/iomux-v3.h>

#define USB1_HUB_RST IMX_GPIO_NR(1, 8)

static iomux_v3_cfg_t const usb1_vbus_en_pads[] = {
	IMX8MM_PAD_GPIO1_IO08_GPIO1_IO8 | MUX_PAD_CTRL(NO_PAD_CTRL),
};

int board_ehci_hcd_init(int index) {
	debug("%s port%d\n", __func__, index);

	if (index == 1) {
		// enable power domain
		call_imx_sip(IMX_SIP_GPC, IMX_SIP_GPC_PM_DOMAIN,
			     2 + index, 1, 0);
		// toggle HUB RST
		imx_iomux_v3_setup_multiple_pads(usb1_vbus_en_pads,
						ARRAY_SIZE(usb1_vbus_en_pads));
		gpio_request(USB1_HUB_RST, "usb1_hub_rst#");
		gpio_direction_output(USB1_HUB_RST, 0);
		udelay(5);
		gpio_direction_output(USB1_HUB_RST, 1);
		mdelay(100);
	}

	return 0;
}

/* override driver version which always returns device
 * return either USB_INIT_DEVICE or USB_INIT_HOST
 */
int board_usb_phy_mode(int index) {
        debug("%s port%d\n", __func__, index);

	if (index == 0)
		return USB_INIT_DEVICE;

	return USB_INIT_HOST;
}
#endif /* !defined DM_USB */

int board_init(void)
{
	// TODO: configure LED's

	if (IS_ENABLED(CONFIG_FEC_MXC))
		setup_fec();

	gsc_hwmon();

	return 0;
}

int board_mmc_get_env_dev(int devno)
{
	return devno;
}
