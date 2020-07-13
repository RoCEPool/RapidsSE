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
  vAddStep("FilterRsb[].get"_Stz,       &Step::vFilterRsbGet);
  vAddStep("FilterRsb[].new"_Stz,       &Step::vFilterRsbNew);
}

/*----------------------------------------------------------------------------*/
/* FilterRsb[].delete -- delete rsb stream filter                             */
/*----------------------------------------------------------------------------*/
void Step::vFilterRsbDelete(void)
{
  ENTRY();
    
  FilterRsb* pFilterRsb = * m_pLogicUnit->m_xpFilterRsb;
  
  m_pLogicUnit->m_xpFilterRsb = nullptr;
  
  delete pFilterRsb;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* FilterRsb[].get -- get rsb stream filter                                   */
/*----------------------------------------------------------------------------*/
void Step::vFilterRsbGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpFilterRsb);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* FilterRsb[].new -- new rsb stream filter                                   */
/*----------------------------------------------------------------------------*/
void Step::vFilterRsbNew(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpFilterRsb = new FilterRsb;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
