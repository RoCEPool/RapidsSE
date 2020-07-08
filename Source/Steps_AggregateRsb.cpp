/*============================================================================*/
/* Steps_AggregateRsb.cpp -- Aggregate Rsb Stream step group                  */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_AggregateRsb(void)
{
  vAddStep("AggregateRsb[]"_Stz,           &Step::vAggregateRsbGet);
  vAddStep("AggregateRsb[].delete"_Stz,    &Step::vAggregateRsbDelete);
  vAddStep("AggregateRsb[].Aggregate"_Stz, &Step::vAggregateRsbAggregate);
  vAddStep("AggregateRsb[].get"_Stz,       &Step::vAggregateRsbGet);
  vAddStep("AggregateRsb[].new"_Stz,       &Step::vAggregateRsbNew);
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].Aggregate -- Aggregate an rsb buffer                        */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbAggregate(void)
{
  ENTRY();
  
  StreamDef* pStreamDefOut = * m_pRegSet->m_xpStreamDefOut;
  
  RsbBuffer* pRsbBuffer = _pRsbBuffer(* m_pRegSet->m_xpOperandA);
  
  Expr* pExpr = * m_pRegSet->m_xpExpr;
  
  Pot* pPot = * m_pRegSet->m_xpPot;
  
  IndexTree* pIndexTree = _pIndexTree(* m_pRegSet->m_xpIndexTree);
  
  m_pRegSet->m_xpAggregateRsb->vAggregate(pPot,pRsbBuffer,pIndexTree,pExpr,pStreamDefOut);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].delete -- delete rsb stream Aggregate                       */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbDelete(void)
{
  ENTRY();
    
  AggregateRsb* pAggregateRsb = * m_pRegSet->m_xpAggregateRsb;
  
  m_pRegSet->m_xpAggregateRsb = nullptr;
  
  delete pAggregateRsb;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].get -- get rsb stream Aggregate                             */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pRegSet->m_xpAggregateRsb);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].new -- create a new rsb stream Aggregate                    */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbNew(void)
{
  ENTRY();
    
  m_pRegSet->m_xpAggregateRsb = new AggregateRsb;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

