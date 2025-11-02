// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

#define AUTHOR "dacortes"
#define DESCRIPTION "Simple example module demonstrating delays and computation in kernel space kernel 6.16.1"
#define VERSION "0.1"

static int do_work(int *my_int, int retval)
{
	int x;
	int y = *my_int;
	int z;

	for (x = 0; x < *my_int; ++x)
		udelay(10);

	if (y < 10) {
		/* That was a long sleep, tell userspace about it */
		pr_info("We slept a long time!\n");

		z = x * y;
		return z;
	}
	return 1;
}

static int __init my_init(void)
{
	int x = 10;

	x = do_work(&x, x);
	return x;
}

static void __exit my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_VERSION(VERSION);