/*============================================================================*/
/* Steps_JoinRsb.cpp -- Join Rsb Stream step group                            */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JoinRsb(void)
{
  vAddStep("JoinRsb[]"_Stz,           &Step::vJoinRsbGet);
  vAddStep("JoinRsb[].delete"_Stz,    &Step::vJoinRsbDelete);
  vAddStep("JoinRsb[].project"_Stz,   &Step::vJoinRsbProject);
  vAddStep("JoinRsb[].get"_Stz,       &Step::vJoinRsbGet);
  vAddStep("JoinRsb[].new"_Stz,       &Step::vJoinRsbNew);
}

/*----------------------------------------------------------------------------*/
/* JoinRsb[].delete -- delete rsb stream project                              */
/*----------------------------------------------------------------------------*/
void Step::vJoinRsbDelete(void)
{
  ENTRY();
    
  JoinRsb* pJoinRsb = * m_pRegSet->m_xpJoinRsb;
  
  m_pRegSet->m_xpJoinRsb = nullptr;
  
  delete pJoinRsb;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JoinRsb[].project -- project an rsb buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vJoinRsbProject(void)
{
  ENTRY();

  StreamDef* pStreamDefOut = * m_pRegSet->m_xpStreamDefOut;
  
  RsbBuffer* pRsbBuffer = _pRsbBuffer(* m_pRegSet->m_xpOperandA);

  Expr* pExpr = * m_pRegSet->m_xpExpr;

  Pot* pPot = * m_pRegSet->m_xpPot;

  m_pRegSet->m_xpJoinRsb->vJoin(pPot,pRsbBuffer,pStreamDefOut);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JoinRsb[].get -- get rsb stream project                                    */
/*----------------------------------------------------------------------------*/
void Step::vJoinRsbGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pRegSet->m_xpJoinRsb);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JoinRsb[].new -- create a new rsb stream project                           */
/*----------------------------------------------------------------------------*/
void Step::vJoinRsbNew(void)
{
  ENTRY();
    
  m_pRegSet->m_xpJoinRsb = new JoinRsb;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

