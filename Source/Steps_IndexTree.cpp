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
  
  
  m_pDataStack->vPushInt(C_UNKNOWN);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
