/*============================================================================*/
/* Steps_Enter.cpp - enter step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Enter(void)
{
  vAddStep("enterObject"_Stz,     &Step::vEnterObject);
  vAddStep("enterRoutine"_Stz,    &Step::vEnterRoutine);
}

/*----------------------------------------------------------------------------*/
/* enterObject -- enter object routine   (this is the does step for a object) */
/*----------------------------------------------------------------------------*/
void Step::vEnterObject(Forth* pForth)
{
  ENTRY("vEnterObject"_Stz);
  
  StepFp* afpEntryCell = (pForth->m_pTask->m_pForth->m_aInxPtr - 1)->m_afpIntrinsic + 1;

  pForth->m_pReturnStack->vPushCell(pForth->m_pTask->m_pForth->m_aInxPtr);
  pForth->m_pTask->m_pForth->m_aInxPtr = *(Cell**)afpEntryCell;
  
  Word* pWord = pAdd<Word*>((Object*)afpEntryCell,
                            -( sizeof(Word)
                               - sizeof(Cell*)
                               - sizeof(Wsp_t<C_WSP_WORD>)
                               - sizeof(Footer)
                             )
                            );

  ASSERT(WK_OBJECT == pWord->eKind());
  
  Object* pObject = (Object*)pWord->aFirstCell();
  pForth->m_pDataStack->vPushPointer(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enterRoutine -- enter word routine    (this is the does step for a : word) */
/*----------------------------------------------------------------------------*/
void Step::vEnterRoutine(Forth* pForth)
{
  ENTRY("vEnterRoutine"_Stz);
  
  StepFp* afpEntryCell = (pForth->m_pTask->m_pForth->m_aInxPtr - 1)->m_afpIntrinsic + 1;
  
  pForth->m_pReturnStack->vPushCell(pForth->m_pTask->m_pForth->m_aInxPtr);
  pForth->m_pTask->m_pForth->m_aInxPtr = *(Cell**)afpEntryCell;

   if ( (pForth->m_pTask->m_iTracing & C_TRACE_ROUTINES) )
    pForth->m_pTask->vTracePoint(">"_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
