/*============================================================================*/
/* Steps_Exit.cpp - exit step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// exit: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddSteps_Exit(void)
{
  vAddStep("exit"_Stz,            &vExit,             false);
}

/*----------------------------------------------------------------------------*/
/* exit -- exit from routine                                                  */
/*----------------------------------------------------------------------------*/
void Task::vExit(MP_Task mpTask)
{
  ENTRY("vExit"_Stz);
  
  //vTrace("Exit: return stack %lld\n"_Stz, mpTask->m_pReturnStack->iValues());
  
  if ( (mpTask->m_iTracing & C_TRACE_ROUTINES) )
    mpTask->vTracePoint("<"_Stz);
  
  if (mpTask->m_pLocalsFrame &&
      mpTask->m_pLocalsFrame->pStackPtr() ==
      (void*)mpTask->m_pReturnStack->pTopValue())
    mpTask->vLocalsPop();
  
  Cell* pCell;
  if ( ! mpTask->m_pReturnStack->bPop(pCell) )
    THROW_INVALID_RSTACK("exit"_Stz);
  
  mpTask->IP.m_pCell = pCell;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
