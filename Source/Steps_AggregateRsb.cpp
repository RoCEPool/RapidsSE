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
  vAddStep("AggregateRsb[].aggregate"_Stz, &Step::vAggregateRsbAggregate);
  vAddStep("AggregateRsb[].aggregateCpy"_Stz, &Step::vAggregateRsbAggregateCpy);
  vAddStep("AggregateRsb[].aggregateGby"_Stz, &Step::vAggregateRsbAggregateGby);
  vAddStep("AggregateRsb[].delete"_Stz,    &Step::vAggregateRsbDelete);
  vAddStep("AggregateRsb[].get"_Stz,       &Step::vAggregateRsbGet);
  vAddStep("AggregateRsb[].new"_Stz,       &Step::vAggregateRsbNew);
  vAddStep("AggregateRsb[].newRow"_Stz,    &Step::vAggregateRsbNewRow);
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].aggregate -- Aggregate an rsb buffer                        */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbAggregate(void)
{
  ENTRY();
  
  RsbBuffer* pRsbBuffer = _pRsbBuffer(* m_pRegSet->m_xpOperandA);
  
  Expr* pExpr = * m_pRegSet->m_xpExpr;
  
  m_pRegSet->m_xpAggregateRsb->vAggregate(pRsbBuffer,pExpr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].AggregateCpy -- copy aggregate results from indexTree to rsb */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbAggregateCpy(void)
{
  ENTRY();
  
  Expr* pExpr = nullptr; //* m_pRegSet->m_xpExpr;
  
  IndexTree* pIndexTree = _pIndexTree(* m_pRegSet->m_xpIndexTree);
  Pot* pPot = * m_pRegSet->m_xpPot;
  
  m_pRegSet->m_xpAggregateRsb->vAggregateCpy(pPot,pIndexTree,pExpr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].AggregateGby -- Aggregate with group by an rsb buffer       */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbAggregateGby(void)
{
  ENTRY();

  
  RsbBuffer* pRsbBuffer = _pRsbBuffer(* m_pRegSet->m_xpOperandA);
  
  Expr* pExpr = * m_pRegSet->m_xpExpr;
  
  IndexTree* pIndexTree = _pIndexTree(* m_pRegSet->m_xpIndexTree);
  Pot* pPot = * m_pRegSet->m_xpPot;
  
  m_pRegSet->m_xpAggregateRsb->vAggregateGby(pPot,pIndexTree,pExpr,pRsbBuffer);
  
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
  
  Pot* pPot = * m_pRegSet->m_xpPot;
  StreamDef* pStreamDefOut = * m_pRegSet->m_xpStreamDefOut;
  
  m_pRegSet->m_xpAggregateRsb = new AggregateRsb(pPot, pStreamDefOut);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].newRow -- create a row in the rsb stream Aggregate          */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbNewRow(void)
{
  ENTRY();
  
  AggregateRsb* pAggregateRsb = _pAggregateRsb(* m_pRegSet->m_xpAggregateRsb);
  
  pAggregateRsb->vNewDstRsb();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

