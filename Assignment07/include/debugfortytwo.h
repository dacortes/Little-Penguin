/* SPDX-License-Identifier: GPL-2.0 */
#pragma once

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/mm.h>

#define AUTHOR		"dacortes"
#define DESCRIPTION	"Simple misc driver  for kernel 6.16.1"
#define VERSION		"0.1"

#define DIR_DEBUG	"fortytwo"
#define ID_FILE		"id"
#define JFF_FILE	"jiffies"
#define FOO_FILE	"foo"

#define ERR_BUFF	"[ERROR] Invalid buffer\n"

static DEFINE_MUTEX(foo_lock);

struct t_string {
	char *data;
	size_t size;
};

extern struct t_string str;

int create_foo_file(struct dentry *dir);
int create_id_file(struct dentry *dir);
int create_jff_file(struct dentry *dir);