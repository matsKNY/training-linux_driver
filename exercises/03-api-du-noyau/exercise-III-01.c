// SPDX-License-Identifier: GPL-2.0
//
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/timekeeping.h>

#define NB_TIMESTAMPS 50

static int __init example_init(void)
{
    time64_t timestamps[NB_TIMESTAMPS] = {[0 ... 9] = 0};

    for (uint64_t i = 0 ; i < NB_TIMESTAMPS ; i++)
    {
        timestamps[i] = ktime_get_ns();
    }

    for (uint64_t i = 1 ; i < NB_TIMESTAMPS ; i++)
    {
        pr_info(
            "%s: timestamp[%llu] = %lld; timelapse[%llu] = %lld\n",
            THIS_MODULE->name,
            i,
            timestamps[i],
            i,
            timestamps[i] - timestamps[i-1]
        );
    }

	return 0;
}

static void __exit example_exit(void)
{
	pr_info("%s: unloading\n", THIS_MODULE->name);
}

module_init(example_init);
module_exit(example_exit);

MODULE_DESCRIPTION("Exercise-III-01");
MODULE_AUTHOR("Mathieu Stoffel <mathieu.stoffel@eviden.com>");
MODULE_LICENSE("GPL v2");
