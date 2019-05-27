/*============================================================================*/
/* Steps_Process.cpp - process step group                                     */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Process(void)
{
  vAddStep("process.getPid"_Stz,  &Step::vProcessGetPid);
  vAddStep("process::quit"_Stz,    &Step::vProcessQuit);
  vAddStep("process.testPid"_Stz, &Step::vProcessTestPid);
}

/*----------------------------------------------------------------------------*/
/* process.getpid -- get pid of this process                                  */
/*----------------------------------------------------------------------------*/
void Step::vProcessGetPid(MP_Task mpTask)
{
ENTRY("vProcessGetPid"_Stz);
  
  i8 iPid = Process::iGetPid();
  
  mpTask->m_pDataStack->vPushI8(iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process::quit -- process exits early                                        */
/*----------------------------------------------------------------------------*/
void Step::vProcessQuit(MP_Task mpTask)
{
  ENTRY("vProcessQuit"_Stz);
  
  vFatal("Process quitting\n"_Stz);
  
  Process::vExit(EXIT_FAILURE);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process.testpid -- test pid on stack                                       */
/*----------------------------------------------------------------------------*/
void Step::vProcessTestPid(MP_Task mpTask)
{
ENTRY("vProcessTestPid"_Stz);

  i8 iPid;
  if ( ! mpTask->m_pDataStack->bPop(iPid) )
    THROW_INVALID_DSTACK("process.testpid"_Stz);
  
  i8 iExists = Process::bTestPid(iPid);
  
  mpTask->m_pDataStack->vPushI8(iExists);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
