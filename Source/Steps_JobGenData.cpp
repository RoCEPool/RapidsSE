/*============================================================================*/
/* Steps_JobGenData.cpp - job_genData step group                              */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobGenData(void)
{
  vAddStep("job_genData.generate"_Stz,     &Step::vJob_GenDataGenerate);
  vAddStep("job_genData.table!"_Stz,       &Step::vJob_GenDataTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_genData.generate -- generate data for a table                          */
/*----------------------------------------------------------------------------*/
void Step::vJob_GenDataGenerate(MP_Task mpTask)
{
  ENTRY("vJob_GenDataGenerate"_Stz);
  
  Object* pPtrJob_GenData;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_GenData) )
    THROW_INVALID_DSTACK("job_genData.generate"_Stz);
  Job_GenData* pJob_GenData = _pJob_GenData(pPtrJob_GenData);
  
  pJob_GenData->vGenerate();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_genData.table! -- store table into job_genData                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_GenDataTableStore(MP_Task mpTask)
{
  ENTRY("vJob_GenDataTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_GenData;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_GenData,pPtrTable) )
    THROW_INVALID_DSTACK("job_genData.tableStore"_Stz);
  
  Job_GenData* pJob_GenData = _pJob_GenData(pPtrJob_GenData);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_GenData->vTable(pTable);
  
  EXIT();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
