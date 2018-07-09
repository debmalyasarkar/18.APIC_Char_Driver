#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>

#define IOAPIC_BASE 0xFEC00000

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

static struct kobject *apic_kobj;

static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
  int retval,temp = 0;

  pr_info("Invoked %s\r\n",__func__);
  
  iowrite32(0x00, ioRegSel);
  temp = (ioread32(ioWin) >> 24) & 0x0F;

  retval = sprintf(buf, "APIC ID : %d\n",temp);

  return retval;
}

static ssize_t maxirq_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
  int retval,temp = 0;

  pr_info("Invoked %s\r\n",__func__);

  iowrite32(0x01, ioRegSel);
  /* Mask Rest and Access Bit 16-23 */
  temp = (ioread32(ioWin) >> 16) & 0x00FF;
  temp = temp + 1;

  retval = sprintf(buf, "APIC MAX IRQ NO : %d\n", temp);

  return retval;
}

static ssize_t irqstatus_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
  uint32_t retval, regLow = 0, regHigh = 0, idx = 0;
 
  pr_info("Invoked %s\r\n",__func__);
  
  /* Here Names are Directly Numbers so directly we are converting. */
  /* If names are strings then we need to parse and calculate the file name. */
  sscanf(attr->attr.name,"%d",&idx);
  
  idx = idx * 2;

  /* Each Interrupt is associated with two 32 bit registers (High and Low) */
  /* Each high and low register has an associated redirection table offset */
  iowrite32(redirectionTable[idx], ioRegSel);
  regLow  = ioread32(ioWin);
  idx++;
  iowrite32(redirectionTable[idx], ioRegSel);
  regHigh = ioread32(ioWin);

  retval = sprintf(buf, "IRQ NO : %d, STATUS : %08X%08X\n", idx/2, regHigh, regLow);

  return retval;
}

/* Linking routines to particular entry */
static struct kobj_attribute id_attribute         = 
  __ATTR(id, 0660, id_show, NULL);

static struct kobj_attribute maxirq_attribute     = 
  __ATTR(maxirq, 0660, maxirq_show, NULL);

static struct kobj_attribute irqstatus1_attribute =
  __ATTR(1,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus2_attribute =
  __ATTR(2,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus3_attribute =
  __ATTR(3,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus4_attribute =
  __ATTR(4,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus5_attribute =
  __ATTR(5,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus6_attribute =
  __ATTR(6,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus7_attribute =
  __ATTR(7,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus8_attribute =
  __ATTR(8,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus9_attribute =
  __ATTR(9,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus10_attribute =
  __ATTR(10,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus11_attribute =
  __ATTR(11,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus12_attribute =
  __ATTR(12,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus13_attribute =
  __ATTR(13,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus14_attribute =
  __ATTR(14,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus15_attribute =
  __ATTR(15,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus16_attribute =
  __ATTR(16,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus17_attribute =
  __ATTR(17,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus18_attribute =
  __ATTR(18,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus19_attribute =
  __ATTR(19,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus20_attribute =
  __ATTR(20,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus21_attribute =
  __ATTR(21,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus22_attribute =
  __ATTR(22,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus23_attribute =
  __ATTR(23,0660, irqstatus_show, NULL);

static struct kobj_attribute irqstatus24_attribute =
  __ATTR(24,0660, irqstatus_show, NULL);


/* Create the attribute structure for all sub files */
static struct attribute *attrs[] = {
  &id_attribute.attr,
  &maxirq_attribute.attr,
  &irqstatus1_attribute.attr,
  &irqstatus2_attribute.attr,
  &irqstatus3_attribute.attr,
  &irqstatus4_attribute.attr,
  &irqstatus5_attribute.attr,
  &irqstatus6_attribute.attr,
  &irqstatus7_attribute.attr,
  &irqstatus8_attribute.attr,
  &irqstatus9_attribute.attr,
  &irqstatus10_attribute.attr,
  &irqstatus11_attribute.attr,
  &irqstatus12_attribute.attr,
  &irqstatus13_attribute.attr,
  &irqstatus14_attribute.attr,
  &irqstatus15_attribute.attr,
  &irqstatus16_attribute.attr,
  &irqstatus17_attribute.attr,
  &irqstatus18_attribute.attr,
  &irqstatus19_attribute.attr,
  &irqstatus20_attribute.attr,
  &irqstatus21_attribute.attr,
  &irqstatus22_attribute.attr,
  &irqstatus23_attribute.attr,
  &irqstatus24_attribute.attr,
  NULL,                /* NULL termination required for the list of attributes */
};

/* Initialize the attribute group for the /sys entry */
static struct attribute_group attr_group = {
  .attrs = attrs,
};

static int __init apic_init(void)
{
  int retval;

  pr_info("Invoked %s\r\n",__func__);

  ioBase = ioremap(IOAPIC_BASE, PAGE_SIZE);

  ioRegSel = (ioBase + 0x00);

  ioWin    = (ioBase + 0x10);

  /* Create a struct kobject dynamically and register it with sysfs */
  apic_kobj = kobject_create_and_add("apic",NULL);
  if(!apic_kobj)
    return -ENOMEM;

  /* Create the files associated with this kobject */
  retval = sysfs_create_group(apic_kobj, &attr_group);
  if(retval)
    kobject_put(apic_kobj);
  
  return retval;
}

static void __exit apic_exit(void)
{  
  pr_info("Invoked %s\r\n",__func__);

  /* Remove sysfs entry */
  kobject_put(apic_kobj);

  iounmap(ioBase);
}

/* Register the names of custom entry and exit routines */
module_init(apic_init);
module_exit(apic_exit);

/* Comments which are retained in code */
MODULE_AUTHOR("debmalyasarkar1@gmail.com");
MODULE_DESCRIPTION("APIC Driver using SysFs");
MODULE_LICENSE("GPL");

