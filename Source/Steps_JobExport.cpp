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
  vAddStep("Job_export.auxAckDecided@"_Stz, &Step::vJob_ExportAuxAckDecidedFetch);
  vAddStep("Job_export.auxAckDecided!"_Stz, &Step::vJob_ExportAuxAckDecidedStore);
  vAddStep("Job_export.auxRemaining@"_Stz,  &Step::vJob_ExportAuxRemainingFetch);
  vAddStep("job_export.begin"_Stz,          &Step::vJob_ExportBegin);
  vAddStep("job_export.decAuxAck"_Stz,      &Step::vJob_ExportDecAuxAck);
  vAddStep("job_export.end"_Stz,            &Step::vJob_ExportEnd);
  vAddStep("Job_export.table!"_Stz,         &Step::vJob_ExportTableStore);
}

/*----------------------------------------------------------------------------*/
/* Job_export.auxAckDecided@ -- fetch if aux ack decided                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportAuxAckDecidedFetch(MP_Task mpTask)
{
  ENTRY("vJob_ExportAuxAckDecidedFetch"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobExport) )
    THROW_INVALID_DSTACK("Job_export.auxAckDecided@"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  bool bPassed = pJob_Export->m_oAuxAck.bPassed();
  
  mpTask->m_pDataStack->vPushI8(bPassed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_export.auxAckDecided! -- store if aux ack decided                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportAuxAckDecidedStore(MP_Task mpTask)
{
  ENTRY("vJob_ExportAuxAckDecidedStore"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobExport) )
    THROW_INVALID_DSTACK("Job_export.auxAckDecided!"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  i8 iPassed;
  if ( ! mpTask->m_pDataStack->bPop(iPassed) )
    THROW_INVALID_DSTACK("Job_export.passed!"_Stz);
  
  pJob_Export->m_oAuxAck.vPassed(iPassed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_export.auxRemaining@ -- fetch no of aux acks remaining                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportAuxRemainingFetch(MP_Task mpTask)
{
  ENTRY("vJob_ExportAuxRemainingFetch"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobExport) )
    THROW_INVALID_DSTACK("Job_export.auxRemaining@"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  i8 iAuxRemaining = pJob_Export->m_oAuxAck.iPrintOnlyValue();
  
  mpTask->m_pDataStack->vPushI8(iAuxRemaining);
  
  EXIT();
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
/* job_export.decAuxAck -- decrement auxack remaining and return if decider   */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportDecAuxAck(MP_Task mpTask)
{
  ENTRY("vJob_ExportDecAuxAck"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobExport) )
    THROW_INVALID_DSTACK("job_export.decAuxAck"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);

  bool bAmDecider = pJob_Export->m_oAuxAck.bDecAndDecide();
  
  mpTask->m_pDataStack->vPushI8(bAmDecider);
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
