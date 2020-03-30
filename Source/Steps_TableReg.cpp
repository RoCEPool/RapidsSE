/*============================================================================*/
/* Steps_table[].cpp - table step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TableReg(void)
{
  vAddStep("table[].columnCount"_Stz,        &Step::vTableColumnCount);
  vAddStep("table[]"_Stz,                    &Step::vTableRegGet);
  vAddStep("table[].set"_Stz,                &Step::vTableRegSet);
  vAddStep("table[].tablePtn@"_Stz,          &Step::vTableRegPtnFetch);
}

/*----------------------------------------------------------------------------*/
/* table.columnCount -- get column count                                      */
/*----------------------------------------------------------------------------*/
void Step::vTableColumnCount(void)
{
  ENTRY(); // ENTRY("vTableColumnCount"_Stz);
  
  StreamDef* pStreamDef = m_pRegSet->m_xpTable->pStreamDef();
  i8 nColumns = pStreamDef->nColumns(C_ALL);
  
  m_pDataStack->vPushInt(nColumns);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[] -- get table                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTableRegGet(void)
{
  ENTRY(); // ENTRY("vTableRegGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].set -- set table                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableRegSet(void)
{
  ENTRY(); // ENTRY("vTableRegSet"_Stz);
  
  m_pRegSet->m_xpTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].tablePtn@ -- feth the table partition                              */
/*----------------------------------------------------------------------------*/
void Step::vTableRegPtnFetch(void)
{
  ENTRY(); // ENTRY("vTableSet"_Stz);
  
  Job* pJob = m_pTask->m_pJob;
  
  i8 iPtn = pJob->m_pOp->nPartition();
  
  i8 iSlot = TablePtn::iPartitionToSlot(iPtn);
  TablePtn* pTablePtn = m_pRegSet->m_xpTable->pSlots()->pPartition(iSlot);
  
  m_pDataStack->vPushPtr(pTablePtn);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
