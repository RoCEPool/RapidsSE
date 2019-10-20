/*============================================================================*/
/* Steps_JobGenData.cpp - job_genData step group                              */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobGenData(void)
{
  vAddStep("job_genData.generate"_Stz,     &Step::vJob_GenerateGenerate);
  vAddStep("job_genData.table!"_Stz,       &Step::vJob_GenerateTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_genData.generate -- generate data for a table                          */
/*----------------------------------------------------------------------------*/
void Step::vJob_GenerateGenerate(void)
{
  ENTRY("vJob_GenerateGenerate"_Stz);
  
  Object* pPtrJob_Generate;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Generate) )
    THROW_DSTACK();
  Job_Generate* pJob_Generate = _pJob_Generate(pPtrJob_Generate);
  
  pJob_Generate->vGenerate();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_genData.table! -- store table into job_genData                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_GenerateTableStore(void)
{
  ENTRY("vJob_GenerateTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_Generate;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Generate,pPtrTable) )
    THROW_DSTACK();
  
  Job_Generate* pJob_Generate = _pJob_Generate(pPtrJob_Generate);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_Generate->vTable(pTable);
  
  EXIT();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
