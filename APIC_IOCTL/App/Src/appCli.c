#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <cli.h>
#include <errno.h>
#include <appCli.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <../../apic_ioctl.h>

#define DEVICE_FILE_NAME "/dev/apicDevice" 
#define MAX 10

int fd;

apic_dev apicObj;

extern int quitFlag;

int openFile(int argc,char *argv[])
{
  fd = open(DEVICE_FILE_NAME, O_RDWR);
  if(fd < 0)
  {
    perror("Open Failed : ");
    return FAILURE;
  }
  printf("File Open Success\n");
  return SUCCESS;
}

int closeFile(int argc,char *argv[])
{
  if(0 > close(fd))
  {
    perror("Close Failed : ");
    return FAILURE;
  }
  printf("File Close Success\n");
  return SUCCESS;
}

int ioctlGetId(int argc,char *argv[])
{
  int retval;

  retval = ioctl(fd, APIC_GETID, (apic_dev *)&apicObj);
  if(retval < 0)
  {
    perror("IOCTL Failed : ");
    return FAILURE;
  }
  printf("File IOCTL Success\n");
  
  printf("APIC ID      = %x\n",apicObj.apicId);
  return 0;
}

int ioctlGetMaxIrqNo(int argc,char *argv[])
{
  int retval;
  
  retval = ioctl(fd, APIC_GETIRQS, (apic_dev *)&apicObj);
  if(retval < 0)
  {
    perror("IOCTL Failed : ");
    return FAILURE;
  }
  printf("File IOCTL Success\n");

  printf("APIC Ver     = %x\n",apicObj.apicVer);
  printf("APIC MaxIRQs = %d\n",apicObj.apicMaxIrqNo);
  return 0;
}

int ioctlGetIrqStatus(int argc,char *argv[])
{
  int retval;
 
  if(argc != 2)
  {
    printf("Usage <Cmd> <IRQ Number>\n");
    return FAILURE;
  }
  sscanf(argv[1],"%d",&apicObj.queryIrqNo);

  retval = ioctl(fd, APIC_GETIRQSTATUS, (apic_dev *)&apicObj);
  if(retval < 0)
  {
    perror("IOCTL Failed : ");
    return FAILURE;
  }
  printf("File IOCTL Success\n");
  printf("APIC IRQ %d Status %d\n",apicObj.queryIrqNo,apicObj.queryIrqStatus);
  return 0;
}

int ioctlGetIrqType(int argc,char *argv[])
{
  int retval;

  if(argc != 2)
  {
    printf("Usage <Cmd> <IRQ Number>\n");
    return FAILURE;
  }
  sscanf(argv[1],"%d",&apicObj.queryIrqNo);

  retval = ioctl(fd, APIC_GETIRQTYPE, (apic_dev *)&apicObj);
  if(retval < 0)
  {
    perror("IOCTL Failed : ");
    return FAILURE;
  }
  printf("File IOCTL Success\n");
  printf("APIC IRQ %d Type %d\n",apicObj.queryIrqNo,apicObj.queryIrqType);
  return 0;
}

int quitApp(int argc,char *argv[])
{
  quitFlag = 1;
  return 0;
}

int dispHlp(int argc,char *argv[])
{
  int ii;
  for(ii = 0; ii < (sizeof(commandTable)/sizeof(cmdFun_t)); ii++)
    printf("%s\t : %s\n",commandTable[ii].cmd,commandTable[ii].hlpStr);
  printf("\n");
  return 0;
}
