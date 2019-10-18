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
void Step::vEnterObject(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEnterObject"_Stz);
  
  StepFp* afpEntryCell = (mpTask->m_pForth->m_aInxPtr - 1)->m_afpIntrinsic + 1;

  mpTask->m_pForth->m_pReturnStack->vPushCell(mpTask->m_pForth->m_aInxPtr);
  mpTask->m_pForth->m_aInxPtr = *(Cell**)afpEntryCell;
  
  Word* pWord = pAdd<Word*>((Object*)afpEntryCell,
                            -( sizeof(Word)
                               - sizeof(Cell*)
                               - sizeof(Wsp_t<C_WSP_WORD>)
                               - sizeof(Footer)
                             )
                            );

  ASSERT(WK_OBJECT == pWord->eKind());
  
  Object* pObject = (Object*)pWord->aFirstCell();
  mpTask->m_pForth->m_pDataStack->vPushPointer(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enterRoutine -- enter word routine    (this is the does step for a : word) */
/*----------------------------------------------------------------------------*/
void Step::vEnterRoutine(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vEnterRoutine"_Stz);
  
  StepFp* afpEntryCell = (mpTask->m_pForth->m_aInxPtr - 1)->m_afpIntrinsic + 1;
  
  mpTask->m_pForth->m_pReturnStack->vPushCell(mpTask->m_pForth->m_aInxPtr);
  mpTask->m_pForth->m_aInxPtr = *(Cell**)afpEntryCell;

   if ( (mpTask->m_iTracing & C_TRACE_ROUTINES) )
    mpTask->vTracePoint(">"_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
