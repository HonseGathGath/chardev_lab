#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "reverse_dev"
#define BUF_LEN 256

static int major_num;
static char msg_buffer[BUF_LEN];
static short msg_size;

static ssize_t device_read(struct file *filp, char __user *buffer,
                           size_t length, loff_t *offset) {
  int bytes_read = 0;

  if (*offset >= msg_size)
    return 0;

  while (length && *offset < msg_size) {
    put_user(msg_buffer[*offset], buffer++);
    length--;
    (*offset)++;
    bytes_read++;
  }
  return bytes_read;
}

static ssize_t device_write(struct file *filp, const char __user *buffer,
                            size_t length, loff_t *offset) {
  int i;
  char temp;

  if (length > BUF_LEN - 1)
    length = BUF_LEN - 1;

  if (copy_from_user(msg_buffer, buffer, length))
    return -EFAULT;

  msg_size = length;

  if (msg_buffer[msg_size - 1] == '\n')
    msg_size--;

  for (i = 0; i < msg_size / 2; i++) {
    temp = msg_buffer[i];
    msg_buffer[i] = msg_buffer[msg_size - 1 - i];
    msg_buffer[msg_size - 1 - i] = temp;
  }

  msg_buffer[msg_size] = '\n';
  msg_size++;

  printk(KERN_INFO "reverse_dev: Received %zu bytes, reversed it.\n", length);
  return length;
}

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
};

static int __init chardev_init(void) {
  major_num = register_chrdev(0, DEVICE_NAME, &fops);
  if (major_num < 0) {
    printk(KERN_ALERT "reverse_dev failed with %d\n", major_num);
    return major_num;
  }
  printk(KERN_INFO "reverse_dev loaded! Major number: %d\n", major_num);
  return 0;
}

static void __exit chardev_exit(void) {
  unregister_chrdev(major_num, DEVICE_NAME);
  printk(KERN_INFO "reverse_dev unloaded.\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ghaith lassoued");
MODULE_DESCRIPTION("A simple character device that reverses strings");
