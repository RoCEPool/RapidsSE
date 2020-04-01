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
  vAddStep("Row[].isNull"_Stz,        &Step::vRowIsNull);
  vAddStep("Row[].set"_Stz,           &Step::vRowSet);
}

/*----------------------------------------------------------------------------*/
/* Row[].get -- get the row                                                   */
/*----------------------------------------------------------------------------*/
void Step::vRowGet(void)
{
  ENTRY(); // ENTRY("vRowGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Row[].getIntColumn -- get the integer column from the row                  */
/*----------------------------------------------------------------------------*/
void Step::vRowGetIntColumn(void)
{
  ENTRY(); // ENTRY("vRowGetIntColumn"_Stz);
  
  i8 iColumn = m_pDataStack->iPopInt(__func__);
  
  Row* pRow = *m_pRegSet->m_xpRow;
  m_pDataStack->vPushInt(pRow->m_waColVal[iColumn]);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Row[].isNull -- is row null ?                                              */
/*----------------------------------------------------------------------------*/
void Step::vRowIsNull(void)
{
  ENTRY(); // ENTRY("vRowIsNull"_Stz);
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpRow.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Row[].set -- set row                                                       */
/*----------------------------------------------------------------------------*/
void Step::vRowSet(void)
{
  ENTRY(); // ENTRY("vRowSet"_Stz);
  
  m_pRegSet->m_xpRow = _pRow(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
