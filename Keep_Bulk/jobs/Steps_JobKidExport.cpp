/*============================================================================*/
/* Steps_JobKidExport.cpp - jobKidExport step group                           */
/*============================================================================*/

#include "Steps.h"

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
void Step::vJob_KidExportExportJobId(void)
{
  ENTRY("vJob_KidExportExportJobId"_Stz);
  
  Object* pPtrJob_KidExport;
  if ( ! m_pDataStack->bPopObject(pPtrJob_KidExport) )
    THROW_DSTACK();
  Job_KidExport* pJob_KidExport = _pJob_KidExport(pPtrJob_KidExport);
    
  m_pDataStack->vPushI8(pJob_KidExport->m_iExportJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_kidExport.exportOrigin -- get the node no where export began           */
/*----------------------------------------------------------------------------*/
void Step::vJob_KidExportExportOrigin(void)
{
  ENTRY("vJob_KidExportExportOrigin"_Stz);
  
  Object* pPtrJob_KidExport;
  if ( ! m_pDataStack->bPopObject(pPtrJob_KidExport) )
    THROW_DSTACK();
  Job_KidExport* pJob_KidExport = _pJob_KidExport(pPtrJob_KidExport);
  
  i8 i_OriginNodeNo = Job::iNodeNo(pJob_KidExport->m_iExportJobId);
  
  m_pDataStack->vPushI8(i_OriginNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_kidExport.writeExportBuffer -- write buffer to export file             */
/*----------------------------------------------------------------------------*/
void Step::vJob_KidExportWriteExportBuffer(void)
{
  ENTRY("vJob_KidExportWriteExportBuffer"_Stz);
  
  Object* pPtrJob_KidExport;
  if ( ! m_pDataStack->bPopObject(pPtrJob_KidExport) )
    THROW_DSTACK();
  Job_KidExport* pJob_KidExport = _pJob_KidExport(pPtrJob_KidExport);
  
  Object* pPtrJob_Export;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Export) )
    THROW_DSTACK();
  Job_Export* pJob_Export = _pJob_Export(pPtrJob_Export);
  
  bool bHasCompleted = pJob_KidExport->bWriteBuffer(pJob_Export);
  
  if (bHasCompleted)
    vTrace("COMPLETED\n"_Stz);
  
  m_pDataStack->vPushI8(bHasCompleted);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
