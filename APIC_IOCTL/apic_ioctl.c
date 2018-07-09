#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "apic_ioctl.h"

/* These are offset values meant to be passed as parameters to IOREGSEL */
uint32_t redirectionTable[] =
{
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
  0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,
  0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21,
  0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
  0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33,
  0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
  0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F
};

void *ioBase = NULL, *ioRegSel = NULL, *ioWin = NULL;

static int inuse = 0;

static int findIrqStatus(int irqNo)
{
  uint32_t regLow = 0, regHigh = 0, idx = 0;

  /* Index to select suitable offset from the redirection table */  
  idx = irqNo * 2;
  
  /* Each Interrupt is associated with two 32 bit registers (High and Low) */
  /* Each high and low register has an associated redirection table offset */
  iowrite32(redirectionTable[idx], ioRegSel);
  regLow  = ioread32(ioWin);
  idx++;
  iowrite32(redirectionTable[idx], ioRegSel);
  regHigh = ioread32(ioWin);

  /* Check the IRQ status bit and return result */
  if((regLow >> 12) & 1)     //IRQ Pending
    return 1;
  return 0;                  //IRQ Idle
}

static int findIrqType(int irqNo)
{
  uint32_t regLow = 0, regHigh = 0, idx = 0;

  /* Index to select suitable offset from the redirection table */ 
  idx = irqNo * 2;
  
  /* Each Interrupt is associated with two 32 bit registers (High and Low) */
  /* Each high and low register has an associated redirection table offset */
  iowrite32(redirectionTable[idx], ioRegSel);
  regLow  = ioread32(ioWin);
  idx++;
  iowrite32(redirectionTable[idx], ioRegSel);
  regHigh = ioread32(ioWin);

  /* Check the IRQ type bit and return result */
  if((regLow >> 15) & 1)    //IRQ Level Sensitive
    return 1;
  return 0;                 //IRQ Edge Sensitive
}

static int apic_dev_open(struct inode *inode, struct file *file)
{
  if(inuse)
  {
    pr_err("Device Busy %s\r\n",APIC_DEVICE_NAME);
    return -EBUSY;
  }
  inuse = 1;
  pr_info("Open Operation Invoked\r\n");
  return SUCCESS;
}

static int apic_dev_release(struct inode *inode, struct file *file)
{
  inuse = 0;
  pr_info("Release Operation Invoked\r\n");
  return SUCCESS;
}

static long apic_dev_ioctl(struct file *file,unsigned int cmd, unsigned long arg)
{
  int temp = 0;

  apic_dev *ptr = (apic_dev *)arg;

  switch(cmd)
  {
    case APIC_GETID:
      iowrite32(0x00, ioRegSel);
      temp = (ioread32(ioWin) >> 24) & 0x0F;
      copy_to_user(&ptr->apicId, &temp, sizeof(int));
      break;

    case APIC_GETIRQS:
      iowrite32(0x01, ioRegSel);
      temp = ioread32(ioWin) & 0xFF;
      copy_to_user(&ptr->apicVer, &temp, sizeof(int));

      /* Mask Rest and Access Bit 16-23 */
      temp = (ioread32(ioWin) >> 16) & 0x00FF;
      temp = temp + 1;
      copy_to_user(&ptr->apicMaxIrqNo, &temp, sizeof(int));
      break;
 
    case APIC_GETIRQSTATUS:
      if(((ptr->queryIrqNo) < 0) || ((ptr->queryIrqNo) > 23))
        return -EINVAL;
      temp = findIrqStatus(ptr->queryIrqNo);
      copy_to_user(&ptr->queryIrqStatus, &temp, sizeof(int));
      break;

    case APIC_GETIRQTYPE:
      if(((ptr->queryIrqNo) < 0) || ((ptr->queryIrqNo) > 23))
        return -EINVAL;
      temp = findIrqType(ptr->queryIrqNo);
      copy_to_user(&ptr->queryIrqType, &temp, sizeof(int));
      break;
  }  
  pr_info("IOCTL Operation Invoked\r\n");
  return SUCCESS;
}

static struct file_operations apic_dev_fops = {
  .owner          = THIS_MODULE,
  .open           = apic_dev_open,
  .release        = apic_dev_release,
  .unlocked_ioctl = apic_dev_ioctl
};

static struct miscdevice apic_dev_misc = {
  .minor = MISC_DYNAMIC_MINOR,
  .name  = APIC_DEVICE_NAME,
  .fops  = &apic_dev_fops
};

static int __init apic_drv_init(void)
{
  int retval;

  /* 
  ioremap - remaps a physical address range into the processor's virtual address 
            space (kernel's linear address), making it available to the kernel
            IOAPIC_BASE: Physical address of IOAPIC
            SIZE: size of the resource to map 
  */
  ioBase = ioremap(IOAPIC_BASE, PAGE_SIZE);

  /* As per IOAPIC Datasheet 0x00 is I/O REGISTER SELECT of size 32 bits */
  ioRegSel = (ioBase + 0x00);

  /* As per IOAPIC Datasheet 0x10 is I/O WINDOW REGISTER of size 32 bits */
  ioWin    = (ioBase + 0x10);

  retval = misc_register(&apic_dev_misc);
  if(retval < 0)
  {
    pr_err("Device Registration Failed with Minor Number %d\r\n",apic_dev_misc.minor);
    return retval;
  }
  pr_info("Device Registered : %s with Minor Number : %d\r\n",APIC_DEVICE_NAME, apic_dev_misc.minor);

  return SUCCESS;
}

static void __exit apic_drv_exit(void)
{
  misc_deregister(&apic_dev_misc);

  /* Unmap the allocated address range in kernel */
  iounmap(ioBase);

  pr_info("Device Unregistered : %s with Minor Number : %d\r\n",APIC_DEVICE_NAME, apic_dev_misc.minor);
}

module_init(apic_drv_init);
module_exit(apic_drv_exit);

MODULE_AUTHOR("debmalyasarkar1@gmail.com");
MODULE_DESCRIPTION("APIC Driver With IOCTL Commands");
MODULE_LICENSE("GPL");
