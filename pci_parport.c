#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple LKM for a PCI to parallel port adapter");

#define VENDOR_ID 0x9710
#define DEVICE_ID 0x9805

/** PCI device */
static struct pci_dev *ptr;

/** variable for timer */
static struct timer_list my_timer;

/** Pointer to PCI device's BAR0 */
static void __iomem * parport_bar0;

/**
 * @brief Timer Callback Function
 *
 * Reads in Inputs from Parport and 
 * writes value in 7 segment display
 */
void timer_callback(struct timer_list * data) {
	u8 regval;

	/* 7 Segment characters */
	u8 display[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111, 119, 124, 57, 94, 121, 113};

	/* Read DIP switches */
	regval = ioread8((parport_bar0 + 1)) >> 3;

	/* Writing to 7 segment display */
	iowrite8(display[regval & 0xf], parport_bar0);

	/* Restart Timer */
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(200));
}
/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	u16 val;

	/* Check if a PCI device with vendor and device id is available */
	ptr = pci_get_device(VENDOR_ID, DEVICE_ID, ptr);
	if(ptr == NULL) {
		printk("pci_parport - PCI to parport adapter is not available!\n");
		return -1;
	}

	/* Enable the device */
	if(pci_enable_device(ptr) < 0) {
		printk("pci_parport - Could not enable the device!\n");
		return -1;
	}

	/* Access config space */
	pci_read_config_word(ptr, PCI_VENDOR_ID, &val);
	printk("pci_parport - VENDOR ID: 0x%x\n", val);
	pci_read_config_word(ptr, PCI_DEVICE_ID, &val);
	printk("pci_parport - DEVICE ID: 0x%x\n", val);

	/* Request and Map BAR0 */
	if(pci_request_region(ptr, 0, "pci_parport_bar0")){
		printk("pci_parport - Could not request BAR0! MAybe already used?\n");
		return -1;
	}

	parport_bar0 = pci_iomap(ptr, 0, pci_resource_len(ptr, 0));

	/* Initialize timer */
	timer_setup(&my_timer, timer_callback, 0);
	mod_timer(&my_timer, jiffies + msecs_to_jiffies(200));

	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("pci_parport - Unload module\n");
	del_timer(&my_timer);
	pci_release_region(ptr, 0);
}

module_init(ModuleInit);
module_exit(ModuleExit);


