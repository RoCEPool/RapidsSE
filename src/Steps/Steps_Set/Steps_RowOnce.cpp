/*============================================================================*/
/* Steps_RowOnce.cpp - Row Once skiplist step group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RowOnce(void)
{
  vAddStep("rowOnce[]"_Stz,                &Step::vRowOnceGet);
  
  vAddStep("rowOnce[].delete"_Stz,         &Step::vRowOnceDelete);
  vAddStep("rowOnce[].isNull"_Stz,         &Step::vRowOnceIsNull);
  vAddStep("rowOnce[].new"_Stz,            &Step::vRowOnceNew);
}

/*----------------------------------------------------------------------------*/
/* rowOnce[].delete -- delete the rowOnce skiplist                            */
/*----------------------------------------------------------------------------*/
void Step::vRowOnceDelete(void)
{
  ENTRY();
  
  RowOnce* pRowOnce = _pRowOnce(*m_pLogicUnit->m_xpRowOnce);
  
  delete pRowOnce;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowOnce[] -- get rowOnce skipList                                          */
/*----------------------------------------------------------------------------*/
void Step::vRowOnceGet(void)
{
  ENTRY();
  
  RowOnce* pRowOnce = _pRowOnce(*m_pLogicUnit->m_xpRowOnce);
  
  m_pDataStack->vPushPtr(pRowOnce);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowOnce[].isNull -- is rowOnce skip list null ?                            */
/*----------------------------------------------------------------------------*/
void Step::vRowOnceIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowOnce.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowOnce[].new -- new a rowOnce skiplist                                    */
/*----------------------------------------------------------------------------*/
void Step::vRowOnceNew(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpRowOnce = new RowOnce;
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
