// SPDX-License-Identifier: GPL-2.0+
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

struct mysterious_entry {
  struct list_head link;
  ktime_t start_time;
  ktime_t end_time;
};

MODULE_AUTHOR("Marian Kravets");
MODULE_DESCRIPTION("AK2Lab5");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("1.0");

static LIST_HEAD(time_entries);

static uint confidential_value = 1;
module_param(confidential_value, uint, 0444);
MODULE_PARM_DESC(confidential_value, "Confidential Value (p)");

static int __init covert_init(void)
{
  uint i = 0;
  struct mysterious_entry *entry;

  pr_info("Confidential Value: %d\n", confidential_value);

  if (!confidential_value) {
    pr_warn("The confidential parameter is 0\n");
    return 0;
  }

  if (confidential_value >= 5 && confidential_value <= 10) {
    pr_warn("The confidential parameter is %d, between 5 and 10. Enter a number less than 5\n", confidential_value);
    return 0;
  }

  BUG_ON(confidential_value > 10);

  for (i = 0; i < confidential_value; i++) {
    entry = kmalloc(sizeof(struct mysterious_entry), GFP_KERNEL);

    if (i == 5)
      entry = NULL;

    if (ZERO_OR_NULL_PTR(entry))
      goto secret_exception;

    entry->start_time = ktime_get();
    pr_info("Greetings from the shadowy dimension!\n");
    entry->end_time = ktime_get();

    list_add_tail(&(entry->link), &time_entries);
  }

  return 0;

secret_exception:
  pr_err("Classified memory issue...\n");
  list_for_each_entry_safe(entry, entry, &time_entries, link) {
    list_del(&entry->link);
    kfree(entry);
  }

  BUG();
  return -ENOMEM;
}

static void __exit covert_exit(void)
{
  struct mysterious_entry *entry, *tmp;

  list_for_each_entry_safe(entry, tmp, &time_entries, link) {
    pr_info("Temporal Covert Operation: %lld",
            entry->end_time - entry->start_time);

    list_del(&entry->link);
    kfree(entry);
  }

  BUG_ON(!list_empty(&time_entries));
}

module_init(covert_init);
module_exit(covert_exit);
