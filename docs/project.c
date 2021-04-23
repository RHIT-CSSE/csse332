#define pr_fmt(fmt) "[csse332]: " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <linux/version.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mohammad");
MODULE_DESCRIPTION("Simple read/write to an array using sequential file");

static struct proc_dir_entry *parent_entry, *entry;
static LIST_HEAD(head);

struct array_element {
	int value;

	struct list_head llist;
};

#define CSSE332_ARRAY_SIZE 10


static ssize_t
csse332_write(struct file *filp, const char __user *buff,
	      size_t count, loff_t *offp)
{
	int ret = 0, sret = 0;
	char *kbuff;
	char c;
	int value;
	struct array_element *element, *tmp;

	kbuff = (char *)kmalloc(count + 1, GFP_KERNEL);
	if (IS_ERR(kbuff)) {
		pr_err("Out of memory, could not allocate room for copy buffer\n");
		return PTR_ERR(kbuff);
	}

	if (copy_from_user(kbuff, buff, count)) {
		pr_err("Could not copy from user...\n");
		ret = -EFAULT;
		goto exit;
	}

	/* remove trailing \n from echo */
	if (kbuff[count-1] == '\n')
		kbuff[count-1] = '\0';
	kbuff[count] = '\0';

	pr_info("Read %s from user..", kbuff);
	sret = sscanf(kbuff, "%c %d", &c, &value);
	if (sret < 2) {
		pr_err("Malformed input command...");
		ret = -EINVAL;
		goto exit;
	}

	if (c == 'A') {
		element = kmalloc(sizeof(struct array_element), GFP_KERNEL);
		element->value = value;
		list_add_tail(&element->llist, &head);
	} else if (c == 'D') {
		list_for_each_entry_safe(element, tmp, &head, llist) {
			if (element->value == value) {
				list_del(&element->llist);
				kfree(element);
			}
		}
	} else {
		ret = -EINVAL;
	}


exit:
	kfree(kbuff);
	return (ret == 0)? count : ret;

}

static void *csse332_start(struct seq_file *s, loff_t *pos)
{
	seq_printf(s, "[");
	return seq_list_start(&head, *pos);
}

static void *csse332_next(struct seq_file *s, void *v, loff_t *pos)
{
	seq_printf(s, ",");
	return seq_list_next(v, &head, pos);
}

static int csse332_show(struct seq_file *s, void *v)
{
	struct list_head *ptr = (struct list_head *)v;
	struct array_element *element =
		list_entry(ptr, struct array_element, llist);

	seq_printf(s, "%d", element->value);
	return 0;
}

static void csse332_stop(struct seq_file *s, void *v)
{
	seq_printf(s, "]\n");
}

static const struct seq_operations csse332_seqops = {
	.start = csse332_start,
	.next  = csse332_next,
	.show  = csse332_show,
	.stop  = csse332_stop,
};

static int
csse332_open(struct inode *inode, struct file *filp)
{
	return seq_open(filp, &csse332_seqops);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,6,0)
static const struct file_operations csse332_fops = {
	.owner   = THIS_MODULE,
	.open    = csse332_open,
	.read    = seq_read,
	.write   = csse332_write,
	.llseek  = seq_lseek,
	.release = seq_release,
};
#else
static const struct proc_ops csse332_fops = {
	.proc_open    = csse332_open,
	.proc_read    = seq_read,
	.proc_write   = csse332_write,
	.proc_lseek   = seq_lseek,
	.proc_release = seq_release,
};
#endif

static int
__init csse_init(void)
{
	int ret = 0, i;
	pr_info("Module csse 332 initializing...\n");

	/* try to create a parent directory in procfs */
	parent_entry = proc_mkdir("csse332", NULL);
	if (!parent_entry) {
		pr_err("Failed to create /proc/csse332...");
		return -ENOMEM;
	}

	/* create the entry for the project */
	entry = proc_create("array", 0666,
			    parent_entry, &csse332_fops);
	if (!entry) {
		pr_err("Failed to create /proc/csse332/array...");
		ret = -ENOMEM;
		goto exit_on_proc;
	}

	return ret;

exit_on_proc:
	proc_remove(parent_entry);

	return ret;
}

static void
__exit csse_exit(void)
{
	struct array_element *element, *tmp;
	pr_info("Module csse 332 exiting...\n");
	list_for_each_entry_safe(element, tmp, &head, llist) {
		list_del(&element->llist);
		kfree(element);
	}
	proc_remove(entry);
	proc_remove(parent_entry);

}

module_init(csse_init);
module_exit(csse_exit);
