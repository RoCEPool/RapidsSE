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
    
  ProjectRsb* pProjectRsb = * m_pLogicUnit->m_xpProjectRsb;
  
  m_pLogicUnit->m_xpProjectRsb = nullptr;
  
  delete pProjectRsb;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ProjectRsb[].project -- project an rsb buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vProjectRsbProject(void)
{
  ENTRY();

  StreamDef* pStreamDefOut = * m_pLogicUnit->m_xpStreamDefOut;
  
  RsbBuffer* pRsbBuffer = _pRsbBuffer(* m_pLogicUnit->m_xpOperandA);

  Pot* pPot = * m_pLogicUnit->m_xpPot;

  m_pLogicUnit->m_xpProjectRsb->vProject(pPot,pRsbBuffer,pStreamDefOut);
  
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

