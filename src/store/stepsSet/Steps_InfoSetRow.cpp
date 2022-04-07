/*============================================================================*/
/* Steps_InfoSetRow.cpp -- InfoSetRow step group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_InfoSetRow(void)
{
  vAddStep("InfoSetRow[].get"_Stz,           &Step::vInfoSetRowGet);
  vAddStep("InfoSetRow[].getIntColumn"_Stz,  &Step::vInfoSetRowGetIntColumn);
  vAddStep("InfoSetRow[].getVchColumn"_Stz,  &Step::vInfoSetRowGetVchColumn);
  vAddStep("InfoSetRow[].isNull"_Stz,        &Step::vInfoSetRowIsNull);
  vAddStep("InfoSetRow[].set"_Stz,           &Step::vInfoSetRowSet);
}

/*----------------------------------------------------------------------------*/
/* InfoSetRow[].get -- get the row                                                   */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetRowGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpInfoSetRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSetRow[].getIntColumn -- get the integer column from the row                  */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetRowGetIntColumn(void)
{
  ENTRY();
  
  i8 i_Col = m_pDataStack->iPopInt(__func__);
  
  Row* pRow = *m_pLogicUnit->m_xpInfoSetRow;
  
  m_pDataStack->vPushInt(pRow->iColVal_int(i_Col));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSetRow[].getVchColumn -- get the varchar column from the row           */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetRowGetVchColumn(void)
{
  ENTRY();
  
  i8 i_Col = m_pDataStack->iPopInt(__func__);
  
  Row* pRow = *m_pLogicUnit->m_xpInfoSetRow;
  
  s256 sColumn = pRow->szColVal_ofs(i_Col);
  m_pDataStack->vPushStr(sColumn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSetRow[].isNull -- is row null ?                                              */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetRowIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpInfoSetRow.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSetRow[].set -- set row                                                       */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetRowSet(void)
{
  ENTRY();

  Row* pRow = _pRow(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpInfoSetRow = _pRow(pRow);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
