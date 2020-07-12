/*============================================================================*/
/* Steps_FilterRsb.cpp -- Filter Rsb Stream step group                        */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_FilterRsb(void)
{
  vAddStep("FilterRsb[]"_Stz,           &Step::vFilterRsbGet);
  vAddStep("FilterRsb[].delete"_Stz,    &Step::vFilterRsbDelete);
  vAddStep("FilterRsb[].filter"_Stz,    &Step::vFilterRsbFilter);
  vAddStep("FilterRsb[].get"_Stz,       &Step::vFilterRsbGet);
  vAddStep("FilterRsb[].new"_Stz,       &Step::vFilterRsbNew);
}

/*----------------------------------------------------------------------------*/
/* FilterRsb[].delete -- delete rsb stream filter                             */
/*----------------------------------------------------------------------------*/
void Step::vFilterRsbDelete(void)
{
  ENTRY();
    
  FilterRsb* pFilterRsb = * m_pRegSet->m_xpFilterRsb;
  
  m_pRegSet->m_xpFilterRsb = nullptr;
  
  delete pFilterRsb;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* FilterRsb[].filter -- filter an rsb buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vFilterRsbFilter(void)
{
  ENTRY();

  RsbBuffer* pRsbBuffer = _pRsbBuffer(* m_pRegSet->m_xpOperandA);

  Expr* pExpr = * m_pRegSet->m_xpExpr;

  Pot* pPot = * m_pRegSet->m_xpPot;

  m_pRegSet->m_xpFilterRsb->vFilter(pPot,pRsbBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* FilterRsb[].get -- get rsb stream filter                                   */
/*----------------------------------------------------------------------------*/
void Step::vFilterRsbGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pRegSet->m_xpFilterRsb);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* FilterRsb[].new -- new rsb stream filter                                   */
/*----------------------------------------------------------------------------*/
void Step::vFilterRsbNew(void)
{
  ENTRY();
    
  m_pRegSet->m_xpFilterRsb = new FilterRsb;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
