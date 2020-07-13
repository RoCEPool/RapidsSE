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
  vAddStep("AggregateRsb[].aggregateCpy"_Stz, &Step::vAggregateRsbAggregateCpy);
  vAddStep("AggregateRsb[].aggregateGby"_Stz, &Step::vAggregateRsbAggregateGby);
  vAddStep("AggregateRsb[].delete"_Stz,    &Step::vAggregateRsbDelete);
  vAddStep("AggregateRsb[].get"_Stz,       &Step::vAggregateRsbGet);
  vAddStep("AggregateRsb[].new"_Stz,       &Step::vAggregateRsbNew);
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].AggregateCpy -- copy aggregate results from indexTree to rsb */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbAggregateCpy(void)
{
  ENTRY();
    
  IndexTree* pIndexTree = _pIndexTree(* m_pLogicUnit->m_xpIndexTree);
  
  Pot* pPot = * m_pLogicUnit->m_xpPot;
  
  m_pLogicUnit->m_xpAggregateRsb->vAggregateCpy(pPot,pIndexTree);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].AggregateGby -- Aggregate with group by an rsb buffer       */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbAggregateGby(void)
{
  ENTRY();

  
  RsbBuffer* pRsbBuffer = _pRsbBuffer(* m_pLogicUnit->m_xpOperandA);
    
  IndexTree* pIndexTree = _pIndexTree(* m_pLogicUnit->m_xpIndexTree);

  Pot* pPot = * m_pLogicUnit->m_xpPot;
  
  m_pLogicUnit->m_xpAggregateRsb->vAggregateGby(pPot,pIndexTree,pRsbBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].delete -- delete rsb stream Aggregate                       */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbDelete(void)
{
  ENTRY();
    
  AggregateRsb* pAggregateRsb = * m_pLogicUnit->m_xpAggregateRsb;
  
  m_pLogicUnit->m_xpAggregateRsb = nullptr;
  
  delete pAggregateRsb;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].get -- get rsb stream Aggregate                             */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpAggregateRsb);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* AggregateRsb[].new -- create a new rsb stream Aggregate                    */
/*----------------------------------------------------------------------------*/
void Step::vAggregateRsbNew(void)
{
  ENTRY();
  
  Pot* pPot = * m_pLogicUnit->m_xpPot;
  StreamDef* pStreamDefOut = * m_pLogicUnit->m_xpStreamDefOut;
  
  m_pLogicUnit->m_xpAggregateRsb = new AggregateRsb(pPot, pStreamDefOut);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

