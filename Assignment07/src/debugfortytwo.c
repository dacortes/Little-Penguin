// SPDX-License-Identifier: GPL-2.0

#include <debugfortytwo.h>

extern struct t_string str;

static struct dentry *dir;

static int __init test_init(void)
{
	dir = debugfs_create_dir(DIR_DEBUG, NULL);

	if (!dir)
		return -ENOMEM;

	if (create_foo_file(dir) == -ENOMEM)
		return -ENOMEM;

	if (create_id_file(dir) == -ENOMEM)
		return -ENOMEM;

	if (create_jff_file(dir) == -ENOMEM)
		return -ENOMEM;

	pr_info("%s: uploaded\n", DIR_DEBUG);
	return 0;
}

static void __exit test_exit(void)
{
	mutex_lock(&foo_lock);
	kfree(str.data);
	str.data = NULL;
	debugfs_remove_recursive(dir);
	pr_info("%s: downloaded\n", DIR_DEBUG);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTHOR);
MODULE_ALIAS("debug:fortytwo");
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_VERSION(VERSION);
