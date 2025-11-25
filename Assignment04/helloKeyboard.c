// SPDX-License-Identifier: GPL-2.0

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/hid.h>
#include <linux/usb.h>


#define AUTHOR "dacortes"
#define DESCRIPTION "Simple Hello World Module for kernel 6.16.1"
#define VERSION "0.1"
#define DRIVER_NAME "helloKeyboard"
#define	DEVICE_NAME "helloKeyboard"


static struct usb_device_id hello_usb_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{}
};
MODULE_DEVICE_TABLE(usb, hello_usb_table);

static int hello_usb_probe(struct usb_interface *interface,
		const struct usb_device_id *id)
{
	struct usb_device *udev = interface_to_usbdev(interface);
	unsigned short vendor = le16_to_cpu(udev->descriptor.idVendor);
	unsigned short product = le16_to_cpu(udev->descriptor.idProduct);

	dev_info(&interface->dev,
		"Hello :D USB device connected: vendor=0x%04x, product=0x%04x\n",
		vendor, product);
	return 0;
}

static void bye_usb_probe(struct usb_interface *interface)
{
	dev_info(&interface->dev, "Keyboard disconnected\n");
}

static struct usb_driver hello_usb_driver = {
	.name = "hello keyboard",
	.probe = hello_usb_probe,
	.disconnect = bye_usb_probe,
	.id_table = hello_usb_table,
};


static int __init hello_init(void)
{
	int	result;

	pr_info("Hello world - USB keyboard module\n");
	result = usb_register(&hello_usb_driver);
	if (result)
		pr_err("Error registering usb device\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("Cleaning up module\n");
}


module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_VERSION(VERSION);
