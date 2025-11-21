// SPDX-License-Identifier: GPL-2.0

#include <debugfortytwo.h>

static struct dentry *jff_file;

static ssize_t jff_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
	pr_err("[ERROR] write not allowed for jiffies\n");
	return -EINVAL;
}

static ssize_t jff_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	char tmp[32];
	int len;

	len = snprintf(tmp, sizeof(tmp), "%lu\n", jiffies);
	return simple_read_from_buffer(buf, count, f_pos, tmp, len);
}

static int jff_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int jff_close(struct inode *inodep, struct file *filp)
{
	return 0;
}

static const struct file_operations fops = {
	.owner		= THIS_MODULE,
	.write		= jff_write,
	.read		= jff_read,
	.open		= jff_open,
	.release	= jff_close,
	.llseek		= noop_llseek,
};

int create_jff_file(struct dentry *dir)
{
	jff_file = debugfs_create_file(JFF_FILE, 0444, dir, NULL, &fops);
	if (!jff_file)
		return -ENOMEM;
	return 0;
}