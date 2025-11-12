# Syscall Logger - Linux Kernel Module

## Overview
`syscall_logger` is a simple Linux kernel module that demonstrates how to log system call invocations.  
It is useful for learning how Linux kernel modules interact with the kernel and handle syscalls.

---

## Requirements
- Linux kernel headers installed  
- GCC compiler (same version as your kernel build)
- Root privileges (for inserting/removing kernel modules)

---

## Project Structure
```
Linux_kernel_module/
│
├── Makefile
├── syscall_logger.c
├── syscall_logger.ko     # Compiled kernel object file
├── syscall_logger.mod.c
├── syscall_logger.mod.o
├── syscall_logger.o
└── Module.symvers
```

---

## Build Instructions

1. **Open terminal and navigate to the module directory**
   ```bash
   cd Linux_kernel_module/
   ```

2. **Build the kernel module**
   ```bash
   make
   ```
   You should see output similar to:
   ```
   CC [M]  syscall_logger.o
   MODPOST Module.symvers
   CC [M]  syscall_logger.mod.o
   LD [M]  syscall_logger.ko
   BTF [M] syscall_logger.ko
   ```

   > *Note:* You may see a warning about compiler version mismatch — this is safe as long as your GCC version is compatible with the kernel version.

---

## Load the Module

To insert the kernel module:
```bash
sudo insmod syscall_logger.ko
```

Check if the module is successfully loaded:
```bash
lsmod | grep syscall_logger
```

You should see:
```
syscall_logger      12288  0
```

---

## View Kernel Logs

To check log messages from your module:
```bash
dmesg | tail
```

If you encounter:
```
read kernel buffer failed: Operation not permitted
```
Run `dmesg` with `sudo`:
```bash
sudo dmesg | tail
```

---

## Unload the Module

To remove the kernel module:
```bash
sudo rmmod syscall_logger
```

Then verify it’s removed:
```bash
lsmod | grep syscall_logger
```

---

## Common Issues

| Issue | Cause | Solution |
|-------|--------|-----------|
| `Operation not permitted` on `dmesg` | Non-root user | Use `sudo dmesg` |
| `No such file or directory` when building | Missing kernel headers | Install using `sudo apt install linux-headers-$(uname -r)` |
| Compiler version mismatch warning | Different GCC version | Usually safe; match kernel’s GCC version if errors occur |

---

## Notes
- Always build kernel modules against the **current running kernel version**.
- Use **virtual environments** (like WSL or VMware Ubuntu) only if the kernel headers for that environment are installed.
- Avoid loading untrusted kernel modules — they run in **kernel space** and can crash your system.

---
