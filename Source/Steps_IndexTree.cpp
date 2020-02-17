/*============================================================================*/
/* Steps_IndexTree.cpp - index tree step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexTree(void)
{
  vAddStep("indexTree.insEntBuffer"_Stz,    &Step::vIndexTreeInsEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* indexTree.insEntBuffer -- insert all entries from given ent buffer         */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeInsEntBuffer(void)
{
  ENTRY(); // ENTRY("vIndexTreeInsEntBuffer"_Stz);
  
  IndexTree* pIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));

  EntBuffer* pEntBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nEntries = 0;
  
  for (Ent* aEnt = pEntBuffer->aFirstItem();
       aEnt;
       aEnt = pEntBuffer->aNextEntry(aEnt,0)) {
    
    // ..add code here to insert entry into index tree..
    
    nEntries++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushInt(nEntries);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
