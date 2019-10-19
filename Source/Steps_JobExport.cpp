/*============================================================================*/
/* Steps_JobExport.cpp - jobExport step group                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobExport(void)
{
  vAddStep("Job_export.auxAckDecided@"_Stz, &Step::vJob_ExportAuxAckDecidedFetch);
  vAddStep("Job_export.auxAckDecided!"_Stz, &Step::vJob_ExportAuxAckDecidedStore);
  vAddStep("job_export.begin"_Stz,          &Step::vJob_ExportBegin);
  vAddStep("job_export.end"_Stz,            &Step::vJob_ExportEnd);
  vAddStep("Job_export.table!"_Stz,         &Step::vJob_ExportTableStore);
}

/*----------------------------------------------------------------------------*/
/* Job_export.auxAckDecided@ -- fetch if aux ack decided                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportAuxAckDecidedFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ExportAuxAckDecidedFetch"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJobExport) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(pJob_Export->m_bAuxAckDecided);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_export.auxAckDecided! -- store if aux ack decided                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportAuxAckDecidedStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ExportAuxAckDecidedStore"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJobExport) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  i8 iAuxAckDecided;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iAuxAckDecided) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  pJob_Export->m_bAuxAckDecided = (bool)iAuxAckDecided;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_export.begin -- begin export                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportBegin(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ExportBegin"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJobExport) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  bool bBegun = pJob_Export->bBegin();
  
  mpTask->m_pForth->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_export.end -- end export                                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportEnd(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ExportEnd"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJobExport) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  pJob_Export->vEnd();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_export.table! -- store table in the job                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_ExportTableStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_ExportTableStore"_Stz);
  
  Object* pPtrJobExport;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJobExport) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_Export* pJob_Export = _pJob_Export(pPtrJobExport);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pTablePtr) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_Export->m_pTable = pTable;
    
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
