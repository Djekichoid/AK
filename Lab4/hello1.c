// SPDX-License-Identifier: GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>
#include "hello1.h"

MODULE_AUTHOR("Ponomarchuk Yevhenii");
MODULE_DESCRIPTION("Lab4");
MODULE_LICENSE("Dual BSD/GPL");

struct message_time {
	struct list_head tlist;
	ktime_t mes_time;
	ktime_t pr_time;
};

static LIST_HEAD(hello_time_list);

static int __init hello_init(void)
{
	pr_info("Module installed");
	return 0;
}

void print_hello(uint times)
{
	int i = 0;
	ktime_t after;

	for (i = 0; i < times; i++) {
		struct message_time *tst = kmalloc(sizeof(*tst), GFP_KERNEL);

		list_add_tail(&tst->tlist, &hello_time_list);

		tst->mes_time = ktime_get();
		pr_info("Hello, world!\n");
		after = ktime_get();

		tst->pr_time = after - tst->mes_time;
	}
}
EXPORT_SYMBOL(print_hello);

static void __exit hello_exit(void)
{
	int i = 1;
	struct message_time *cur, *tmp;

	list_for_each_entry_safe(cur, tmp, &hello_time_list, tlist) {
		pr_info("[%d] Event = %lld; Print = %lld\n",
			i++, cur->mes_time, cur->pr_time);
		list_del(&cur->tlist);
		kfree(cur);
	}
	pr_info("CHao, Bambino).\n");
}

module_init(hello_init);
module_exit(hello_exit);
