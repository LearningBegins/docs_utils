/* **************** LDD:1.0 s_32/lab1_cpufreq.c **************** */
/*
 * The code herein is: Copyright Jerry Cooperstein, 2009
 *
 * This Copyright is retained for the purpose of protecting free
 * redistribution of source.
 *
 *     URL:    http://www.coopj.com
 *     email:  coop@coopj.com
 *
 * The primary maintainer for this code is Jerry Cooperstein
 * The CONTRIBUTORS file (distributed with this
 * file) lists those known to have contributed to the source.
 *
 * This code is distributed under Version 2 of the GNU General Public
 * License, which you should have received with the source.
 *
 */
/* CPU Frequency Notifiers
 *
 * Write a module that registers callback functions for the CPU
 * frequency transition and policy notifier chains.
 *
 * Print out what event is causing the callback, and some information
 * from the data structures delivered to the callback functions.
 *
 * You can test this by echoing values to some of the entries in
 * /sys/devices/system/cpu/cpu0/cpufreq.  Even easier you can add the
 * CPU Frequency Scaling Monitor applet to your taskbar, and easily
 * switch governors and frequencies.
 *
 @*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/notifier.h>
#include <linux/cpufreq.h>

static int my_transition_notifier_call (struct notifier_block *b,
                                        unsigned long event, void *data)
{
    struct cpufreq_freqs *cf = data;
    printk (KERN_INFO "\nRECEIVING CPUFREQ TRANSITION event = %ld\n", event);
    switch (event) {
    case CPUFREQ_PRECHANGE:
        printk (KERN_INFO "event=CPUFREQ_PRECHANGE\n");
        break;
    case CPUFREQ_POSTCHANGE:
        printk (KERN_INFO "event=CPUFREQ_POSTCHANGE\n");
        break;
    case CPUFREQ_RESUMECHANGE:
        printk (KERN_INFO "event=CPUFREQ_RESUMECHANGE\n");
        break;
    case CPUFREQ_SUSPENDCHANGE:
        printk (KERN_INFO "event=CPUFREQ_SUSPENDCHANGE\n");
        break;
    default:
        printk (KERN_INFO "Receiving an unknown CPUFREQ TRANSITION event\n");
        break;
    }

    printk (KERN_INFO "       cpu=%d, old=%d, new=%d, flags=%d\n",
            cf->cpu, cf->old, cf->new, cf->flags);

    return NOTIFY_OK;
}
static int my_policy_notifier_call (struct notifier_block *b,
                                    unsigned long event, void *data)
{
    struct cpufreq_policy *cp = data;
    printk (KERN_INFO "\nRECEIVING CPUFREQ POLICY event = %ld\n", event);
    switch (event) {
    case CPUFREQ_ADJUST:
        printk (KERN_INFO "event=CPUFREQ_ADJUST\n");
        break;
    case CPUFREQ_INCOMPATIBLE:
        printk (KERN_INFO "event=CPUFREQ_INCOMPATIBLE\n");
        break;
    case CPUFREQ_NOTIFY:
        printk (KERN_INFO "event=CPUFREQ_NOTIFY\n");
        break;
    default:
        printk (KERN_INFO "Receiving an unknown CPUFREQ POLICY event\n");
        break;
    }
    printk (KERN_INFO "      cpu=%d, min=%d, max=%d, cur=%d, policy=%d\n",
            cp->cpu, cp->min, cp->max, cp->cur, cp->policy);
    return NOTIFY_OK;
}

static struct notifier_block my_transition_nh_block = {
    .notifier_call = my_transition_notifier_call,
    .priority = 0,
};

static struct notifier_block my_policy_nh_block = {
    .notifier_call = my_policy_notifier_call,
    .priority = 0,
};

static int __init my_init (void)
{
    cpufreq_register_notifier (&my_transition_nh_block,
                               CPUFREQ_TRANSITION_NOTIFIER);
    cpufreq_register_notifier (&my_policy_nh_block, CPUFREQ_POLICY_NOTIFIER);
    printk (KERN_INFO "\nCPUFREQ Notifier module successfully loaded\n");

    return 0;
}
static void __exit my_exit (void)
{
    cpufreq_unregister_notifier (&my_transition_nh_block,
                                 CPUFREQ_TRANSITION_NOTIFIER);
    cpufreq_unregister_notifier (&my_policy_nh_block, CPUFREQ_POLICY_NOTIFIER);
    printk (KERN_INFO "\nCPUFREQ Notifier module successfully unloaded\n");
}

module_init (my_init);
module_exit (my_exit);

MODULE_AUTHOR ("Jerry Cooperstein");
MODULE_DESCRIPTION ("LDD:1.0 s_32/lab1_cpufreq.c");
MODULE_LICENSE ("GPL v2");
