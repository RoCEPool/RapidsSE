/*============================================================================*/
/* Process.cpp -- process utility                                             */
/*============================================================================*/

#include "Resources.h"

extern pthread_key_t giTLS_ThreadKey;

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
  
  exit((int)iExitStatus);
}

/*----------------------------------------------------------------------------*/
/* process exit failure                                                       */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Process::vExitFailure(void)
{
  fprintf(stderr,"Calling exit(%lld)\n",(u8)EXIT_FAILURE);
  
  exit((int)EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* process exit success                                                       */
/*----------------------------------------------------------------------------*/
[[ noreturn ]] void Process::vExitSuccess(void)
{
  fprintf(stderr,"Calling exit(%lld)\n",(i8)EXIT_SUCCESS);
  
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
  
#ifdef WITH_APPLE
  
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
  
#ifdef WITH_APPLE
  
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
  
#ifdef WITH_APPLE
  
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
