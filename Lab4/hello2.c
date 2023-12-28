// SPDX-License-Identifier: GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include "hello1.h"

MODULE_AUTHOR("Ponomarchuk Yevhenii");
MODULE_DESCRIPTION("Lab4");
MODULE_LICENSE("Dual BSD/GPL");

static uint times = 1;
module_param(times, uint, 0444);
MODULE_PARM_DESC(times, "times to print Hello World");


static int __init hello_init(void)
{
	if (times == 0 || (times >= 5 && times <= 10)) {
		pr_warn("Warning: times is equal to %d\n", times);
	} else if (times > 10) {
		pr_err("Error: times cannot be greater than 10");
		return -EINVAL;
	}
	print_hello(times);
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("CHao, Bambino2).\n");
}

module_init(hello_init);
module_exit(hello_exit);
