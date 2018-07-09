
#ifndef INC_APPCLI_H_
#define INC_APPCLI_H_

#define SUCCESS  0
#define FAILURE -1
 
int openFile(int argc,char *argv[]);
int closeFile(int argc,char *argv[]);
int ioctlGetId(int argc,char *argv[]);
int ioctlGetMaxIrqNo(int argc,char *argv[]);
int ioctlGetIrqStatus(int argc,char *argv[]);
int ioctlGetIrqType(int argc,char *argv[]);
int quitApp(int argc,char *argv[]);
int dispHlp(int argc,char *argv[]);

#endif /* INC_APPCLI_H_ */