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
  vAddStep("FilterRsb[].completed"_Stz, &Step::vFilterRsbCompleted);
  vAddStep("FilterRsb[].dangling"_Stz,  &Step::vFilterRsbDangling);
  vAddStep("FilterRsb[].delete"_Stz,    &Step::vFilterRsbDelete);
  vAddStep("FilterRsb[].filter"_Stz,    &Step::vFilterRsbFilter);
  vAddStep("FilterRsb[].get"_Stz,       &Step::vFilterRsbGet);
  vAddStep("FilterRsb[].new"_Stz,       &Step::vFilterRsbNew);
}

/*----------------------------------------------------------------------------*/
/* FilterRsb[].completed -- get any completed/full buffer                     */
/*----------------------------------------------------------------------------*/
void Step::vFilterRsbCompleted(void)
{
  ENTRY();
  
  FilterRsb* pFilterRsb = * m_pRegSet->m_xpFilterRsb;

  i8 i_Ptn = 0;
  
  RsbBuffer* pRsbBuffer = pFilterRsb->pCompleted(i_Ptn);

  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* FilterRsb[].danglingRsbBuffer -- get any dangling/last buffer              */
/*----------------------------------------------------------------------------*/
void Step::vFilterRsbDangling(void)
{
  ENTRY();
  
  FilterRsb* pFilterRsb = * m_pRegSet->m_xpFilterRsb;

  i8 i_Ptn = 0;

  RsbBuffer* pRsbBuffer = pFilterRsb->pDangling(i_Ptn);
  
  m_pDataStack->vPushPtr(pRsbBuffer);

  EXIT();
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

  RsbBuffer* pRsbBuffer = _pRsbBuffer(m_pDataStack->pPopPtr(__func__));

  Expr* pExpr = * m_pRegSet->m_xpExpr;
  
  m_pRegSet->m_xpFilterRsb->vFilter(pRsbBuffer,pExpr);
  
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

