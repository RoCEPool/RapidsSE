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
void Step::vEnterObject(void)
{
  ENTRY(); // ENTRY("vEnterObject"_Stz);
  
  StepFp* afpEntryCell = (m_aInxPtr - 1)->m_afpIntrinsic + 1;

  m_pReturnStack->vPushCell(m_aInxPtr);
  m_aInxPtr = *(Cell**)afpEntryCell;
  
  Word* pWord = pAdd<Word*>((Object*)afpEntryCell,
                            -( sizeof(Word)
                               - sizeof(Cell*)
                               - sizeof(Wsp_t<C_WSP_WORD>)
                               - sizeof(Footer)
                             )
                            );

  ASSERT(WK_OBJECT == pWord->eKind());
  
  Object* pObject = (Object*)pWord->aFirstCell();
  m_pDataStack->vPushPointer(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enterRoutine -- enter word routine    (this is the does step for a : word) */
/*----------------------------------------------------------------------------*/
void Step::vEnterRoutine(void)
{
  ENTRY(); // ENTRY("vEnterRoutine"_Stz);
  
  StepFp* afpEntryCell = (m_aInxPtr - 1)->m_afpIntrinsic + 1;
  
  m_pReturnStack->vPushCell(m_aInxPtr);
  m_aInxPtr = *(Cell**)afpEntryCell;

   if ( (m_pTask->m_iTraceMask & C_TRACE_ROUTINES) )
    m_pTask->vTracePoint(">"_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
