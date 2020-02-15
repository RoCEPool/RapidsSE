/*============================================================================*/
/* Steps_IndexTreeReg.cpp - index[] step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexTreeReg(void)
{
  vAddStep("indexTree[].get"_Stz,  &Step::vIndexTreeRegGet);
  vAddStep("indexTree[].set"_Stz,  &Step::vIndexTreeRegSet);
}

/*----------------------------------------------------------------------------*/
/* indexTree[].get -- get index tree                                          */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeRegGet(void)
{
  ENTRY(); // ENTRY("vIndexTreeRegGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpIndexTree);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* indexTree[].set -- set index tree                                          */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeRegSet(void)
{
  ENTRY(); // ENTRY("vIndexTreeRegSet"_Stz);
  
  m_pRegSet->m_xpIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
