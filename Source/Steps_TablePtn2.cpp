/*============================================================================*/
/* Steps_TablePtn.cpp -- Table Partition Step Group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TablePartition2(void)
{
  vAddStep("tablePtn[].root"_Stz , &Step::vTablePtnRoot);
  vAddStep("tablePtn[].set"_Stz  , &Step::vTablePtnSet);
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
