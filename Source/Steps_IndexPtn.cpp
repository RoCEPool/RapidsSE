/*============================================================================*/
/* Steps_IndexPtn.cpp - index tree step group                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexPtn(void)
{
  vAddStep("IndexPtn.insEntBuffer"_Stz,    &Step::vIndexPtnInsEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* IndexPtn.insEntBuffer -- insert all entries from given ent buffer          */
/*----------------------------------------------------------------------------*/
void Step::vIndexPtnInsEntBuffer(void)
{
  ENTRY();
  
  IndexPtn* pIndexPtn = _pIndexPtn(m_pDataStack->pPopPtr(__func__));

  EntBuffer* pEntBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nEntries = 0;
  
  for (Ent* aEnt = pEntBuffer->aFirstItem();
       aEnt;
       aEnt = pEntBuffer->aNextEntry(aEnt,0)) {
    
    // ..add code here to insert entry into index tree..
    
    bool bInserted = pIndexPtn->bInsertEntry(aEnt);
    
    nEntries++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushInt(nEntries);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
