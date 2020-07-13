/*============================================================================*/
/* Steps_ProjectRsb.cpp -- Project Rsb Stream step group                      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ProjectRsb(void)
{
  vAddStep("ProjectRsb[]"_Stz,           &Step::vProjectRsbGet);
  vAddStep("ProjectRsb[].delete"_Stz,    &Step::vProjectRsbDelete);
  vAddStep("ProjectRsb[].get"_Stz,       &Step::vProjectRsbGet);
  vAddStep("ProjectRsb[].new"_Stz,       &Step::vProjectRsbNew);
}

/*----------------------------------------------------------------------------*/
/* ProjectRsb[].delete -- delete rsb stream project                           */
/*----------------------------------------------------------------------------*/
void Step::vProjectRsbDelete(void)
{
  ENTRY();
    
  ProjectRsb* pProjectRsb = * m_pLogicUnit->m_xpProjectRsb;
  
  m_pLogicUnit->m_xpProjectRsb = nullptr;
  
  delete pProjectRsb;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ProjectRsb[].get -- get rsb stream project                                 */
/*----------------------------------------------------------------------------*/
void Step::vProjectRsbGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpProjectRsb);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ProjectRsb[].new -- create a new rsb stream project                        */
/*----------------------------------------------------------------------------*/
void Step::vProjectRsbNew(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpProjectRsb = new ProjectRsb;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

