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
    (fpSTEPFN*) ((m_pTask->m_aInxPtr - 1)->m_afpObjectFn + 1);
  
  m_pTask->m_pReturnStack->vPushFip(m_pTask->m_aInxPtr);
  m_pTask->m_aInxPtr = *(ForthCell**)afpEntryCell;

   if ( (m_pTask->m_iTraceMask & C_TRACE_ROUTINES) )
    m_pTask->vTracePoint(">"_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
