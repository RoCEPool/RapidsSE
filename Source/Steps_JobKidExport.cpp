/*============================================================================*/
/* Steps_JobKidExport.cpp - jobKidExport step group                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobKidExport(void)
{
  vAddStep("job_kidExport.exportJobId"_Stz,       &Step::vJob_KidExportExportJobId);
  vAddStep("job_kidExport.exportOrigin"_Stz,      &Step::vJob_KidExportExportOrigin);
  vAddStep("job_kidExport.writeExportBuffer"_Stz, &Step::vJob_KidExportWriteExportBuffer);
}

/*----------------------------------------------------------------------------*/
/* job_kidExport.exportJobId -- get the job id of the original export job     */
/*----------------------------------------------------------------------------*/
void Step::vJob_KidExportExportJobId(MP_Task mpTask)
{
  ENTRY("vJob_KidExportExportJobId"_Stz);
  
  Object* pPtrJob_KidExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_KidExport) )
    THROW_INVALID_DSTACK("job_kidExport.exportJobId"_Stz);
  Job_KidExport* pJob_KidExport = _pJob_KidExport(pPtrJob_KidExport);
    
  mpTask->m_pDataStack->vPushI8(pJob_KidExport->m_iExportJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_kidExport.exportOrigin -- get the node no where export began           */
/*----------------------------------------------------------------------------*/
void Step::vJob_KidExportExportOrigin(MP_Task mpTask)
{
  ENTRY("vJob_KidExportExportOrigin"_Stz);
  
  Object* pPtrJob_KidExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_KidExport) )
    THROW_INVALID_DSTACK("job_kidExport.exportOrigin"_Stz);
  Job_KidExport* pJob_KidExport = _pJob_KidExport(pPtrJob_KidExport);
  
  i8 i_OriginNodeNo = Job::iNodeNo(pJob_KidExport->m_iExportJobId);
  
  mpTask->m_pDataStack->vPushI8(i_OriginNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_kidExport.writeExportBuffer -- write buffer to export file             */
/*----------------------------------------------------------------------------*/
void Step::vJob_KidExportWriteExportBuffer(MP_Task mpTask)
{
  ENTRY("vJob_KidExportWriteExportBuffer"_Stz);
  
  Object* pPtrJob_KidExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_KidExport) )
    THROW_INVALID_DSTACK("job_kidExport.writeExportBuffer"_Stz);
  Job_KidExport* pJob_KidExport = _pJob_KidExport(pPtrJob_KidExport);
  
  Object* pPtrJob_Export;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Export) )
    THROW_INVALID_DSTACK("job_kidExport.writeExportBuffer"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJob_Export);
  
  bool bHasCompleted = pJob_KidExport->bWriteBuffer(pJob_Export);
  
  if (bHasCompleted)
    vTrace("COMPLETED\n"_Stz);
  
  mpTask->m_pDataStack->vPushI8(bHasCompleted);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
