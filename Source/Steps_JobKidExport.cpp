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
  vAddStep("job_kidExport.exportOrigin"_Stz,   &Step::vJob_KidExportExportOrigin);
  vAddStep("job_kidExport.writeKidBuffer"_Stz, &Step::vJob_KidExportWriteKidBuffer);
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
/* job_kidExport.writeKidBuffer -- write buffer to export file                */
/*----------------------------------------------------------------------------*/
void Step::vJob_KidExportWriteKidBuffer(MP_Task mpTask)
{
  ENTRY("vJob_KidExportWriteKidBuffer"_Stz);
  
  Object* pPtrJob_KidExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_KidExport) )
    THROW_INVALID_DSTACK("job_kidExport.writeKidBuffer"_Stz);
  Job_KidExport* pJob_KidExport = _pJob_KidExport(pPtrJob_KidExport);
  
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
