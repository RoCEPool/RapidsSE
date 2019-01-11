/*============================================================================*/
/* Steps_Exit.cpp - exit step group                                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Exit(void)
{
  vAddStep("exit"_Stz,            &Step::vExit);
}

/*----------------------------------------------------------------------------*/
/* exit -- exit from routine                                                  */
/*----------------------------------------------------------------------------*/
void Step::vExit(MP_Task mpTask)
{
  ENTRY("vExit"_Stz);
  
  //vTrace("Exit: return stack %lld\n"_Stz, mpTask->m_pReturnStack->iValues());
  
  if ( (mpTask->m_iTracing & C_TRACE_ROUTINES) )
    mpTask->vTracePoint("<"_Stz);
  
  if (mpTask->m_pLocalsFrame &&
      mpTask->m_pLocalsFrame->pStackPtr() ==
      (void*)mpTask->m_pReturnStack->aPeekValue())
    mpTask->vLocalsPop();
  
  Cell* aCell;
  if ( ! mpTask->m_pReturnStack->bPop(aCell) )
    THROW_INVALID_RSTACK("exit"_Stz);
  
  mpTask->IP.m_aCell = aCell;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
