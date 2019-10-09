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
void Step::vEnterObject(MP_Task mpTask)
{
  ENTRY("vEnterObject"_Stz);
  
  StepFp* afpEntryCell = (mpTask->m_aFipCell - 1)->m_afpStep + 1;

  mpTask->m_pReturnStack->vPushCell(mpTask->m_aFipCell);
  mpTask->m_aFipCell = *(Cell**)afpEntryCell;
  
  Word* pWord = pAdd<Word*>((Object*)afpEntryCell,
                            -( sizeof(Word)
                               - sizeof(Cell*)
                               - sizeof(Wsp_t<C_WSP_WORD>)
                               - sizeof(Footer)
                             )
                            );

  ASSERT(WK_OBJECT == pWord->eWord());
  
  Object* pObject = (Object*)pWord->aFirstCell();
  mpTask->m_pDataStack->vPushPointer(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enterRoutine -- enter word routine    (this is the does step for a : word) */
/*----------------------------------------------------------------------------*/
void Step::vEnterRoutine(MP_Task mpTask)
{
  ENTRY("vEnterRoutine"_Stz);
  
  StepFp* afpEntryCell = (mpTask->m_aFipCell - 1)->m_afpStep + 1;
  
  mpTask->m_pReturnStack->vPushCell(mpTask->m_aFipCell);
  mpTask->m_aFipCell = *(Cell**)afpEntryCell;

   if ( (mpTask->m_iTracing & C_TRACE_ROUTINES) )
    mpTask->vTracePoint(">"_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
