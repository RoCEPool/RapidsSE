/*============================================================================*/
/* Steps_table[].cpp - table step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Table2(void)
{
  vAddStep("table[].set"_Stz,                &Step::vTableSet);
  vAddStep("table[].tablePtn@"_Stz,          &Step::vTablePtnFetch);
}

/*----------------------------------------------------------------------------*/
/* table[].set -- set table                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableSet(void)
{
  ENTRY(); // ENTRY("vTableSet"_Stz);
  
  m_pRegSet->m_xpTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].tablePtn@ -- feth the table partition                              */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnFetch(void)
{
  ENTRY(); // ENTRY("vTableSet"_Stz);
  
  Job_Universal* pJob = _pJob_Universal(m_pDataStack->pPopPtr(__func__));
  
  i8 iPtn = pJob->m_pOp->m_nPartition;
  
  i8 iSlot = TablePtn::iPartitionToSlot(iPtn);
  TablePtn* pTablePtn = m_pRegSet->m_xpTable->pSlots()->pPartition(iSlot);
  
  m_pDataStack->vPushPtr(pTablePtn);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
