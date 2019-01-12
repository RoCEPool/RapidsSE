/*============================================================================*/
/* Steps_Enter.cpp - enter step group                                         */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
  
  mpTask->m_pReturnStack->vPushCell(mpTask->FIP.m_aCell);
  mpTask->FIP.m_aCell = *(Cell**)mpTask->m_pEntryCell;
  
  //  Word* pWord = pAdd<Word*>(mpTask->m_pEntryCell,-(sizeof(Word)-sizeof(Cell*)));

  Word* pWord = pAdd<Word*>(mpTask->m_pEntryCell,
                            -( sizeof(Word)
                               - sizeof(Cell*)
                               - sizeof(WspFxd<C_WSP_WORD>)
                               - sizeof(Footer)
                             )
                            );

  ASSERT(WT_OBJECT == pWord->eWordType());
  
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
  
  mpTask->m_pReturnStack->vPushCell(mpTask->FIP.m_aCell);
  mpTask->FIP.m_aCell = *(Cell**)mpTask->m_pEntryCell;
 
   if ( (mpTask->m_iTracing & C_TRACE_ROUTINES) )
    mpTask->vTracePoint(">"_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
