#include <linux/module.h>
#include <linux/init.h>
#include <linux/pci.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple LKM for a PCI to parallel port adapter");

#define VENDOR_ID 0x9710
#define DEVICE_ID 0x9805

/** PCI device */
static struct pci_dev *ptr;

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

	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("pci_parport - Unload module\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);


