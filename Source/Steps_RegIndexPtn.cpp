/*============================================================================*/
/* Steps_RegIndexTree.cpp - indexTree[] step group                              */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegIndexTree(void)
{
  vAddStep("IndexTree[].get"_Stz,  &Step::vRegIndexTreeGet);
  vAddStep("IndexTree[].set"_Stz,  &Step::vRegIndexTreeSet);
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].get -- get index tree                                           */
/*----------------------------------------------------------------------------*/
void Step::vRegIndexTreeGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpIndexTree);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].set -- set index tree                                           */
/*----------------------------------------------------------------------------*/
void Step::vRegIndexTreeSet(void)
{
  ENTRY();
  
  m_pRegSet->m_xpIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
