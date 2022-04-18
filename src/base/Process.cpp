/*============================================================================*/
/* Process.cpp -- process utility                                             */
/*============================================================================*/

#include "BASE.h"

#ifdef WITH_APPLE
  #include <mach/mach.h>
#endif

#include <signal.h>
#include <sys/resource.h>
#include <fstream>
#include <sys/sysctl.h>

//#include <sys/vmmeter.h>
//#include <sys/limits.h>
//#include <vm/vm_param.h>

extern pthread_key_t giTLS_ThreadKey;
extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_HandlerKey;
extern pthread_key_t giTLS_StepKey;

/*----------------------------------------------------------------------------*/
/* process entry                                                              */
/*----------------------------------------------------------------------------*/
void Process::vEntry(void)
{
}

/*----------------------------------------------------------------------------*/
/* process exit                                                               */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Process::vExit(i8 iExitStatus)
{
  fprintf(stderr,"Calling exit(%lld)\n",iExitStatus);
  
  vThreadCallStackDump();

  exit((int)iExitStatus);
}

/*----------------------------------------------------------------------------*/
/* process exit failure                                                       */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Process::vExitFailure(void)
{
  fprintf(stderr,"Process::vExitFailure()\n");
  
  vThreadCallStackDump();

  fprintf(stderr,"Calling exit(%lld)\n",(u8)EXIT_FAILURE);

 
  exit((int)EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* process exit success                                                       */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Process::vExitSuccess(void)
{
  fprintf(stderr,"Calling exit(%lld)\n",(i8)EXIT_SUCCESS);
  
//  vThreadCallStackDump();
  
  exit((int)EXIT_SUCCESS);
}

/*----------------------------------------------------------------------------*/
/* unrecoverable fatal error                                                  */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Process::vUnrecoverableError(Stz* szErrorMessage,
                                                 Stz* szInfoText)
{
  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  printf("RapidsSE Unrecoverable Error\n");
  printf("  %s\n",szErrorMessage->azChar());
  printf("  %s\n",szInfoText->azChar());
  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

  vThreadCallStackDump();

  Process::vExitFailure();
}


/*----------------------------------------------------------------------------*/
/* get process id                                                             */
/*----------------------------------------------------------------------------*/
i8 Process::iGetPid(void)
{
  i8 iPid = getpid();
  
  return iPid;
}

/*----------------------------------------------------------------------------*/
/* is this an SE process?                                                     */
/*----------------------------------------------------------------------------*/
bool Process::bIsSeProcess(void)
{
  if (C_UNKNOWN != (int)giTLS_ThreadKey)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* kill process                                                               */
/*----------------------------------------------------------------------------*/
bool Process::bKillPid(i8 iPid)
{
  i8 iResult = kill((pid_t)iPid,SIGKILL);
  
  if (! iResult)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* test process id exists                                                     */
/*----------------------------------------------------------------------------*/
bool Process::bTestPid(i8 iPid)
{
  i8 iResult = kill((pid_t)iPid,0);
  
  if (! iResult)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get resident size of process                                               */
/*----------------------------------------------------------------------------*/
i8 Process::iResidentSize(void)
{
  i8 iResident = 0;

  struct rusage UsageInfo;
  if (getrusage(RUSAGE_SELF, &UsageInfo) == 0) {
    iResident = UsageInfo.ru_maxrss;
  } else {
    iResident = 0;  /* Can't access? */
  }
  
#ifdef WITH_APPLE1
  
  // refer: http://web.mit.edu/darwin/src/modules/xnu/osfmk/man/task_info.html
  //        http://web.mit.edu/darwin/src/modules/xnu/osfmk/man/task_basic_info.html

  struct mach_task_basic_info info;
  
  mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
  
  if ( task_info( mach_task_self( ), MACH_TASK_BASIC_INFO,
                 (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
 
    iResident = 0; /* Can't access? */
  else
    iResident = info.resident_size;

#endif
  
  return iResident;
}

/*----------------------------------------------------------------------------*/
/* get virtual size of process                                                */
/*----------------------------------------------------------------------------*/
i8 Process::iVirtualSize(void)
{
  i8 iVirtual = 0;
  
#ifdef WITH_APPLE
  
  // refer: http://web.mit.edu/darwin/src/modules/xnu/osfmk/man/task_info.html
  //        http://web.mit.edu/darwin/src/modules/xnu/osfmk/man/task_basic_info.html

  struct mach_task_basic_info info;
  
  mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
  
  if ( task_info( mach_task_self( ), MACH_TASK_BASIC_INFO,
                 (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
    
    iVirtual = 0;      /* Can't access? */
  else
    iVirtual = info.virtual_size;
  
#endif
  
  return iVirtual;
}

/*----------------------------------------------------------------------------*/
/* get system time of process (in microseconds)                               */
/*----------------------------------------------------------------------------*/
i8 Process::iSystemUsec(void)
{
  i8 iSystem = 0;

  struct rusage UsageInfo;
  if (getrusage(RUSAGE_SELF, &UsageInfo) == 0) {
    iSystem = UsageInfo.ru_stime.tv_sec * C_1M + UsageInfo.ru_stime.tv_usec;
  } else {
    iSystem = 0;  /* Can't access? */
  }
  
#ifdef WITH_APPLE1
  
  struct task_thread_times_info info;
  
  mach_msg_type_number_t infoCount = TASK_THREAD_TIMES_INFO_COUNT;
  
  if ( task_info( mach_task_self( ), TASK_THREAD_TIMES_INFO,
                 (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
    
    iSystem = 0;      /* Can't access? */
  else
    iSystem = info.system_time.seconds * C_1M + info.system_time.microseconds;

#endif
  
  return iSystem;
}

/*----------------------------------------------------------------------------*/
/* get user time of process (in microseconds)                                 */
/*----------------------------------------------------------------------------*/
i8 Process::iUserUsec(void)
{
  i8 iUser = 0;

  struct rusage UsageInfo;
  if (getrusage(RUSAGE_SELF, &UsageInfo) == 0) {
    iUser = UsageInfo.ru_utime.tv_sec * C_1M + UsageInfo.ru_utime.tv_usec;
  } else {
    iUser = 0;  /* Can't access? */
  }


#ifdef WITH_APPLE1
  
  struct task_thread_times_info info;
  
  mach_msg_type_number_t infoCount = TASK_THREAD_TIMES_INFO_COUNT;
  
  if ( task_info( mach_task_self( ), TASK_THREAD_TIMES_INFO,
                 (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
    
    iUser = 0;      /* Can't access? */
  else
    iUser = info.user_time.seconds * C_1M + info.user_time.microseconds;
  
#endif
  
  return iUser;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
