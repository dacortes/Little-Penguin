// SPDX-License-Identifier: GPL-2.0

#include <debugfortytwo.h>

static struct dentry *foo_file;
struct t_string str = { NULL, 0 };

static ssize_t foo_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
	char *tmp;

	if (!len || (len + 1) > PAGE_SIZE)
		return -EINVAL;

	tmp = kcalloc(len + 1, sizeof(char), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;

	if (copy_from_user(tmp, buf, len)) {
		kfree(tmp);
		return -EFAULT;
	}

	mutex_lock(&foo_lock);

	kfree(str.data);
	str.data = tmp;
	str.size = len;

	mutex_unlock(&foo_lock);

	pr_info("[foo] written: %s\n", str.data);

	return len;
}

static ssize_t foo_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t ret;

	mutex_lock(&foo_lock);

	if (!str.data) {
		mutex_unlock(&foo_lock);
		return 0;
	}
	ret = simple_read_from_buffer(buf, count, f_pos, str.data, str.size);
	mutex_unlock(&foo_lock);
	return ret;
}

static int foo_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int foo_close(struct inode *inodep, struct file *filp)
{
	return 0;
}


static const struct file_operations fops = {
	.owner		= THIS_MODULE,
	.write		= foo_write,
	.read		= foo_read,
	.open		= foo_open,
	.release	= foo_close,
	.llseek		= noop_llseek,
};

int create_foo_file(struct dentry *dir)
{
	foo_file = debugfs_create_file(FOO_FILE, 0644, dir, NULL, &fops);
	if (!foo_file)
		return -ENOMEM;
	return 0;
}
