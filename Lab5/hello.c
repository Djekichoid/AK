// SPDX-License-Identifier: GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Ponomarchuk Yevhenii");
MODULE_DESCRIPTION("Lab5");
MODULE_LICENSE("Dual BSD/GPL");

static uint times = 1;
module_param(times, uint, 0444);
MODULE_PARM_DESC(times, "Times to print Hello, World!");

struct message_time {
	struct list_head tlist;
	ktime_t time;
};

static LIST_HEAD(hello_time_list);

int i = 0;

static int __init hello_init(void)
{
	if (times == 0 || (times >= 5 && times <= 10)) {
		printk(KERN_WARNING "Warning: times is %d\n", times);
	}

	BUG_ON(times > 10);

	for (i = 0; i < times; i++) {
		struct message_time *tst = kmalloc(sizeof(*tst), GFP_KERNEL);

		if (i == times - 1) tst = NULL;

		if (tst != NULL) {
			tst->time = ktime_get();
			list_add_tail(&tst->tlist, &hello_time_list);
			printk(KERN_INFO "Hello, world!\n");
		}
		else {
			struct message_time *cur, *tmp;
        		list_for_each_entry_safe(cur, tmp, &hello_time_list, tlist) {
	                list_del(&cur->tlist);
        		kfree(cur);

			BUG();
        		}
		}
	}
	return 0;
}

static void __exit hello_exit(void)
{
	int i = 0;
	struct message_time *cur, *tmp;

	list_for_each_entry_safe(cur, tmp, &hello_time_list, tlist) {
		long long nanos = ktime_to_ns(cur->time);

		printk(KERN_INFO "%d time = %lld\n", i++, nanos);
		list_del(&cur->tlist);
		kfree(cur);
	}
	printk(KERN_INFO "CHao!, Bambino)\n");
}

module_init(hello_init);
module_exit(hello_exit);
