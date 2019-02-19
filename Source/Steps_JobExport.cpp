/*============================================================================*/
/* Steps_JobExport.cpp - jobExport step group                                 */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobExport(void)
{
  vAddStep("job_export.begin"_Stz,          &Step::vJob_ExportBegin);
  vAddStep("job_export.decTapper"_Stz,      &Step::vJob_ExportDecTapper);
  vAddStep("job_export.end"_Stz,            &Step::vJob_ExportEnd);
  vAddStep("Job_export.table!"_Stz,         &Step::vJob_ExportTableStore);
  vAddStep("Job_export.tapsRemaining@"_Stz, &Step::vJob_ExportTapsRemainingFetch);
  vAddStep("Job_export.twiddling@"_Stz,     &Step::vJob_ExportTwiddlingFetch);
  vAddStep("Job_export.twiddling!"_Stz,     &Step::vJob_ExportTwiddlingStore);
}

/*----------------------------------------------------------------------------*/
/* job_export.begin -- begin export                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportBegin(MP_Task mpTask)
{
  ENTRY("vJob_ExportBegin"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobExport) )
    THROW_INVALID_DSTACK("job_export.begin"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  bool bBegun = pJob_Export->bBegin();
  
  mpTask->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_export.decTapper -- decrement taps remaining and return if decider     */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportDecTapper(MP_Task mpTask)
{
  ENTRY("vJob_ExportDecTapper"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobExport) )
    THROW_INVALID_DSTACK("job_export.decTapper"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);

  bool bAmTapper = pJob_Export->m_oAuxAck.bDecAndDecide();
  
  mpTask->m_pDataStack->vPushI8(bAmTapper);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_export.end -- end export                                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportEnd(MP_Task mpTask)
{
  ENTRY("vJob_ExportEnd"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobExport) )
    THROW_INVALID_DSTACK("job_export.end"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  pJob_Export->vEnd();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_export.table! -- store table in the job                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportTableStore(MP_Task mpTask)
{
  ENTRY("vJob_ExportTableStore"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobExport) )
    THROW_INVALID_DSTACK("Job_export.table!"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("Job_export.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_Export->m_pTable = pTable;
  
  ActionSequence::vAdd(pJob_Export,AN_TABLE_USE,pTable->aTableName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_export.tapsRemaining@ -- fetch no of taps remaining                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportTapsRemainingFetch(MP_Task mpTask)
{
  ENTRY("vJob_ExportTapsRemainingFetch"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobExport) )
    THROW_INVALID_DSTACK("Job_export.tapsRemaining@"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  i8 iTapsRemaining = pJob_Export->m_oAuxAck.iPrintOnlyValue();
  
  mpTask->m_pDataStack->vPushI8(iTapsRemaining);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_export.twiddling@ -- fetch tapper::twiddling                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportTwiddlingFetch(MP_Task mpTask)
{
  ENTRY("vJob_ExportTwiddlingFetch"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobExport) )
    THROW_INVALID_DSTACK("Job_export.twiddling@"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  bool bTwiddling = pJob_Export->m_oAuxAck.bTwiddling();
  
  mpTask->m_pDataStack->vPushI8(bTwiddling);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_export.twiddling! -- store tapper::twiddling                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportTwiddlingStore(MP_Task mpTask)
{
  ENTRY("vJob_ExportTwiddlingStore"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobExport) )
    THROW_INVALID_DSTACK("Job_export.twiddling!"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);

  i8 iTwiddling;
  if ( ! mpTask->m_pDataStack->bPop(iTwiddling) )
    THROW_INVALID_DSTACK("Job_export.twiddling!"_Stz);

  pJob_Export->m_oAuxAck.vTwiddling(iTwiddling);
  
  EXIT();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
