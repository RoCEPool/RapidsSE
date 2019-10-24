/*============================================================================*/
/* Steps_Process.cpp - process step group                                     */
/*============================================================================*/

#include "Steps.h"

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
void Step::vProcessGetPid(void)
{
ENTRY("vProcessGetPid"_Stz);
  
  i8 iPid = Process::iGetPid();
  
  m_pDataStack->vPushI8(iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process::quit -- process exits early                                        */
/*----------------------------------------------------------------------------*/
void Step::vProcessQuit(void)
{
  ENTRY("vProcessQuit"_Stz);
  
//  vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
//         "Process quitting\n"_Stz);
  
  Process::vExitFailure();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process.testpid -- test pid on stack                                       */
/*----------------------------------------------------------------------------*/
void Step::vProcessTestPid(void)
{
ENTRY("vProcessTestPid"_Stz);

  i8 iPid;
  if ( ! m_pDataStack->bPopI8(iPid) )
    THROW_DSTACK();
  
  i8 iExists = Process::bTestPid(iPid);
  
  m_pDataStack->vPushI8(iExists);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
