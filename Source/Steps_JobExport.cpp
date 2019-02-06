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
  vAddStep("job_export.begin"_Stz,     &Step::vJob_ExportBegin);
  vAddStep("job_export.end"_Stz,       &Step::vJob_ExportEnd);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
