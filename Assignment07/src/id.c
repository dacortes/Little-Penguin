// SPDX-License-Identifier: GPL-2.0

#include <debugfortytwo.h>

static struct dentry *id_file;

static ssize_t id_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
	if (!len || len < 0) {
		pr_err("[ERROR] Invalid buffer\n");
		return -EINVAL;
	}
	char    dst[128];
	ssize_t error;

	if (len >= sizeof(dst))
		len = sizeof(dst);
	error = strncpy_from_user(dst, buf, len);
	if (error < 0) {
		pr_err("[ERROR] copying buffer\n");
		return error;
	}
	size_t  tmp = strlen(AUTHOR);

	if (dst[len - 1] == '\n')
		dst[len - 1] = '\0';
	if (tmp < len)
		tmp = len;
	if (strncmp(dst, AUTHOR, tmp)) {
		pr_err("[ERROR] Invalid value\n");
		return -EINVAL;
	}
	pr_info("[SUCCESS] writing done correctly on fortytwo\n");
	return len;
}

static ssize_t id_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	const char *msg = "dacortes\n";

	return simple_read_from_buffer(buf, count, f_pos, msg, strlen(msg));
}

static int id_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int id_close(struct inode *inodep, struct file *filp)
{
	return 0;
}

static const struct file_operations fops = {
	.owner		= THIS_MODULE,
	.write		= id_write,
	.read		= id_read,
	.open		= id_open,
	.release	= id_close,
	.llseek		= noop_llseek,
};

int create_id_file(struct dentry *dir)
{
	id_file = debugfs_create_file(ID_FILE, 0666, dir, NULL, &fops);

	if (!id_file)
		return -ENOMEM;
	return 0;
}
