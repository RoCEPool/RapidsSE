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
  vAddStep("ProjectRsb[].project"_Stz,   &Step::vProjectRsbProject);
  vAddStep("ProjectRsb[].get"_Stz,       &Step::vProjectRsbGet);
  vAddStep("ProjectRsb[].new"_Stz,       &Step::vProjectRsbNew);
}

/*----------------------------------------------------------------------------*/
/* ProjectRsb[].delete -- delete rsb stream project                           */
/*----------------------------------------------------------------------------*/
void Step::vProjectRsbDelete(void)
{
  ENTRY();
    
  ProjectRsb* pProjectRsb = * m_pRegSet->m_xpProjectRsb;
  
  m_pRegSet->m_xpProjectRsb = nullptr;
  
  delete pProjectRsb;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ProjectRsb[].project -- project an rsb buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vProjectRsbProject(void)
{
  ENTRY();

  StreamDef* pStreamDefOut = * m_pRegSet->m_xpStreamDefOut;
  
  RsbBuffer* pRsbBuffer = _pRsbBuffer(* m_pRegSet->m_xpOperandA);

  Expr* pExpr = * m_pRegSet->m_xpExpr;

  Pot* pPot = * m_pRegSet->m_xpPot;

  m_pRegSet->m_xpProjectRsb->vProject(pPot,pRsbBuffer,pStreamDefOut);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ProjectRsb[].get -- get rsb stream project                                 */
/*----------------------------------------------------------------------------*/
void Step::vProjectRsbGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pRegSet->m_xpProjectRsb);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ProjectRsb[].new -- create a new rsb stream project                        */
/*----------------------------------------------------------------------------*/
void Step::vProjectRsbNew(void)
{
  ENTRY();
    
  m_pRegSet->m_xpProjectRsb = new ProjectRsb;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

