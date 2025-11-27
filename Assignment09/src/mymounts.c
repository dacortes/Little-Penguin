// SPDX-License-Identifier: GPL-2.0

#include <mymounts.h>

static int mymounts_show(struct seq_file *m, void *v)
{
	struct file *f;
	char *buf, *line, *p;
	ssize_t read_size;

	f = filp_open("/proc/self/mountinfo", O_RDONLY, 0);
	if (IS_ERR(f))
		return -EINVAL;

	buf = kzalloc(PAGE_SIZE, GFP_KERNEL);
	if (!buf) {
		filp_close(f, NULL);
		return -ENOMEM;
	}

	while ((read_size = kernel_read(f, buf, PAGE_SIZE - 1, &f->f_pos)) > 0) {

		p = buf;

		while ((line = strsep(&p, "\n")) != NULL) {

			char mount_point[256] = {0};
			char fs_type[64] = {0};
			char *token;
			int field = 0;

			char *tmp = kstrdup(line, GFP_KERNEL);
			char *t = tmp;

			while ((token = strsep(&t, " ")) != NULL) {
				field++;

				if (field == 5)
					strscpy(mount_point, token, sizeof(mount_point)-1);

				if (strcmp(token, "-") == 0) {
					token = strsep(&t, " ");
					if (token)
						strscpy(fs_type, token, sizeof(fs_type)-1);
					break;
				}
			}
			pr_info("\n");
			kfree(tmp);

			if (fs_type[0] && mount_point[0])
				seq_printf(m, "%s %s\n", fs_type, mount_point);
		}
	}

	kfree(buf);
	filp_close(f, NULL);
	return 0;
}

static int mymounts_open(struct inode *inode, struct file *file)
{
		return single_open(file, mymounts_show, NULL);
}

static const struct proc_ops mymounts_fops = {
		.proc_open      = mymounts_open,
		.proc_read      = seq_read,
		.proc_lseek     = seq_lseek,
		.proc_release   = single_release,
};

static int __init mymounts_init(void)
{
		proc_create("mymounts", 0, NULL, &mymounts_fops);
		pr_info("mymounts module loaded\n");
		return 0;
}

static void __exit mymounts_exit(void)
{
		remove_proc_entry("mymounts", NULL);
		pr_info("mymounts module unloaded\n");
}

module_init(mymounts_init);
module_exit(mymounts_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_ALIAS("mymount");
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_VERSION(VERSION);
