// SPDX-License-Identifier: GPL-2.0

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/string.h>

#define AUTHOR "dacortes"
#define DESCRIPTION "Simple misc driver  for kernel 6.16.1"
#define VERSION "0.1"

static ssize_t etx_misc_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
	if (!len || len < 0) {
		pr_err("[ERROR] Invalid buffer\n");
		return -EINVAL;
	}
	char	dst[128];
	ssize_t	error;

	if (len >= sizeof(dst))
		len = sizeof(dst);
	error = strncpy_from_user(dst, buf, len);
	if (error < 0) {
		pr_err("[ERROR] copying buffer\n");
		return error;
	}
	size_t	tmp = strlen(AUTHOR);

	if (dst[len - 1] == '\n')
		dst[len - 1] = '\0';
	if (tmp < len)
		tmp = len;
	if (strncmp(dst, AUTHOR, tmp)) {
		pr_err("[ERROR] Invalid value\n");
		return len;
	}
	pr_info("[SUCCESS] writing done correctly on fortytwo\n");
	return len;
}

static ssize_t etx_misc_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	const char *msg = "dacortes\n";

	return simple_read_from_buffer(buf, count, f_pos, msg, strlen(msg));
}

static int etx_misc_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int etx_misc_close(struct inode *inodep, struct file *filp)
{
	return 0;
}

static const struct file_operations fops = {
	.owner		= THIS_MODULE,
	.write		= etx_misc_write,
	.read		= etx_misc_read,
	.open		= etx_misc_open,
	.release	= etx_misc_close,
	.llseek		= noop_llseek,
};

struct miscdevice etx_misc_device = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "fortytwo",
	.fops	= &fops,
};

static int __init fortytwo_init(void)
{
	int error;

	error = misc_register(&etx_misc_device);
	if (error) {
		pr_err("misc_register fortytwo failed!!!\n");
		return error;
	}
	pr_info("hello misc_register fortytwo\n");
	return 0;
}

static void __exit fortytwo_exit(void)
{
	misc_deregister(&etx_misc_device);
	pr_info("misc_register fortytwo exit done!!!\n");
}


module_init(fortytwo_init);
module_exit(fortytwo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_ALIAS("misc:fortytwo");
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_VERSION(VERSION);
