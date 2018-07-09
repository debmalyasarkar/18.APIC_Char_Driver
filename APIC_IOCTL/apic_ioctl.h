
typedef struct apic_dev_t
{
  int apicId;
  int apicVer;
  int apicMaxIrqNo;
  int queryIrqNo;
  int queryIrqStatus;
  int queryIrqType;
}apic_dev;

#define APIC_DEVICE_NAME "apicDevice"

#define IOAPIC_BASE 0xFEC00000

#define SUCCESS 0

#define APIC_MAGIC_NO 'A'

#define APIC_GETID        _IOR(APIC_MAGIC_NO,1,apic_dev *)  /* Returns Identification Number of APIC */
#define APIC_GETIRQS      _IOR(APIC_MAGIC_NO,2,apic_dev *)  /* Returns Number of IRQ Pins on APIC */
#define APIC_GETIRQSTATUS _IOR(APIC_MAGIC_NO,3,apic_dev *)  /* Returns IRQ Pin Status (Enabled, Disabled) */
#define APIC_GETIRQTYPE   _IOR(APIC_MAGIC_NO,4,apic_dev *)  /* Returns IRQ Pin Signal Type */

