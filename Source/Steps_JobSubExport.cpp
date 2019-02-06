/*============================================================================*/
/* Steps_JobSubExport.cpp - jobSubExport step group                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSubExport(void)
{
  vAddStep("Job_subExporter::spawn"_Stz,     &Step::vJob_SubExporterSpawn);

  vAddStep("job_subExporter.exportJob@"_Stz, &Step::vJob_SubExporterExportJobFetch);
}

/*----------------------------------------------------------------------------*/
/* job_subExporter::spawn -- spawn a new subexporter                          */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubExporterSpawn(MP_Task mpTask)
{
  ENTRY("vJob_SubExporterSpawn"_Stz);
  
  Object* pPtrJob_Export;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Export) )
    THROW_INVALID_DSTACK("Job_subExporter::spawn"_Stz);
  Job_Export* pJob_Export = _pJob_Export(pPtrJob_Export);
  
  i8 i_Slot;
  if ( ! mpTask->m_pDataStack->bPop(i_Slot) )
    THROW_INVALID_DSTACK("Job_subExporter::spawn"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = pJob_Export->m_pSession;
  
  //! this needs to be optimised
  JobClass* pJobClass_SubExport = JobClass::pFind(CN_JOB_SUBEXPORT);
  
  Job_SubExport* pJob_SubExport =
    static_cast<Job_SubExport*>(pJobClass_SubExport->pPreJob(pSession));
  
  pJob_SubExport->m_pJobExport = pJob_Export;
  pJob_SubExport->m_iSlot = i_Slot;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pJob_SubExport->vSeed(SS_SEED_RUN);

  IGNORE_RETURN(Api::bSpawnJob(pSession,pJob_SubExport,SS_SEED_RUN));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subExporter.exportJob@ -- get export job                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubExporterExportJobFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubExporterSpawn"_Stz);
  
  Object* pPtrJob_SubExport;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubExport) )
    THROW_INVALID_DSTACK("Job_subExporter.exportJob@"_Stz);
  Job_SubExport* pJob_SubExport = _pJob_SubExport(pPtrJob_SubExport);
  
  mpTask->m_pDataStack->vPushPointer(pJob_SubExport->m_pJobExport);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
