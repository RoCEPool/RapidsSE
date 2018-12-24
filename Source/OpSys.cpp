/*============================================================================*/
/* OpSys.cpp -- operating system utility                                      */
/*============================================================================*/

#include "System.h"

static struct utsname gxUtsName;

/*----------------------------------------------------------------------------*/
/* get information about o/s and machine                                      */
/*----------------------------------------------------------------------------*/
void OpSys::vGetInfo(void)
{
  int iResult = uname(&gxUtsName);
  
  if (0 == iResult)
    return;
 
  vWarn("OpSys::vGetInfo: uname() error; errno=%d"_Stz,errno);  
}

/*----------------------------------------------------------------------------*/
/* get scheduler period in microseconds                                       */
/*----------------------------------------------------------------------------*/
i8 OpSys::iSchedulerPeriod_us(void)
{
  
#ifdef __APPLE__
  
  struct clockinfo xKernelClockRate;
  
  int iMib[2];
  size_t nSize;
  
  iMib[0] = CTL_KERN;
  iMib[1] = KERN_CLOCKRATE;
  nSize = sizeof xKernelClockRate;
  sysctl(iMib, 2, &xKernelClockRate, &nSize, NULL, 0);
  
  //vTrace("Kern ClockRate %d\n"_Stz,clockrate.hz);
  
  return xKernelClockRate.hz;
  
#else
  const i8 C_BUFSIZ=512;
  char buf[C_BUFSIZ];
  double dFreq;
  i8 iFreq = 0;
  
  // open the /Proc/cpuinfo file
  FILE* aFile = fopen("/proc/cpuinfo", "r");
  if ( !aFile ) {
    vWarn("File::bOpen: fopen(/proc/cpuinfo,r) error; errno=%d"_Stz, errno);
    return 0;
  }
  
  // look for cpu MHz
  while (fgets(buf, C_BUFSIZ, aFile) != NULL) {
    if (strstr(buf, "cpu MHz") == NULL)
      continue;
    
    char* p = strchr(buf,':');
    if (p == NULL)
      continue;
    p++;
    while (isspace(*p))
      p++;
    sscanf(p,"%lf", &dFreq);
    iFreq = static_cast<i8>(dFreq * (double)C_1M);
  }
  
  //vTrace("Clock frequency: %lld\n"_Stz, iFreq);
  
  return iFreq;
  
#endif
  
}

/*----------------------------------------------------------------------------*/
/* trace utsname information                                                  */
/*----------------------------------------------------------------------------*/
void OpSys::vTraceInfo(void)
{
  vTrace("OpSys::vGetInfo:\n"_Stz);
  vTrace("  sysname : %s\n"_Stz,aSysName());
  vTrace("  nodename: %s\n"_Stz,aNodeName());
  vTrace("  release : %s\n"_Stz,aRelease());
  vTrace("  version : %s\n"_Stz,aVersion());
  vTrace("  machine : %s\n"_Stz,aMachine());
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* get utsnamme.machine (Hardware type)                                       */
/*----------------------------------------------------------------------------*/
char* OpSys::aMachine(void)
{
  return gxUtsName.machine;
}

/*----------------------------------------------------------------------------*/
/* get utsnamme.nodename (Name of this network node)                          */
/*----------------------------------------------------------------------------*/
char* OpSys::aNodeName(void)
{
  return gxUtsName.nodename;
}

/*----------------------------------------------------------------------------*/
/* get utsnamme.release (Release level)                                       */
/*----------------------------------------------------------------------------*/
char* OpSys::aRelease(void)
{
  return gxUtsName.release;
}

/*----------------------------------------------------------------------------*/
/* get utsnamme.sysname (Name of OS)                                          */
/*----------------------------------------------------------------------------*/
char* OpSys::aSysName(void)
{
  return gxUtsName.sysname;
}

/*----------------------------------------------------------------------------*/
/* get utsnamme.version (Version level)                                       */
/*----------------------------------------------------------------------------*/
char* OpSys::aVersion(void)
{
  return gxUtsName.version;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
