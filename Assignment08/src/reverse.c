// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("reverse string misc device");

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs);
static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs);
static int myfd_open(struct inode *inode, struct file *file);
static int myfd_close(struct inode *inodep, struct file *filp);

static char str[PAGE_SIZE];
static char *tmp;

static DEFINE_MUTEX(str_lock);

static const struct file_operations myfd_fops = {
	.owner		= THIS_MODULE,
	.read		= myfd_read,
	.write		= myfd_write,
	.open		= myfd_open,
	.release	= myfd_close,
	.llseek		= noop_llseek,
};

static struct miscdevice myfd_device = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "reverse",
	.fops	= &myfd_fops
};


static int __init myfd_init(void)
{
	int retval;

	retval = misc_register(&myfd_device);
	if (retval) {
		pr_err("[ERROR] misc_register reverse failed!\n");
		return retval;
	}
	pr_info("[SUCCESS] reverse: uploaded\n");
	return 0;
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
	pr_info("[SUCCESS] rever: downloaded\n");
}

ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	size_t len, i = 0, j;

	mutex_lock(&str_lock);
	len = strnlen(str, PAGE_SIZE);
	if (!len) {
		mutex_unlock(&str_lock);
		return -EINVAL;
	}
	tmp = kcalloc(len + 2, sizeof(char), GFP_KERNEL);
	if (!tmp) {
		mutex_unlock(&str_lock);
		return -ENOMEM;
	}
	j = len - (!str[len]);
	while (i < len)
		tmp[i++] = str[j--];
	ssize_t ret = simple_read_from_buffer(user, size, offs, tmp, i);

	pr_info("[SUCCESS] read: %s\n", tmp);
	kfree(tmp);
	mutex_unlock(&str_lock);
	return ret;
}


ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
	ssize_t res;

	if (!size)
		return -EINVAL;

	if (size >= PAGE_SIZE)
		size = PAGE_SIZE - 1;

	mutex_lock(&str_lock);
	res = simple_write_to_buffer(str, size, offs, user, size);
	str[size] = '\0';
	mutex_unlock(&str_lock);
	pr_info("[SUCCESS] written: %s\n", str);
	return res;
}

int myfd_open(struct inode *inode, struct file *file)
{
	return 0;
}

int myfd_close(struct inode *inodep, struct file *filp)
{
	return 0;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
