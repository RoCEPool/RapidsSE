/*============================================================================*/
/* Steps_TablePtn.cpp -- Table Partition Step Group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TablePartition2(void)
{
  vAddStep("tablePtn[].get"_Stz,   &Step::vTablePtnGet);
  vAddStep("tablePtn[].purge"_Stz, &Step::vTablePtnPurge);
  vAddStep("tablePtn[].root"_Stz , &Step::vTablePtnRoot);
  vAddStep("tablePtn[].set"_Stz  , &Step::vTablePtnSet);
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].get -- get table                                                */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnGet(void)
{
  ENTRY(); // ENTRY("vTablePtnGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].purge -- purge prior versions of this table ptn                 */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnPurge(void)
{
  ENTRY(); // ENTRY("vTablePtnPurge"_Stz);
  
  TablePtn* pTablePtn = *m_pRegSet->m_xpTablePtn;
  
  // get the oldest perspective
  t8 tOldestPsp = AM::pTxSet()->tOldestPsp();
  pTablePtn->pReadApi()->vPurgePriorVersions(tOldestPsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].root -- get the tablePtn root                                   */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnRoot(void)
{
  ENTRY(); // ENTRY("vTablePtnSet"_Stz);
  
  TablePtn* pTablePtn = _pTablePtn(*m_pRegSet->m_xpTablePtn);
  
  m_pDataStack->vPushPtr(pTablePtn->pRootCrwBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].set -- set the tablePtn                                         */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnSet(void)
{
  ENTRY(); // ENTRY("vTablePtnSet"_Stz);
  
  m_pRegSet->m_xpTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
