/* SPDX-License-Identifier: GPL-2.0 */
#pragma once

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/path.h>
#include <linux/mount.h>
#include <linux/nsproxy.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define AUTHOR          "dacortes"
#define DESCRIPTION     "List all mount points in /proc/mymounts"
#define VERSION         "0.1"

#define ERR_BUFF        "[ERROR] Invalid buffer\n"