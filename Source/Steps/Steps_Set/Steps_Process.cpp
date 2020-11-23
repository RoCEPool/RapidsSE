/*============================================================================*/
/* Steps_Process.cpp - process step group                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Process(void)
{
  vAddStep("process::getPid"_Stz,  &Step::vProcessGetPid);
  vAddStep("process::quit"_Stz,    &Step::vProcessQuit);
  vAddStep("process::testPid"_Stz, &Step::vProcessTestPid);
}

/*----------------------------------------------------------------------------*/
/* process::getPid -- get pid of this process                                  */
/*----------------------------------------------------------------------------*/
void Step::vProcessGetPid(void)
{
  ENTRY();
  
  i8 iPid = Process::iGetPid();
  
  m_pDataStack->vPushInt(iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process::quit -- process exits early                                        */
/*----------------------------------------------------------------------------*/
void Step::vProcessQuit(void)
{
  ENTRY();
  
//  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
//         "Process quitting\n"_Stz);
  
  Process::vExitFailure();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*process::testPid -- test pid on stack                                       */
/*----------------------------------------------------------------------------*/
void Step::vProcessTestPid(void)
{
  ENTRY();

  i8 iPid = m_pDataStack->iPopInt(__func__);
  
  i8 iExists = Process::bTestPid(iPid);
  
  m_pDataStack->vPushInt(iExists);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
