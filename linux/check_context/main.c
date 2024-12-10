#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>

MODULE_AUTHOR("Manbing <manbing3@gmail.com>");
MODULE_DESCRIPTION("check context");
MODULE_LICENSE("GPL");

void tasklet_callback(unsigned long data);
/* workqueue callback function */
static void wq_callback(struct work_struct *work);

struct tasklet_struct *my_tasklet = NULL;
struct work_struct *my_workqueue = NULL;
static char test_data [] = "this is test data";

void tasklet_callback(unsigned long data)
{
        char *input = (char *) data;
        printk("it is tasklet\n");
        printk("input is [%s]\n", input);
}

void wq_callback(struct work_struct *work)
{
        printk("it is workqueue\n");

        kfree(work);
}

static int __init init(void)
{
        my_workqueue = kzalloc(sizeof(struct work_struct), GFP_ATOMIC);
        INIT_WORK(my_workqueue, wq_callback);
        /* Using linux kernel default work queue, system_wq. */
        schedule_work(my_workqueue);
        
        tasklet_init(my_tasklet, tasklet_callback, (unsigned long) test_data);
        tasklet_schedule(my_tasklet);
        return 0;
}

static void __exit exit(void)
{
        tasklet_kill(my_tasklet);
}

module_init(init);
module_exit(exit);
