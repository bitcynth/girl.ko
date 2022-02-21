#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Cynthia Revström <me@cynthia.re>");
MODULE_DESCRIPTION("meow");
MODULE_VERSION("0.1.0");

dev_t dev = 0;
static struct class* dev_class;

static int __init girl_init(void) {
    printk(KERN_INFO "meow\n");

    if ((alloc_chrdev_region(&dev, 0, 1, "girl")) < 0) {
        pr_err("cannot allocate major number for dev\n");
        return -1;
    }

    if ((dev_class = class_create(THIS_MODULE, "girl")) == NULL) {
        pr_err("cannot create class\n");
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    if ((device_create(dev_class, NULL, dev, NULL, "cynthia")) == NULL) {
        pr_err("cannot create device\n");
        class_destroy(dev_class);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    return 0;
}

static void __exit girl_exit(void) {
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "meow :c\n");
}

module_init(girl_init);
module_exit(girl_exit);
