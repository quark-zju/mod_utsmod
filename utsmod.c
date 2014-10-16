/*  utsmod - sysctl interface to modify fields of a uts namespace
 *
 *  Copyright (C) 2014 Jun Wu <quark@lihdd.net>
 *
 *  This file contains code that was borrowed from Linux 3.16.3
 *  kernel/utsname_sysctl.c, which has the following copyright:
 *
 *  Copyright (C) 2007
 *
 *  Author: Eric Biederman <ebiederm@xmision.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2 of the
 *  License.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/uts.h>
#include <linux/utsname.h>
#include <linux/sysctl.h>
#include <linux/wait.h>
#include <linux/capability.h>

static struct ctl_table_header * utsmod_table_header;

DECLARE_RWSEM(utsmod_sem);

static void *utsmod_get_uts(struct ctl_table *table, int write) {
  char *which = table->data;
  struct uts_namespace *uts_ns;

  uts_ns = current->nsproxy->uts_ns;
  which = (which - (char *)&init_uts_ns) + (char *)uts_ns;

  if (!write)
    down_read(&utsmod_sem);
  else
    down_write(&utsmod_sem);
  return which;
}

static void utsmod_put_uts(struct ctl_table *table, int write, void *which) {
  if (!write)
    up_read(&utsmod_sem);
  else
    up_write(&utsmod_sem);
}

static int utsmod_proc_do_string(struct ctl_table *table, int write, void __user *buffer, size_t *lenp, loff_t *ppos) {
  struct ctl_table utsmod_table;
  int r;

  if (write) {
    // init_uts_ns is read only
    if (current->nsproxy->uts_ns == &init_uts_ns)
      return -EPERM;

    // require CAP_SYS_ADMIN
    if (!ns_capable(current->nsproxy->uts_ns->user_ns, CAP_SYS_ADMIN))
      return -EPERM;
  }

  // update current utsns
  memcpy(&utsmod_table, table, sizeof(utsmod_table));
  utsmod_table.data = utsmod_get_uts(table, write);
  r = proc_dostring(&utsmod_table, write, buffer, lenp, ppos);
  utsmod_put_uts(table, write, utsmod_table.data);

  return r;
}

static struct ctl_table utsmod_child_table[] = {
  {
    .procname	= "ostype",
    .data		= init_uts_ns.name.sysname,
    .maxlen		= sizeof(init_uts_ns.name.sysname),
    .mode		= 0644,
    .proc_handler	= utsmod_proc_do_string,
  },
  {
    .procname	= "osrelease",
    .data		= init_uts_ns.name.release,
    .maxlen		= sizeof(init_uts_ns.name.release),
    .mode		= 0644,
    .proc_handler	= utsmod_proc_do_string,
  },
  {
    .procname	= "version",
    .data		= init_uts_ns.name.version,
    .maxlen		= sizeof(init_uts_ns.name.version),
    .mode		= 0644,
    .proc_handler	= utsmod_proc_do_string,
  },
  {}
};

static struct ctl_table utsmod_root_table[] = {
  {
    .procname	= "utsmod",
    .mode		= 0555,
    .child		= utsmod_child_table,
  },
  {}
};

static int utsmod_init(void) {
  utsmod_table_header = register_sysctl_table(utsmod_root_table);
  if (utsmod_table_header == NULL) {
    printk(KERN_INFO "register_sysctl_table failed\n");
    return -1;
  }
  return 0;
}

static void utsmod_exit(void) {
  if (utsmod_table_header != NULL) {
    unregister_sysctl_table(utsmod_table_header);
  }
}

module_init(utsmod_init);
module_exit(utsmod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jun Wu");
MODULE_DESCRIPTION("Provide interfaces to modify read-only fields of a UTS namespace");
