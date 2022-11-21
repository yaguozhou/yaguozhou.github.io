#include <linux/kernel.h> /* for KERN_DEBUG */
#include <linux/module.h> /* for all kernel modules */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaguo Zhou <yaguozhou@126.com>");
MODULE_DESCRIPTION("my test driver");
//MODULE_SUPPORTED_DEVICE("testdevice");

static int __init  my_init_module(void)
{
  printk(KERN_DEBUG "hello\n"); /* linux/kern_levels.h定义了日志8个级别 */
  return 0; /* 初始化成功 */
}

static void __exit my_cleanup_module(void)
{
  printk(KERN_DEBUG "goodbey\n");
}

module_init(my_init_module);
module_exit(my_cleanup_module);
