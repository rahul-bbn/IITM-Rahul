#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rahul");
MODULE_DESCRIPTION("Syscall logger using kprobes");
MODULE_VERSION("2.0");

static struct kprobe kp_openat;
static struct kprobe kp_execve;

static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    printk(KERN_INFO "[syscall_logger] %s called by PID=%d (%s)\n",
           p->symbol_name, current->pid, current->comm);
    return 0;
}

static int __init syscall_logger_init(void)
{
    int ret;

    kp_openat.symbol_name = "__x64_sys_openat";
    kp_openat.pre_handler = handler_pre;
    ret = register_kprobe(&kp_openat);
    if (ret < 0) {
        printk(KERN_ERR "[syscall_logger] register_kprobe openat failed: %d\n", ret);
        return ret;
    }

    kp_execve.symbol_name = "__x64_sys_execve";
    kp_execve.pre_handler = handler_pre;
    ret = register_kprobe(&kp_execve);
    if (ret < 0) {
        printk(KERN_ERR "[syscall_logger] register_kprobe execve failed: %d\n", ret);
        unregister_kprobe(&kp_openat);
        return ret;
    }

    printk(KERN_INFO "[syscall_logger] Module loaded. Monitoring syscalls.\n");
    return 0;
}

static void __exit syscall_logger_exit(void)
{
    unregister_kprobe(&kp_execve);
    unregister_kprobe(&kp_openat);
    printk(KERN_INFO "[syscall_logger] Module unloaded.\n");
}

module_init(syscall_logger_init);
module_exit(syscall_logger_exit);
