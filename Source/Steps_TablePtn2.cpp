/*============================================================================*/
/* Steps_TablePtn.cpp -- Table Partition Step Group                           */
/*============================================================================*/

#include "Steps.h"

#define m_xptablePtn m_xpObject0

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TablePartition2(void)
{
  vAddStep("tablePtn[].set"_Stz , &Step::vTablePtnSet);
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].set -- set the tablePtn                                           */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnSet(void)
{
  ENTRY("vTablePtnSet"_Stz);
  
  Object* pPtrObject;
  if ( ! m_pDataStack->bPopObject(pPtrObject) )
    THROW_DSTACK();
  
  m_pRegSet->m_xptablePtn = pPtrObject;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
