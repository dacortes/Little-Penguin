#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

#define AUTHOR "dacortes"
#define DESCRIPTION "Simple Hello World Module for kernel 6.16.1"
#define VERSION "0.1"
static int __init hello_init(void)
{
	pr_info("Hello World\n");
	return (0);
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
