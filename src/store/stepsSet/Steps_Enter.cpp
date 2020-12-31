/*============================================================================*/
/* Steps_Enter.cpp - enter step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Enter(void)
{
  vAddStep("enterRoutine"_Stz,    &Step::vEnterRoutine);
}

/*----------------------------------------------------------------------------*/
/* enterRoutine -- enter word routine    (this is the does step for a : word) */
/*----------------------------------------------------------------------------*/
void Step::vEnterRoutine(void)
{
  ENTRY();
  
  fpSTEPFN* afpEntryCell =
    (fpSTEPFN*) ((m_aInxPtr - 1)->m_afpStepFn + 1);
  
  m_pReturnStack->vPushFip(m_aInxPtr);
  m_aInxPtr = *(ForthCell**)afpEntryCell;

   if ( (m_iTraceMask & C_TRACE_ROUTINES) )
    Task::vTracePoint(">"_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
