# Linux Kernel Character Device: /dev/reverse

This project is a custom Linux Kernel Module (LKM) that registers a character device at `/dev/reverse`. It demonstrates safe data transfer between User Space (Ring 3) and Kernel Space (Ring 0).

## How it Works
When a user-space application writes a string to `/dev/reverse`, the kernel module safely copies the payload into kernel memory using `copy_from_user()`. It then reverses the string in place. When the user reads from the device, the kernel copies the reversed string back to user space using `copy_to_user()`.

## Core Concepts Demonstrated
* Linux Kernel Module initialization and cleanup (`module_init`, `module_exit`).
* Character device registration (`register_chrdev`).
* File operations mapping (`struct file_operations`).
* Safe memory management between kernel and user space.

## Usage (WARNING)
**Do not run this on a host machine.** Load this module in an isolated Virtual Machine. A memory fault in kernel space will cause a Kernel Panic and crash the OS.

```bash
make
sudo insmod chardev.ko
# Check dmesg for the major number assigned
dmesg | tail -n 1
# Create the device node (Replace <MAJOR_NUM> with your number)
sudo mknod /dev/reverse c <MAJOR_NUM> 0
sudo chmod 666 /dev/reverse

# Test it
echo "Hello Kernel" > /dev/reverse
cat /dev/reverse
# Output: lenreK olleH
