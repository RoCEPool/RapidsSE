/*============================================================================*/
/* Steps_Row.cpp -- Row step group                                            */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Row(void)
{
  vAddStep("Row[].get"_Stz,           &Step::vRowGet);
  vAddStep("Row[].getIntColumn"_Stz,  &Step::vRowGetIntColumn);
  vAddStep("Row[].getVchColumn"_Stz,  &Step::vRowGetVchColumn);
  vAddStep("Row[].isNull"_Stz,        &Step::vRowIsNull);
  vAddStep("Row[].set"_Stz,           &Step::vRowSet);
}

/*----------------------------------------------------------------------------*/
/* Row[].get -- get the row                                                   */
/*----------------------------------------------------------------------------*/
void Step::vRowGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Row[].getIntColumn -- get the integer column from the row                  */
/*----------------------------------------------------------------------------*/
void Step::vRowGetIntColumn(void)
{
  ENTRY();
  
  i8 i_Col = m_pDataStack->iPopInt(__func__);
  
  Row* pRow = *m_pLogicUnit->m_xpRow;
  
  m_pDataStack->vPushInt(pRow->iColVal_int(i_Col));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Row[].getVchColumn -- get the varchar column from the row                  */
/*----------------------------------------------------------------------------*/
void Step::vRowGetVchColumn(void)
{
  ENTRY();
  
  i8 i_Col = m_pDataStack->iPopInt(__func__);
  
  Row* pRow = *m_pLogicUnit->m_xpRow;
  
  s256 sColumn = pRow->szColVal_ofs(i_Col);
  m_pDataStack->vPushStr(sColumn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Row[].isNull -- is row null ?                                              */
/*----------------------------------------------------------------------------*/
void Step::vRowIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpRow.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Row[].set -- set row                                                       */
/*----------------------------------------------------------------------------*/
void Step::vRowSet(void)
{
  ENTRY();

  Row* pRow = _pRow(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpRow = _pRow(pRow);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
