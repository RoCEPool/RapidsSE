/*============================================================================*/
/* OpSys.cpp -- operating system utility                                      */
/*============================================================================*/

#include "Host.h"

static struct utsname gxUtsName;

/*----------------------------------------------------------------------------*/
/* get information about o/s and machine                                      */
/*----------------------------------------------------------------------------*/
void OpSys::vGetInfo(void)
{
  _static_
  
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
  _static_

#ifdef WITH_APPLE
  
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
  static const i8 C_BUFSIZ=512;
  char caBuf[C_BUFSIZ];
  double dFreq;
  i8 iFreq = 0;
  
  // open the /Proc/cpuinfo file
  FILE* aFile = fopen("/proc/cpuinfo", "r");
  if ( !aFile ) {
    vWarn("File::bOpen: fopen(/proc/cpuinfo,r) error; errno=%d"_Stz, errno);
    return 0;
  }
  
  // look for cpu MHz
  while (fgets(caBuf, C_BUFSIZ, aFile) != NULL) {
    if (strstr(caBuf, "cpu MHz") == NULL)
      continue;
    
    char* p = strchr(caBuf,':');
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
  _static_
  
  vTrace("OpSys::vGetInfo:\n"_Stz);
  vTrace("  sysname : %s\n"_Stz,azSysName());
  vTrace("  nodename: %s\n"_Stz,azNodeName());
  vTrace("  release : %s\n"_Stz,azRelease());
  vTrace("  version : %s\n"_Stz,azVersion());
  vTrace("  machine : %s\n"_Stz,azMachine());
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* get utsnamme.machine (Hardware type)                                       */
/*----------------------------------------------------------------------------*/
char* OpSys::azMachine(void)
{
  _static_
  
  return gxUtsName.machine;
}

/*----------------------------------------------------------------------------*/
/* get utsnamme.nodename (Name of this network node)                          */
/*----------------------------------------------------------------------------*/
char* OpSys::azNodeName(void)
{
  _static_
  
  return gxUtsName.nodename;
}

/*----------------------------------------------------------------------------*/
/* get utsnamme.release (Release level)                                       */
/*----------------------------------------------------------------------------*/
char* OpSys::azRelease(void)
{
  _static_
  
  return gxUtsName.release;
}

/*----------------------------------------------------------------------------*/
/* get utsnamme.sysname (Name of OS)                                          */
/*----------------------------------------------------------------------------*/
char* OpSys::azSysName(void)
{
  _static_
  
  return gxUtsName.sysname;
}

/*----------------------------------------------------------------------------*/
/* get utsnamme.version (Version level)                                       */
/*----------------------------------------------------------------------------*/
char* OpSys::azVersion(void)
{
  _static_
  
  return gxUtsName.version;
}


/*----------------------------------------------------------------------------*/
/* largest size (in bytes) core file that may be created                      */
/*----------------------------------------------------------------------------*/
void OpSys::vRLimitCore(i8& nSoft, i8& nHard)
{
  _static_
  
#ifdef WITH_APPLE
  
  struct rlimit xRLimit;
  
  int iRetVal = getrlimit(RLIMIT_CORE,&xRLimit);
  
  nSoft = xRLimit.rlim_cur;
  nHard = xRLimit.rlim_max;

#else
  nSoft = 0;
  nHard = 0;
#endif
}

/*----------------------------------------------------------------------------*/
/* maximum amount of cpu time (in seconds) to be used by each process         */
/*----------------------------------------------------------------------------*/
void OpSys::vRLimitCpu(i8& nSoft, i8& nHard)
{
  _static_

#ifdef WITH_APPLE

  struct rlimit xRLimit;

  int iRetVal = getrlimit(RLIMIT_CPU,&xRLimit);

  nSoft = xRLimit.rlim_cur;
  nHard = xRLimit.rlim_max;

#else
  nSoft = 0;
  nHard = 0;
#endif
}

/*----------------------------------------------------------------------------*/
/* maximum size (in bytes) of the data segment for a process                  */
/*----------------------------------------------------------------------------*/
void OpSys::vRLimitData(i8& nSoft, i8& nHard)
{
  _static_

#ifdef WITH_APPLE

  struct rlimit xRLimit;

  int iRetVal = getrlimit(RLIMIT_DATA,&xRLimit);

  nSoft = xRLimit.rlim_cur;
  nHard = xRLimit.rlim_max;

#else
  nSoft = 0;
  nHard = 0;
#endif
}

/*----------------------------------------------------------------------------*/
/* largest size (in bytes) file that may be created                           */
/*----------------------------------------------------------------------------*/
void OpSys::vRLimitFileSize(i8& nSoft, i8& nHard)
{
  _static_

#ifdef WITH_APPLE

  struct rlimit xRLimit;

  int iRetVal = getrlimit(RLIMIT_FSIZE,&xRLimit);

  nSoft = xRLimit.rlim_cur;
  nHard = xRLimit.rlim_max;

#else
  nSoft = 0;
  nHard = 0;
#endif
}

/*----------------------------------------------------------------------------*/
/* maximum size (in bytes) which a process may lock into memory               */
/*----------------------------------------------------------------------------*/
void OpSys::vRLimitMemoryLocked(i8& nSoft, i8& nHard)
{
  _static_

#ifdef WITH_APPLE

  struct rlimit xRLimit;

  int iRetVal = getrlimit(RLIMIT_MEMLOCK,&xRLimit);

  nSoft = xRLimit.rlim_cur;
  nHard = xRLimit.rlim_max;

#else
  nSoft = 0;
  nHard = 0;
#endif
}

/*----------------------------------------------------------------------------*/
/* maximum number of open files for this process                              */
/*----------------------------------------------------------------------------*/
void OpSys::vRLimitOpenFiles(i8& nSoft, i8& nHard)
{
  _static_

#ifdef WITH_APPLE

  struct rlimit xRLimit;

  int iRetVal = getrlimit(RLIMIT_NOFILE,&xRLimit);

  nSoft = xRLimit.rlim_cur;
  nHard = xRLimit.rlim_max;

#else
  nSoft = 0;
  nHard = 0;
#endif
}

/*----------------------------------------------------------------------------*/
/* maximum number of simultaneous processes for this user id                  */
/*----------------------------------------------------------------------------*/
void OpSys::vRLimitProcesses(i8& nSoft, i8& nHard)
{
  _static_

#ifdef WITH_APPLE

  struct rlimit xRLimit;

  int iRetVal = getrlimit(RLIMIT_NPROC,&xRLimit);

  nSoft = xRLimit.rlim_cur;
  nHard = xRLimit.rlim_max;

#else
  nSoft = 0;
  nHard = 0;
#endif
}

/*----------------------------------------------------------------------------*/
/* maximum size (in bytes) to which a process's resident set size may grow    */
/*----------------------------------------------------------------------------*/
void OpSys::vRLimitRss(i8& nSoft, i8& nHard)
{
  _static_

#ifdef WITH_APPLE

  struct rlimit xRLimit;

  int iRetVal = getrlimit(RLIMIT_RSS,&xRLimit);

  nSoft = xRLimit.rlim_cur;
  nHard = xRLimit.rlim_max;

#else
  nSoft = 0;
  nHard = 0;
#endif
}

/*----------------------------------------------------------------------------*/
/* maximum size (in bytes) of the stack segment for a process                 */
/*----------------------------------------------------------------------------*/
void OpSys::vRLimitStack(i8& nSoft, i8& nHard)
{
  _static_

#ifdef WITH_APPLE

  struct rlimit xRLimit;

  int iRetVal = getrlimit(RLIMIT_STACK,&xRLimit);

  nSoft = xRLimit.rlim_cur;
  nHard = xRLimit.rlim_max;

#else
  nSoft = 0;
  nHard = 0;
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
